/*###################################################
##  文件说明：
##  CUDA相关函数，kernal函数，callback函数的实现
#####################################################*/

#include "core.h"

/*#####################################
## info类
## 记录当前最短距离信息
## dis：最短距离
## pos：最短距离对应点的编号
#####################################*/
struct info{
    float dis;
    int pos; 
};

/*#####################################
## kernal
## CUDA的kernal函数的实现，实现了最终版
## 的kernal，支持NMK三个维度的优化，同时
## 也添加了共享内存优化。
## params:
## n: 参考坐标数
## m: 询问坐标数
## D: 空间维度
## PartNum: m维的分割数
## DimNum: D维的分割数
## qry_d: 询问坐标
## refer_d: 参考坐标
## output_d: 输出结果
#####################################*/
__global__ void kernal(int n, int m, int D, int PartNum, int DimNum, float *qry_d, float *refer_d, int *output_d) {
    int idx = blockIdx.x;
    int ThreadId = threadIdx.x;
    int part = ThreadId / DimNum;
    int dim = ThreadId % DimNum;
    int BlkSize = (n+PartNum-1)/PartNum;
    int DimSize = (D+DimNum-1)/DimNum;
    int L = BlkSize*part, R = min(L+BlkSize, n);
    int _l = DimSize*dim, _r = min(_l+DimSize, D);
    
    extern __shared__ info Str[];
    info *Info = Str;
    float *sum = (float*)&Info[PartNum];
    float *Cur = (float*)&sum[PartNum*DimNum];
    
    if(part==0){
        for(int i=_l;i<_r;i++) Cur[i] = qry_d[idx*D+i];
    }
    __syncthreads();
    
    float ans = -1;
    int pos = -1;
    for(int i=L;i<R;i++)
    {
        float dis = 0, diff;
        for(int j=_l;j<_r;j++) {
            diff = Cur[j] - refer_d[i*D+j];
            dis += diff * diff;
        }
        
        sum[ThreadId] = dis;
        __syncthreads();
        
        if(!dim){
            for(int j=1;j<DimNum;j++) dis += sum[ThreadId+j];
            if(ans<0 || dis<ans) ans=dis, pos=i;
        }
    }
    if(!dim) Info[part] = (info){ans, pos};
    __syncthreads();
    
    if(!dim) {
        for(int stride=(PartNum>>1);stride>0;stride>>=1) {
            if(part+stride<PartNum){
                if(Info[part].dis > Info[part+stride].dis)
                    Info[part] = Info[part+stride];
            }
            __syncthreads();
        }
    }
    
    if(!ThreadId) output_d[idx] = Info[0].pos;
}

/*#####################################
## kernal_ns
## CUDA的kernal函数的实现，实现了最终版
## 的kernal，支持NMK三个维度的优化，不
## 添加了共享内存优化。
## params:
## n: 参考坐标数
## m: 询问坐标数
## D: 空间维度
## PartNum: m维的分割数
## DimNum: D维的分割数
## qry_d: 询问坐标
## refer_d: 参考坐标
## output_d: 输出结果
#####################################*/
__global__ void kernal_ns(int n, int m, int D, int PartNum, int DimNum, float *qry_d, float *refer_d, int *output_d) {
    int idx = blockIdx.x;
    int ThreadId = threadIdx.x;
    int part = ThreadId / DimNum;
    int dim = ThreadId % DimNum;
    int BlkSize = (n+PartNum-1)/PartNum;
    int DimSize = (D+DimNum-1)/DimNum;
    int L = BlkSize*part, R = min(L+BlkSize, n);
    int _l = DimSize*dim, _r = min(_l+DimSize, D);
    
    extern __shared__ info Str[];
    info *Info = Str;
    float *sum = (float*)&Info[PartNum];
    
    float ans = -1;
    int pos = -1;
    for(int i=L;i<R;i++)
    {
        float dis = 0, diff;
        for(int j=_l;j<_r;j++) {
            diff = qry_d[idx*D+j] - refer_d[i*D+j];
            dis += diff * diff;
        }
        
        sum[ThreadId] = dis;
        __syncthreads();
        
        if(!dim){
            for(int j=1;j<DimNum;j++) dis += sum[ThreadId+j];
            if(ans<0 || dis<ans) ans=dis, pos=i;
        }
    }
    if(!dim) Info[part] = (info){ans, pos};
    __syncthreads();
    
    if(!dim) {
        for(int stride=(PartNum>>1);stride>0;stride>>=1) {
            if(part+stride<PartNum){
                if(Info[part].dis > Info[part+stride].dis)
                    Info[part] = Info[part+stride];
            }
            __syncthreads();
        }
    }
    
    if(!ThreadId) output_d[idx] = Info[0].pos;
}

/*#####################################
## cudaCallbackCPU
## 串行方法，作为对照，参考样例代码
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
#####################################*/
extern void cudaCallbackCPU(int k, int m, int n, float *searchPoints,
                            float *referencePoints, int **results) {

    int *tmp = (int*)malloc(sizeof(int)*m);
    int minIndex;
    float minSquareSum, diff, squareSum;

    // Iterate over all search points
    for (int mInd = 0; mInd < m; mInd++) {
        minSquareSum = -1;
        // Iterate over all reference points
        for (int nInd = 0; nInd < n; nInd++) {
            squareSum = 0;
            for (int kInd = 0; kInd < k; kInd++) {
                diff = searchPoints[k*mInd+kInd] - referencePoints[k*nInd+kInd];
                squareSum += (diff * diff);
            }
            if (minSquareSum < 0 || squareSum < minSquareSum) {
                minSquareSum = squareSum;
                minIndex = nInd;
            }
        }
        tmp[mInd] = minIndex;
    }

    *results = tmp;
    // Note that you don't have to free searchPoints, referencePoints, and
    // *results by yourself
}

/*#####################################
## cudaCallback
## GPU版本callback函数，内存拷贝，调用内核
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
## dim1: M维优化线程数
## dim2: K维优化线程数
## use_share：是否使用内存优化，默认使用
#####################################*/
extern void cudaCallback(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results,
                         int dim1, int dim2, bool use_share = true) {
    float *qry_d, *refer_d;
    int *output_d;
    int size_m = m*k;
    int size_n = n*k;
    
    CHECK(cudaMalloc((void **)&qry_d, sizeof(float)*size_m));
    CHECK(cudaMalloc((void **)&refer_d, sizeof(float)*size_n));
    CHECK(cudaMalloc((void **)&output_d, sizeof(int)*m));
    CHECK(cudaMemcpy(qry_d, searchPoints, sizeof(float)*size_m, cudaMemcpyHostToDevice));
    CHECK(cudaMemcpy(refer_d, referencePoints, sizeof(float)*size_n, cudaMemcpyHostToDevice));
    
    int BlkNum = m;
    int PartNum = min(dim1, n);
    int DimNum = min(dim2, (k+1)/2);
    int ThreadNum = PartNum * DimNum;
    int SizeInfo = sizeof(info) * PartNum;
    int SizeSum = sizeof(float) * (PartNum * DimNum);
    int SizeCur = sizeof(float) * k;
    if(use_share) {
        kernal<<< BlkNum, ThreadNum, SizeInfo + SizeSum + SizeCur >>>(n, m, k, PartNum, DimNum, qry_d, refer_d, output_d);
    } else {
        kernal_ns<<< BlkNum, ThreadNum, SizeInfo + SizeSum >>>(n, m, k, PartNum, DimNum, qry_d, refer_d, output_d);
    }
    
    *results = (int *)malloc(sizeof(int)*m);
    CHECK(cudaMemcpy(*results, output_d, sizeof(int)*m, cudaMemcpyDeviceToHost));
    CHECK(cudaFree(qry_d));
    CHECK(cudaFree(refer_d));
    
    // Note that you don't have to free searchPoints, referencePoints, and
    // *results by yourself
}


/*##########################################
## cudaCallback1
## baseline策略, dim1=1, dim2=1, 不使用内存优化
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
##########################################*/
extern void cudaCallback1(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results) {
    cudaCallback(k, m, n, searchPoints, referencePoints, results, 1, 1, false);
}

/*##########################################
## cudaCallback2
## baseline策略, dim1=1, dim2=1, 使用内存优化
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
##########################################*/
extern void cudaCallback2(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results) {
    cudaCallback(k, m, n, searchPoints, referencePoints, results, 1, 1);
}

/*##########################################
## cudaCallback3
## 在M维优化的策略, dim1=128, dim2=1, 使用内存优化
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
##########################################*/
extern void cudaCallback3(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results) {
    cudaCallback(k, m, n, searchPoints, referencePoints, results, 128, 1);
}

/*##########################################
## cudaCallback4
## 在MK两维分别优化的策略, dim1=128, dim2=4, 不使用内存优化
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
##########################################*/
extern void cudaCallback4(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results) {
    
    cudaCallback(k, m, n, searchPoints, referencePoints, results, 128, 4, false);
}

/*##########################################
## cudaCallback5
## 在MK两维分别优化的策略, dim1=128, dim2=4, 使用内存优化
## params:
## m: 询问坐标数
## n: 参考坐标数
## k: 空间维度
## searchPoints: 询问坐标
## referencePoints: 参考坐标
## results: 输出结果
##########################################*/
extern void cudaCallback5(int k, int m, int n, float *searchPoints,
                         float *referencePoints, int **results) {
    
    cudaCallback(k, m, n, searchPoints, referencePoints, results, 128, 4);
}


