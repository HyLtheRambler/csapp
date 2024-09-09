/* 
 * Simple, 32-bit and 64-bit clean allocator based on implicit free
 * lists, first-fit placement, and boundary tag coalescing, as described
 * in the CS:APP3e text. Blocks must be aligned to doubleword (8 byte) 
 * boundaries. Minimum block size is 16 bytes. 
 * 
 * Based on above, changed to explicit free list with LIFO free policy
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */


/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */ 
#define DSIZE       8       /* Double word size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */  

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) 

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   
#define GET_ALLOC(p) (GET(p) & 0x1)                    


/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) 
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) 

/* Given an uninitialized free block, compute the address of prev and succ*/
#define PREV_PTR(bp)   ((char *)(bp))
#define NEXT_PTR(bp)   ((char *)(bp) + WSIZE)

/* compute, put and read the offset between two ptrs in heap */
#define GET_OFFSET(ep, bp) ((int)((char *)(ep) - (char *)(bp)))
#define PUT_OFFSET(p, val) (*(int *)(p) = (val))
#define READ_OFFSET(p) (*(int *)(p))

/* Given free block ptr bp, compute address of next and previous free ones*/
#define PREV_FREE(bp)  ((char *)(bp) + READ_OFFSET((char *)(bp)))
#define NEXT_FREE(bp)  ((char *)(bp) + READ_OFFSET((char *)(bp) + WSIZE))

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */  
static char *root = 0;        /* Root of the free list */
static char *end_listp = 0;   /* End of the free list */
/* print debug message */
int debug = 0;

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void LIFO_insert(void *bp);
static void delete_node(void *bp);

/* 
 * mm_init - Initialize the memory manager 
 */
int mm_init(void) 
{
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) 
        return -1;
    PUT(heap_listp, 0);                          /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */ 
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */ 
    PUT(heap_listp + (3*WSIZE), PACK(0, 1));     /* Epilogue header */
    heap_listp += (2*WSIZE);                     
    end_listp = heap_listp;                      /* Set end of free list */
    root = heap_listp;                           /* Set roor of free list*/

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
        return -1;
    return 0;
}

/* 
 * malloc - Allocate a block with at least size bytes of payload 
 */
void *malloc(size_t size) 
{
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;      

    if (heap_listp == 0){
        mm_init();
    }
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)                                          
        asize = 2*DSIZE;                                        
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE); 

    if (debug) {
        printf("Before find_fit\n");
        mm_checkheap(1);
    }
    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {  
        place(bp, asize);                  
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);                 
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
        return NULL;                                  
    place(bp, asize);                                 
    return bp;
} 

/* 
 * free - Free a block 
 */
void free(void *bp)
{
    if (debug) {
        printf("-----freeing pointer %p-----\n", bp);
    }
    if (bp == 0) 
        return;

    size_t size = GET_SIZE(HDRP(bp));
    if (heap_listp == 0){
        mm_init();
    }

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    
    coalesce(bp);
}

/*
 * realloc - Naive implementation of realloc
 */
void *realloc(void *ptr, size_t size)
{
    size_t oldsize;
    void *newptr;

    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0) {
        mm_free(ptr);
        return 0;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if(ptr == NULL) {
        return mm_malloc(size);
    }

    newptr = mm_malloc(size);

    /* If realloc() fails the original block is left untouched  */
    if(!newptr) {
        return 0;
    }

    /* Copy the old data. */
    oldsize = GET_SIZE(HDRP(ptr));
    if(size < oldsize) oldsize = size;
    memcpy(newptr, ptr, oldsize);

    /* Free the old block. */
    mm_free(ptr);

    return newptr;
}

/* 
 * mm_checkheap - Check the heap for correctness. Helpful hint: You
 *                can call this function using mm_checkheap(__LINE__);
 *                to identify the line number of the call site.
 */
void mm_checkheap(int lineno)  
{ 
    printf("*** checkheap begin ***\n");
    lineno = lineno; /* keep gcc happy */
    void *bp = root;
    printf("root: %p\n", bp);
    printf("end_listp: %p\n", end_listp);
    for (; bp != end_listp; bp = NEXT_FREE(bp)) {
        printf("\n");
        printf("PREV_FREE: %p\n", PREV_FREE(bp));
        printf("bp: %p\n", bp);
        printf("NEXT_FREE: %p\n", NEXT_FREE(bp));
        printf("Current block size: %d\n", GET_SIZE(HDRP(bp)));
        if (NEXT_FREE(bp) == bp) {
            printf("quit here\n");
            exit(0);
        }
    }
    printf("**** checkheap end ****\n");
}

/* 
 * The remaining routines are internal helper routines 
 */

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap(size_t words) 
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; 
    if ((long)(bp = mem_sbrk(size)) == -1)  
        return NULL;                                        

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */   
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   

    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ 

    /* Coalesce if the previous block was free */
    if (debug) {
        printf("after extend_heap:\n");
        mm_checkheap(1);
        printf("Check twice\n");
        mm_checkheap(1);
    }
    return coalesce(bp);                                          
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) 
{
    if (debug) {
        printf("Before coalesce");
        mm_checkheap(1);
    }

    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1 */
        /* set the free list */
        LIFO_insert(bp);
        return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        delete_node(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size,0));
        LIFO_insert(bp);
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        delete_node(bp);
        LIFO_insert(bp);
    }

    else {                                     /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
            GET_SIZE(FTRP(NEXT_BLKP(bp)));
        delete_node(NEXT_BLKP(bp));
        delete_node(PREV_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        LIFO_insert(bp);
    }

    if (debug) {
        printf("After coalesce:\n");
        mm_checkheap(1);
    }
    return bp;
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));   

    if ((csize - asize) >= (2*DSIZE)) { 
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));

        /* Set the free list */
        void *ep = PREV_BLKP(bp);
        if (root == ep) { 
            /* No prev block */
            root = bp;
            PUT_OFFSET(PREV_PTR(bp), GET_OFFSET(end_listp, bp));
        } else { 
            /* Normal prev block */
            /* adjust next ptr of prev block */
            PUT_OFFSET(NEXT_PTR(PREV_FREE(ep)), READ_OFFSET(NEXT_PTR(PREV_FREE(ep))) + GET_OFFSET(bp, ep));
            /* set prev ptr of current block */
            PUT_OFFSET(PREV_PTR(bp), READ_OFFSET(PREV_PTR(ep)) + GET_OFFSET(ep, bp));
        }
        if (NEXT_FREE(ep) == end_listp) { 
            /* No next block */
            PUT_OFFSET(NEXT_PTR(bp), GET_OFFSET(end_listp, bp));
        } else { 
            /* Normal next block */
            /* adjust prev ptr of next block */
            PUT_OFFSET(PREV_PTR(NEXT_FREE(ep)), READ_OFFSET(PREV_PTR(NEXT_FREE(ep))) + GET_OFFSET(bp, ep));
            /* set next ptr of current block */
            PUT_OFFSET(NEXT_PTR(bp), READ_OFFSET(NEXT_PTR(ep)) + GET_OFFSET(ep, bp));
        }
    }
    else { 
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));

        /* Set the free list */
        delete_node(bp);
    }
    if (debug) {
        printf("after place:\n");
        mm_checkheap(1);
    }
}

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
static void *find_fit(size_t asize)
{
    /* First-fit search */
    void *bp;

    if (debug) {
        printf("-----asize: %zu. search begin-----\n", asize);
        mm_checkheap(1);
    }

    for (bp = root; GET_SIZE(HDRP(bp)) > 0 && bp != end_listp; bp = NEXT_FREE(bp)) {

        if (debug) {
            printf("bp:%p\n", bp);
            printf("block size:%d\n", GET_SIZE(HDRP(bp)));
        }

        if (asize <= GET_SIZE(HDRP(bp))) {
            if (debug) {
                printf("-----search end-----\n");
                mm_checkheap(1);
            }
            return bp;
        }
    }

    if (debug) {
        printf("-----search end-----\n");
    }

    return NULL; /* No fit */
}

/*
 * LIFO_insert - perform LIFO insert of a new free block 
 */
static void LIFO_insert(void *bp) {
    if (root == end_listp) {
        /* First block in the free list */

        /* set prevptr to none */
        PUT_OFFSET(PREV_PTR(bp), GET_OFFSET(end_listp, bp));
        /* set nextptr to root */
        PUT_OFFSET(NEXT_PTR(bp), GET_OFFSET(root, bp)); 
        /* set root to current block */
        root = bp;
    } else {
        /* set current block to be the first */

        void *pre_root = root;
        root = bp;
        /* set nextptr to preroot */
        PUT_OFFSET(NEXT_PTR(bp), GET_OFFSET(pre_root, root));
        /* set prevptr to none */
        PUT_OFFSET(PREV_PTR(bp), GET_OFFSET(end_listp, root));
        /* set prevptr of preroot to root block */
        PUT_OFFSET(PREV_PTR(pre_root), GET_OFFSET(root, pre_root));
    } 
}

/* 
 * delete_node - delete a free block in the current free list
 * either for place a allocated block or coalesce
 */
static void delete_node(void *bp) {
    if (root == bp && NEXT_FREE(bp) == end_listp) {
        /* the only block */
        root = end_listp;
    } else if (root == bp) {
        /* normal next free block but no prev one */
        root = NEXT_FREE(bp);
        PUT_OFFSET(PREV_PTR(root), GET_OFFSET(end_listp, root));
    } else if (NEXT_FREE(bp) == end_listp) { 
        /* normal prev one but no next one */
        PUT_OFFSET(NEXT_PTR(PREV_FREE(bp)), GET_OFFSET(end_listp, PREV_FREE(bp)));
    } else {
        /* next and prev free blocks both exist*/
        PUT_OFFSET(PREV_PTR(NEXT_FREE(bp)), GET_OFFSET(PREV_FREE(bp), NEXT_FREE(bp)));
        PUT_OFFSET(NEXT_PTR(PREV_FREE(bp)), GET_OFFSET(NEXT_FREE(bp), PREV_FREE(bp)));
    }
}