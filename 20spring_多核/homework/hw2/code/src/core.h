/*###################################################
##  文件说明：
##  CUDA相关函数，kernal函数，callback函数的声明
#####################################################*/

#ifndef _INCL_CORE
#define _INCL_CORE

#include <stdio.h>

#define CALLBACK1 cudaCallbackCPU
#define CALLBACK2 cudaCallback1
#define CALLBACK3 cudaCallback2
#define CALLBACK4 cudaCallback3
#define CALLBACK5 cudaCallback4
#define CALLBACK6 cudaCallback5

// The main function would invoke the "cudaCallback"s on each sample. Note that
// you don't have to (and shouldn't) free the space of searchPoints,
// referencePoints, and result by yourself since the main function have included
// the free statements already.
//
// To make the program work, you shouldn't modify the signature of\
// "cudaCallback"s.
extern void cudaCallbackCPU(int k, int m, int n, float *searchPoints,
                            float *referencePoints, int **results);
extern void cudaCallback(int k, int m, int n, float *searchPoints,
                            float *referencePoints, int **results,
                            int dim1, int dim2, bool use_share);
extern void cudaCallback1(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results);
extern void cudaCallback2(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results);
extern void cudaCallback3(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results);
extern void cudaCallback4(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results);
extern void cudaCallback5(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results);

// divup calculates n / m and would round it up if the remainder is non-zero.
extern int divup(int n, int m);

// kernel function with all optimization
extern __global__ void kernal(int n, int m, int D, int PartNum, float *qry_d, float *refer_d, int *output_d);

// kernel function with all optimization ecxept memory optimization
extern __global__ void kernal_ns(int n, int m, int D, int PartNum, float *qry_d, float *refer_d, int *output_d);

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