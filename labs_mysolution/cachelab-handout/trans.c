/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii, jj, t0, t1, t2, t3, t4, t5, t6, t7;

    if (M == 32) {
        //Read A into B to warm up B and then transpose B
        for (ii = 0; ii < M; ii += 8) {
            for (jj = 0; jj < N; jj += 8) {
                for (i = 0; i < 8; i++) {
                    t0 = A[jj + i][ii + 0];
                    t1 = A[jj + i][ii + 1];
                    t2 = A[jj + i][ii + 2];
                    t3 = A[jj + i][ii + 3];
                    t4 = A[jj + i][ii + 4];
                    t5 = A[jj + i][ii + 5];
                    t6 = A[jj + i][ii + 6];
                    t7 = A[jj + i][ii + 7];
                    B[ii + i][jj + 0] = t0;
                    B[ii + i][jj + 1] = t1;
                    B[ii + i][jj + 2] = t2;
                    B[ii + i][jj + 3] = t3;
                    B[ii + i][jj + 4] = t4;
                    B[ii + i][jj + 5] = t5;
                    B[ii + i][jj + 6] = t6;
                    B[ii + i][jj + 7] = t7;
                }
                for (i = 0; i < 8; ++i) {
                    for (j = i + 1; j < 8; ++j) {
                        t0 = B[ii + i][jj + j];
                        B[ii + i][jj + j] = B[ii + j][jj + i];
                        B[ii + j][jj + i] = t0;
                    }
                }
            }
        }

        return;

    }
    if (M == 64) {
        //A[i][0] 和 A[i + 4][0]就有conflict miss
        //改为4 * 4分块，得到1600+ misses
        //考虑 4 * 8 分块载入，再转置
        for (ii = 0; ii < M; ii += 8) {
            for (jj = 0; jj < N; jj += 8) {
                // 非对角块
                if (ii != jj) {
                    for (i = 0; i < 4; i++) {
                        t0 = A[jj + i][ii + 0];
                        t1 = A[jj + i][ii + 1];
                        t2 = A[jj + i][ii + 2];
                        t3 = A[jj + i][ii + 3];
                        t4 = A[jj + i][ii + 4];
                        t5 = A[jj + i][ii + 5];
                        t6 = A[jj + i][ii + 6];
                        t7 = A[jj + i][ii + 7];
                        B[ii + i][jj + 0] = t0;
                        B[ii + i][jj + 1] = t1;
                        B[ii + i][jj + 2] = t2;
                        B[ii + i][jj + 3] = t3;
                        B[ii + i][jj + 4] = t4;
                        B[ii + i][jj + 5] = t5;
                        B[ii + i][jj + 6] = t6;
                        B[ii + i][jj + 7] = t7;
                    }
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    }

                    jj += 4;
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    }
                    jj -= 4;

                    for (i = 0; i < 4; i++) {
                        t0 = A[jj + 4][ii + i];
                        t1 = A[jj + 5][ii + i];
                        t2 = A[jj + 6][ii + i];
                        t3 = A[jj + 7][ii + i];

                        t4 = B[ii + i][jj + 4];
                        t5 = B[ii + i][jj + 5];
                        t6 = B[ii + i][jj + 6];
                        t7 = B[ii + i][jj + 7];

                        B[ii + i][jj + 4] = t0;
                        B[ii + i][jj + 5] = t1;
                        B[ii + i][jj + 6] = t2;
                        B[ii + i][jj + 7] = t3;

                        B[ii + i + 4][jj + 0] = t4;
                        B[ii + i + 4][jj + 1] = t5;
                        B[ii + i + 4][jj + 2] = t6;
                        B[ii + i + 4][jj + 3] = t7;

                        t0 = A[jj + i + 4][ii + 4];
                        t1 = A[jj + i + 4][ii + 5];
                        t2 = A[jj + i + 4][ii + 6];
                        t3 = A[jj + i + 4][ii + 7];

                        B[ii + i + 4][jj + 4] = t0;
                        B[ii + i + 4][jj + 5] = t1;
                        B[ii + i + 4][jj + 6] = t2;
                        B[ii + i + 4][jj + 7] = t3;
                    }

                    ii += 4;
                    jj += 4;
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    }
                    jj -= 4;
                    ii -= 4;

                } else { // 对角块，不作特殊处理。
                    for (i = 0; i < 4; i++) {
                        t0 = A[jj + i][ii + 0];
                        t1 = A[jj + i][ii + 1];
                        t2 = A[jj + i][ii + 2];
                        t3 = A[jj + i][ii + 3];
                        B[ii + i][jj + 0] = t0;
                        B[ii + i][jj + 1] = t1;
                        B[ii + i][jj + 2] = t2;
                        B[ii + i][jj + 3] = t3;
                    }
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    } 

                    jj += 4;

                    for (i = 0; i < 4; i++) {
                        t0 = A[jj + i][ii + 0];
                        t1 = A[jj + i][ii + 1];
                        t2 = A[jj + i][ii + 2];
                        t3 = A[jj + i][ii + 3];
                        B[ii + i][jj + 0] = t0;
                        B[ii + i][jj + 1] = t1;
                        B[ii + i][jj + 2] = t2;
                        B[ii + i][jj + 3] = t3;
                    }
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    } 

                    ii += 4;

                    for (i = 0; i < 4; i++) {
                        t0 = A[jj + i][ii + 0];
                        t1 = A[jj + i][ii + 1];
                        t2 = A[jj + i][ii + 2];
                        t3 = A[jj + i][ii + 3];
                        B[ii + i][jj + 0] = t0;
                        B[ii + i][jj + 1] = t1;
                        B[ii + i][jj + 2] = t2;
                        B[ii + i][jj + 3] = t3;
                    }
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    } 

                    jj -= 4;

                    for (i = 0; i < 4; i++) {
                        t0 = A[jj + i][ii + 0];
                        t1 = A[jj + i][ii + 1];
                        t2 = A[jj + i][ii + 2];
                        t3 = A[jj + i][ii + 3];
                        B[ii + i][jj + 0] = t0;
                        B[ii + i][jj + 1] = t1;
                        B[ii + i][jj + 2] = t2;
                        B[ii + i][jj + 3] = t3;
                    }
                    for (i = 0; i < 4; ++i) {
                        for (j = i + 1; j < 4; ++j) {
                            t0 = B[ii + i][jj + j];
                            B[ii + i][jj + j] = B[ii + j][jj + i];
                            B[ii + j][jj + i] = t0;
                        }
                    } 

                    ii -= 4;

                }
            }
        } 

        return;
         
    } else {

               //Read A into B to warm up B and then transpose B
        for (ii = 0; ii < 55; ii += 8) {
            for (jj = 0; jj < 63; jj += 8) {
                for (i = 0; i < 8; i++) {
                    t0 = A[jj + i][ii + 0];
                    t1 = A[jj + i][ii + 1];
                    t2 = A[jj + i][ii + 2];
                    t3 = A[jj + i][ii + 3];
                    t4 = A[jj + i][ii + 4];
                    t5 = A[jj + i][ii + 5];
                    t6 = A[jj + i][ii + 6];
                    t7 = A[jj + i][ii + 7];
                    B[ii + 0][jj + i] = t0;
                    B[ii + 1][jj + i] = t1;
                    B[ii + 2][jj + i] = t2;
                    B[ii + 3][jj + i] = t3;
                    B[ii + 4][jj + i] = t4;
                    B[ii + 5][jj + i] = t5;
                    B[ii + 6][jj + i] = t6;
                    B[ii + 7][jj + i] = t7;
                }
            }
        }

        ii = 56;
        for (jj = 0; jj < 65; jj += 5) {
            for (i = 0; i < 5; i++) {
                t0 = A[jj + i][ii + 0];
                t1 = A[jj + i][ii + 1];
                t2 = A[jj + i][ii + 2];
                t3 = A[jj + i][ii + 3];
                t4 = A[jj + i][ii + 4];

                B[ii + i][jj + 0] = t0;
                B[ii + i][jj + 1] = t1;
                B[ii + i][jj + 2] = t2;
                B[ii + i][jj + 3] = t3;
                B[ii + i][jj + 4] = t4;
            }
            for (i = 0; i < 5; ++i) {
                for (j = i + 1; j < 5; ++j) {
                    t0 = B[ii + i][jj + j];
                    B[ii + i][jj + j] = B[ii + j][jj + i];
                    B[ii + j][jj + i] = t0;
                }
            }
        }

        for (i = 0; i < 56; i += 3) {
            for (j = 64; j < N; j++) {
                t0 = A[j][i + 0];
                t1 = A[j][i + 1];
                t2 = A[j][i + 2];

                B[i + 0][j] = t0;
                B[i + 1][j] = t1;
                B[i + 2][j] = t2;
            }
        }

        for (i = 54; i < 56; i++) {
            for (j = 64; j < N; j++) {
                t0 = A[j][i];
                B[i][j] = t0;
            }
        }

        for (i = 56; i < M; i++) {
            for (j = 65; j < N; j++) {
                t0 = A[j][i];
                B[i][j] = t0;
            }
        }

        return; 

    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char trans_32ver1_desc[] = "343 with (M, N) = (32, 32)";
void trans_32ver1(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii, jj, tmp; 

    if (M == 32) {
        for (ii = 0; ii < M; ii += 8) {
            for (jj = 0; jj < N; jj += 8) {
                for (i = ii; i < ii + 8; i++) {
                    for (j = jj; j < jj + 8; j++) {
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(trans_32ver1, trans_32ver1_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

