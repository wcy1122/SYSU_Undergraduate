/*###################################################
##  文件说明：
##  CUDA相关函数，kernal函数，callback函数的声明
#####################################################*/

#ifndef _INCL_CORE
#define _INCL_CORE

#include <stdio.h>

// kernel is an element-wise kernel function for demonstrating purpose only.
// threr are sevel kernals
extern __global__ void kernel0(int width, int height, float *input, float *output);
extern __global__ void kernel1(int width, int height, float *input, float *output);
extern __global__ void kernel2(int width, int height, float *input, float *output);
extern __global__ void kernel3(int width, int height, float *input, float *output, int ThreadNum);
extern __global__ void kernel4(int width, int height);
/*
 * In general, you don't need to modify the lines below to finish hw1.
 */

// The main function would invoke the cudaCallback on each sample. Note that you
// don't have to (and shouldn't) free the space of sample and result by yourself
// since the main function have included the free statements already.
//
// To make the program work, you shouldn't modify the signature of cudaCallback.
extern void cudaCallback(int width, int height, int Flag, float *sample, float **result);

// precompute log
extern void prepare_log();

// divup calculates n / m and would round it up if the remainder is non-zero.
extern int divup(int n, int m);

// CHECK macro from Grossman and McKercher, "Professional CUDA C Programming"
#define CHECK(call)                                         \
{                                                           \
    const cudaError_t error = call;                         \
    if (error != cudaSuccess) {                             \
        printf("Error: %s:%d, ", __FILE__, __LINE__);       \
        printf("code:%d, reason: %s \n",                    \
                error, cudaGetErrorString(error));          \
        exit(1);                                            \
    }                                                       \
}                                                           \

#endif