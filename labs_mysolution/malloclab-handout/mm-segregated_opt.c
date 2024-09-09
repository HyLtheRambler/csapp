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
#define GET_PREV_ALLOC(p) ((GET(p) & 0x2))

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
static char **root = 0;        /* pointer to root of the free list to operate */
static char *end_listp = 0;   /* End of the free list */

static char *root_0 = 0;
static char *root_1 = 0;
static char *root_2 = 0;
static char *root_3 = 0;
static char *root_4 = 0;
static char *root_5 = 0;
static char *root_6 = 0;
static char *root_7 = 0;
static char *root_8 = 0;
static char *root_9 = 0;
static char *root_10 = 0;
static char *root_11 = 0;
static char *root_12 = 0;
static char *root_13 = 0;
static char *root_14 = 0;

/* print debug message */
int debug = 0;

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void LIFO_insert(void *bp);
static void delete_node(void *bp);
static void select_root(size_t asize);
static void root_incr();

/* 
 * mm_init - Initialize the memory manager 
 */
int mm_init(void) 
{
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(2*WSIZE)) == (void *)-1) 
        return -1;
    PUT(heap_listp, PACK(0, 3)); /* Prologue header */ 
    PUT((heap_listp + WSIZE), PACK(0, 3));     /* Epilogue header */
    heap_listp += (WSIZE);                     
    root = &heap_listp;                          /* Set root of free list*/
    end_listp = heap_listp;                      /* Set end of free list */
    root_0 = heap_listp;
    root_1 = heap_listp;
    root_2 = heap_listp;
    root_3 = heap_listp;
    root_4 = heap_listp;
    root_5 = heap_listp;
    root_6 = heap_listp;
    root_7 = heap_listp;
    root_8 = heap_listp;
    root_9 = heap_listp;
    root_10 = heap_listp;
    root_11 = heap_listp;
    root_12 = heap_listp;
    root_13 = heap_listp;
    root_14 = heap_listp;

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
    if (size <= (3*WSIZE))                                          
        asize = 2*DSIZE;                                        
    else
        asize = DSIZE * ((size + (WSIZE) + (DSIZE-1)) / DSIZE); 

    if (debug) {
        printf("\n      Before find_fit\n");
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
        printf("\n          freeing pointer %p, block size: %d\n", bp, GET_SIZE(HDRP(bp)));
    }
    if (bp == 0) 
        return;

    size_t size = GET_SIZE(HDRP(bp));
    if (heap_listp == 0){
        mm_init();
    }

    PUT(HDRP(bp), PACK(size, (GET_PREV_ALLOC(HDRP(bp)))));
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
 * calloc: 分配并初始化块（初始化为 0）
 */
void* calloc(size_t elem_num, size_t size) {
    size_t total = elem_num * size;
    void* ptr = malloc(total);
    memset(ptr, 0, total);
    return ptr;
}

/* 
 * mm_checkheap - Check the heap for correctness. Helpful hint: You
 *                can call this function using mm_checkheap(__LINE__);
 *                to identify the line number of the call site.
 */
void mm_checkheap(int lineno)  
{ 
    printf("\n*** checkheap begin ***\n");
    
    void *bp = *root;
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
static inline void *extend_heap(size_t words) 
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; 
    if ((long)(bp = mem_sbrk(size)) == -1)  
        return NULL;                                        

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, (GET_PREV_ALLOC(HDRP(bp)))));         /* Free block header */   
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   

    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ 

    /* Coalesce if the previous block was free */
    if (debug) {
        printf("                        after extend_heap:\n");
        select_root(size);
        mm_checkheap(1);
    }
    return coalesce(bp);                                          
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static inline void *coalesce(void *bp) 
{
    if (debug) {
        printf("\n              Before coalesce\n");
    }

    size_t prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1 */
        /* set the free list */
        select_root(size);
        LIFO_insert(bp);
        return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));

        select_root(GET_SIZE(HDRP(NEXT_BLKP(bp))));
        delete_node(NEXT_BLKP(bp));

        PUT(HDRP(bp), PACK(size, 2));
        PUT(FTRP(bp), PACK(size, 0));

        select_root(size);
        LIFO_insert(bp);
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));

        select_root(GET_SIZE(HDRP(PREV_BLKP(bp))));
        delete_node(PREV_BLKP(bp));

        PUT(FTRP(bp), PACK(size, 0));
        bp = PREV_BLKP(bp);
        PUT(HDRP(bp), PACK(size, 0 | GET_PREV_ALLOC(HDRP(bp))));

        select_root(size);
        LIFO_insert(bp);
    }

    else {                                     /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
            GET_SIZE(FTRP(NEXT_BLKP(bp)));

        select_root(GET_SIZE(HDRP(PREV_BLKP(bp))));
        delete_node(PREV_BLKP(bp));

        select_root(GET_SIZE(FTRP(NEXT_BLKP(bp))));
        delete_node(NEXT_BLKP(bp));

        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0 | GET_PREV_ALLOC(HDRP(PREV_BLKP(bp)))));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);

        select_root(size);
        LIFO_insert(bp);
    }

    if (debug) {
        printf("\n              After coalesce:\n");
        mm_checkheap(1);
    }
    return bp;
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static inline void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));   

    if ((csize - asize) >= (2*DSIZE)) { 

        select_root(csize);
        delete_node(bp);

        PUT(HDRP(bp), PACK(asize, 1 | GET_PREV_ALLOC(HDRP(bp))));

        if (debug) {
            printf("                        Place at bp:%p, block size:%d\n", PREV_BLKP(bp), GET_SIZE(HDRP(PREV_BLKP(bp))));
        }

        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 2));
        PUT(FTRP(bp), PACK(csize-asize, 0));

        select_root(csize - asize);
        LIFO_insert(bp);

        if (debug) {
            printf("heap after place:\n");
            mm_checkheap(1);
        }
    }
    else { 
        PUT(HDRP(bp), PACK(csize, 1 | GET_PREV_ALLOC(HDRP(bp))));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(GET_SIZE(HDRP(NEXT_BLKP(bp))), 2 | GET_ALLOC(HDRP(NEXT_BLKP(bp)))));

        /* Set the free list */
        select_root(csize);
        delete_node(bp);

        if (debug) {
            printf("                        Place at bp:%p, block size:%d\n", bp, GET_SIZE(HDRP((bp))));
            printf("heap after place:\n");
            mm_checkheap(1);
        }
    }

}

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
static inline void *find_fit(size_t asize)
{
    /* First-fit search */
    void *bp;

    if (debug) {
        printf("\n                      asize: %zu. search begin\n", asize);
        mm_checkheap(1);
    }

    select_root(asize);   
    for (; root != NULL; root_incr()) {
        if (debug) {
            mm_checkheap(1);
        }
        for (bp = *root; GET_SIZE(HDRP(bp)) > 0 && bp != end_listp; bp = NEXT_FREE(bp)) {

            if (asize <= GET_SIZE(HDRP(bp))) {
                if (debug) {
                    printf("\n            FOUND: ");
                    printf("bp:%p  ", bp);
                    printf("block size:%d\n", GET_SIZE(HDRP(bp)));
                    printf("-----search end-----\n");
                    mm_checkheap(1);
                }
                return bp;
            }
        }
    }

    if (debug) {
        printf("\n                  Not found \n");
    }

    return NULL; /* No fit */
}

/*
 * LIFO_insert - perform LIFO insert of a new free block 
 */
static inline void LIFO_insert(void *bp) {
    if (*root == end_listp) {
        /* First block in the free list */

        /* set prevptr to none */
        PUT_OFFSET(PREV_PTR(bp), GET_OFFSET(end_listp, bp));
        /* set nextptr to root */
        PUT_OFFSET(NEXT_PTR(bp), GET_OFFSET(*root, bp)); 
        /* set root to current block */
        *root = bp;
    } else {
        /* set current block to be the first */

        void *pre_root = *root;
        *root = bp;
        /* set nextptr to preroot */
        PUT_OFFSET(NEXT_PTR(bp), GET_OFFSET(pre_root, *root));
        /* set prevptr to none */
        PUT_OFFSET(PREV_PTR(bp), GET_OFFSET(end_listp, *root));
        /* set prevptr of preroot to root block */
        PUT_OFFSET(PREV_PTR(pre_root), GET_OFFSET(*root, pre_root));
    } 
}

/* 
 * delete_node - delete a free block in the current free list
 * either for place a allocated block or coalesce
 */
static inline void delete_node(void *bp) {
    if (*root == bp && NEXT_FREE(bp) == end_listp) {
        /* the only block */
        *root = end_listp;
    } else if (*root == bp) {
        /* normal next free block but no prev one */
        *root = NEXT_FREE(bp);
        PUT_OFFSET(PREV_PTR(*root), GET_OFFSET(end_listp, *root));
    } else if (NEXT_FREE(bp) == end_listp) { 
        /* normal prev one but no next one */
        PUT_OFFSET(NEXT_PTR(PREV_FREE(bp)), GET_OFFSET(end_listp, PREV_FREE(bp)));
    } else {
        /* next and prev free blocks both exist*/
        PUT_OFFSET(PREV_PTR(NEXT_FREE(bp)), GET_OFFSET(PREV_FREE(bp), NEXT_FREE(bp)));
        PUT_OFFSET(NEXT_PTR(PREV_FREE(bp)), GET_OFFSET(NEXT_FREE(bp), PREV_FREE(bp)));
    }
}

/*
 * select_root - set global variable root to
 * the segregated free list to operate
 */
static inline void select_root(size_t asize) {
    if (asize <= 24) {
        root = &root_0;
    } else if (asize <= 32) {
        root = &root_1;
    } else if (asize <= 64) {
        root = &root_2;
    } else if (asize <= 80) {
        root = &root_3;
    } else if (asize <= 120) {
        root = &root_4;
    } else if (asize <= 240) {
        root = &root_5;
    } else if (asize <= 480) {
        root = &root_6;
    } else if (asize <= 960) {
        root = &root_7;
    } else if (asize <= 1920) {
        root = &root_8;
    } else if (asize <= 3840) {
        root = &root_9;
    } else if (asize <= 7680) {
        root = &root_10;
    } else if (asize <= 15360) {
        root = &root_11;
    } else if (asize <= 30720) {
        root = &root_12;
    } else if (asize <= 61440) {
        root = &root_13;
    } else {
        root = &root_14;
    }
}

/*
 * root_incr() - increment the root to the next size class
 */
static inline void root_incr() {
    if (root == &root_0) {
        root = &root_1;
    } else if (root == &root_1) {
        root = &root_2;
    } else if (root == &root_2) {
        root = &root_3;
    } else if (root == &root_3) {
        root = &root_4;
    } else if (root == &root_4) {
        root = &root_5;
    } else if (root == &root_5) {
        root = &root_6;
    } else if (root == &root_6) {
        root = &root_7;
    } else if (root == &root_7) {
        root = &root_8;
    } else if (root == &root_8) {
        root = &root_9;
    } else if (root == &root_9) {
        root = &root_10;
    } else if (root == &root_10) {
        root = &root_11;
    } else if (root == &root_11) {
        root = &root_12;
    } else if (root == &root_12) {
        root = &root_13;
    } else if (root == &root_13) {
        root = &root_14;
    } else if (root == &root_14) {
        root = NULL;
    }
    return;
}