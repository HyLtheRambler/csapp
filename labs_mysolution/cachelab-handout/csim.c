#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void printUsage(const char *prog_name) {
    printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", prog_name);
    printf("Options:\n");
    printf("  -h         Print this help message.\n  -v         Optional verbose flag.\n  -s <num>   Number of set index bits.\n  -E <num>   Number of lines per set.\n  -b <num>   Number of block offset bits.\n  -t <file>  Trace file.  \n");
    printf("\n");
    printf("Examples:\n  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    return;
}

void printSummary(int hits,  /* number of  hits */
				  int misses, /* number of misses */
				  int evictions); /* number of evictions */

struct line {
    int valid; // 有效位
    int tag; // 标记
    int last_used_time; // 最后使用时间
}; // 字节信息是没用的，不用存
typedef struct line* set;
set* cache;

int main(int argc, char *argv[])
{
    int opt, v, s, E, b;
    FILE *trace_file = NULL;
    char operation; long unsigned int address; int size;
    int hit_count = 0, miss_count = 0, eviction_count = 0;

    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
        case 'h':
            printUsage("./csim");
            exit(0);
        case 'v':
            v = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = fopen(optarg, "r");
            break;
        default:
            printUsage("./csim");
            exit(0);
        }
    }

    cache = (set*)malloc(sizeof(set) * (1 << s));
    for (int i = 0; i < (1 << s); i++) {
        cache[i] = (set)malloc(sizeof(struct line) * E);
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = -1;
            cache[i][j].tag = -1;
            cache[i][j].last_used_time = -1;
        }
    }
    
    /* my work starts here */
    //构造掩码
    long tag_mask = 0xffffffffffffffff << (s + b);
    long unsigned int byte_offset_mask = (~((long unsigned int)1 << 63)) >> (63 - b);
    long unsigned int set_index_mask = (~tag_mask) & (~byte_offset_mask);
    int current_tag = 0, current_set_index = 0;
    int hit_flag, miss_flag, eviction_flag;
    int least_used_index = -1;
    int time = -1;

    while(fscanf(trace_file, "%s %lx,%d\n", &operation, &address, &size) == 3) {
        time++;
        if (operation == 'I') {
            continue;
        }
        hit_flag = 0;
        miss_flag = 0;
        eviction_flag = 0;
        current_tag = (tag_mask & address) >> (s + b);
        current_set_index = (set_index_mask & address) >> b;
        least_used_index = -1;

        for (int i = 0; i < E; i++) {
            if (cache[current_set_index][i].tag == current_tag && cache[current_set_index][i].valid == 1) {
                cache[current_set_index][i].last_used_time = time;
                hit_flag = 1;
                break;
            }
        }

        if (hit_flag == 0) {
            miss_flag++;
            if (operation == 'M') {
                hit_flag++;
            }
        } else if (operation == 'M') {
            hit_flag++;
        }

        if (miss_flag == 1) {
            for (int i = 0; i < E; i++) {
                if (cache[current_set_index][i].valid == -1) {
                    cache[current_set_index][i].valid = 1;
                    cache[current_set_index][i].tag = current_tag;
                    cache[current_set_index][i].last_used_time = time;
                    least_used_index = -1;
                    break;
                } else if (least_used_index == -1) {
                    least_used_index = i;
                } else if (cache[current_set_index][least_used_index].last_used_time >= cache[current_set_index][i].last_used_time) {
                    least_used_index = i;
                }
            }
        }
        if (least_used_index != -1) {
            eviction_flag++;
            cache[current_set_index][least_used_index].tag = current_tag;
            cache[current_set_index][least_used_index].last_used_time = time;
        }

        hit_count += hit_flag;
        miss_count += miss_flag;
        eviction_count += eviction_flag;

        if (v) {
            printf ("   --------------- #tag: %d, set: %d #", current_tag, current_set_index);
            printf ("\n%c /", operation);
            if (miss_flag) {
                printf("miss ");
            }
            if (eviction_flag) {
                printf("eviction");
            }
            while (hit_flag) {
                printf("hit ");
                hit_flag--;
            }
            printf("\n");
        }
    }

    /* my work ends here */

    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
