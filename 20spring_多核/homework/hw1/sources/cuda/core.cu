/*###################################################
##  文件说明：
##  CUDA相关函数，kernal函数，callback函数的实现
#####################################################*/

#include "core.h"
#define N 7000010
__device__ float input_g[N], output_g[N], logs[26];

/*#####################################
## kernel0
## baseline方法的核函数
## params:
## width, height: 矩阵长宽
## input: 输入数据
## output: 运算结果
#####################################*/
__global__ void kernel0(int width, int height, float *input, float *output) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int x = idx / height, y = idx % height;
    int L = max(0, x-2), R = min(x+3, width);
    int D = max(0, y-2), U = min(y+3, height);
    int all_idxs = (R-L) * (U-D);
    float ans = 0, prob;
    
    int Cnt;
    for(int k=0;k<16;k++) {
        Cnt = 0;
        for(int i=L;i<R;i++)
            for(int j=D;j<U;j++) {
                if(input[i * height + j]==k) Cnt++;
            }
        if(Cnt) {
            prob = 1.0f * Cnt / all_idxs;
            ans -= prob * logf(prob);
        }
    }
    output[idx] = ans;
}

/*#####################################
## kernel1
## MemTime方法的核函数
## params:
## width, height: 矩阵长宽
## input: 输入数据
## output: 运算结果
#####################################*/
__global__ void kernel1(int width, int height, float *input, float *output) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int x = idx / height, y = idx % height;
    int L = max(0, x-2), R = min(x+3, width);
    int D = max(0, y-2), U = min(y+3, height);
    int all_idxs = (R-L) * (U-D);
    float ans = 0, prob;
    
    char Cnt[16] = {0};
    for(int i=L;i<R;i++)
        for(int j=D;j<U;j++) {
            Cnt[(int)input[i * height + j]]++;
        }
    for(int i=0;i<16;i++)
        if(Cnt[i]) {
            prob = 1.0f * Cnt[i] / all_idxs;
            ans -= prob * logf(prob);
        }
    output[idx] = ans;
}

/*#####################################
## kernel2
## PreLog方法的核函数
## params:
## width, height: 矩阵长宽
## input: 输入数据
## output: 运算结果
#####################################*/
__global__ void kernel2(int width, int height, float *input, float *output) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int x = idx / height, y = idx % height;
    int L = max(0, x-2), R = min(x+3, width);
    int D = max(0, y-2), U = min(y+3, height);
    int all_idxs = (R-L) * (U-D);
    float ans = 0, prob;
    
    char Cnt[16] = {0};
    for(int i=L;i<R;i++)
        for(int j=D;j<U;j++) {
            Cnt[(int)input[i * height + j]]++;
        }
    float log_base = logs[all_idxs];
    for(int i=0;i<16;i++)
        if(Cnt[i]) {
            prob = 1.0f * Cnt[i] / all_idxs;
            ans -= prob * (logs[Cnt[i]] - log_base);
        }
    output[idx] = ans;
}

/*#####################################
## kernel3
## ShareMemo(使用共享内存)方法的核函数
## params:
## width, height: 矩阵长宽
## input: 输入数据
## output: 运算结果
## ThreadNum: 线程数
#####################################*/
__global__ void kernel3(int width, int height, float *input, float *output, int ThreadNum) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    // int p = threadDim.x;
    int x = idx / height, y = idx % height;
    int L = max(0, x-2), R = min(x+3, width);
    int D = max(0, y-2), U = min(y+3, height);
    int all_idxs = (R-L) * (U-D);
    float ans = 0, prob;
    
    extern __shared__ float Cur[6500];
    int sid = threadIdx.x;
    
    char Cnt[16] = {0};
    for(int i=L;i<R;i++){
        Cur[sid] = input[i * height + y];
        __syncthreads();
        if(y<2 || y>=height-2 || sid<2 || sid+2>=ThreadNum) {
            for(int j=D;j<U;j++) {
                Cnt[(int)input[i * height + j]]++;
            }
        }
        else {
            for(int j=-2;j<=2;j++) {
                Cnt[(int)Cur[sid + j]]++;
            }
        }
    }
    float log_base = logs[all_idxs];
    for(int i=0;i<16;i++)
        if(Cnt[i]) {
            prob = 1.0f * Cnt[i] / all_idxs;
            ans -= prob * (logs[Cnt[i]] - log_base);
        }
    output[idx] = ans;
}

/*#####################################
## kernel4
## StaticMemo(使用全局内存)方法的核函数
## params:
## width, height: 矩阵长宽
#####################################*/
__global__ void kernel4(int width, int height) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int x = idx / height, y = idx % height;
    int L = max(0, x-2), R = min(x+3, width);
    int D = max(0, y-2), U = min(y+3, height);
    int all_idxs = (R-L) * (U-D);
    float ans = 0, prob;
    
    char Cnt[16] = {0};
    for(int i=L;i<R;i++)
        for(int j=D;j<U;j++) {
            Cnt[(int)input_g[i * height + j]]++;
        }
    float log_base = logs[all_idxs];
    for(int i=0;i<16;i++)
        if(Cnt[i]) {
            prob = 1.0f * Cnt[i] / all_idxs;
            ans -= prob * (logs[Cnt[i]] - log_base);
        }
    output_g[idx] = ans;
}

/*#####################################
## prepare_log
## 预处理log
#####################################*/
void prepare_log() {
    float _logs[26];
    for(int i=1;i<=25;i++) _logs[i] = logf(i);
    CHECK(cudaMemcpyToSymbol(logs, _logs, sizeof(float)*26));
}

/*#####################################
## cudaCallback
## CUDA的callback函数，负责数据传输，kernalxuanze
## params:
## width, height: 矩阵长宽
## Flag：选择的方法
## sample: 输入数据
## result: 运算结果
#####################################*/
void cudaCallback(int width, int height, int Flag, float *sample, float **result) {
    int size = width * height;
    float *input_d, *output_d;
    
    if(Flag<4) {
        CHECK(cudaMalloc((void **)&input_d, sizeof(float)*size));
        CHECK(cudaMalloc((void **)&output_d, sizeof(float)*size));
        CHECK(cudaMemcpy(input_d, sample, sizeof(float)*size, cudaMemcpyHostToDevice));
    }
    else {
        CHECK(cudaMemcpyToSymbol(input_g, sample, sizeof(float)*size));
    }
    // Allocate device memory and copy data from host to device
    
    // Invoke the device function
    int ThreadNum = divup(size, 1024);
    if(Flag==0){
        kernel0<<< ThreadNum, 1024 >>>(width, height, input_d, output_d);
    } else if(Flag==1) {
        kernel1<<< ThreadNum, 1024 >>>(width, height, input_d, output_d);
    } else if(Flag==2) {
        kernel2<<< ThreadNum, 1024 >>>(width, height, input_d, output_d);
    } else if(Flag==3) {
        kernel3<<< ThreadNum, 1024 >>>(width, height, input_d, output_d, ThreadNum);
    } else {
        kernel4<<< ThreadNum, 1024 >>>(width, height);
    }
    cudaDeviceSynchronize();

    // Copy back the results and de-allocate the device memory
    *result = (float *)malloc(sizeof(float)*size);
    if(Flag<4) {
        CHECK(cudaMemcpy(*result, output_d, sizeof(float)*size, cudaMemcpyDeviceToHost));
        CHECK(cudaFree(input_d));
        CHECK(cudaFree(output_d));
    } else {
        CHECK(cudaMemcpyFromSymbol(*result, output_g, sizeof(float)*size));
    }

    // Note that you don't have to free sample and *result by yourself
}
