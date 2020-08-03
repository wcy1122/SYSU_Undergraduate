/*###################################################
##  文件说明：
##  主函数
#####################################################*/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "config.h"
#include "utils.h"
#include "samples.h"
using namespace std;

/*###########################
## ompwork
## 利用openmp计算子矩阵信息熵
## params:
## width, height: 矩阵长宽
## input: 输入数据
## output: 运算结果
###########################*/
void ompwork(int width, int height, float *input, float *output) {
    int k;
    int size = width * height;
    int TH_NUM = min(size, 1000);
    int Blo_Size = size / TH_NUM;
    #pragma omp parallel for private(k)
    for(k=0; k<TH_NUM; k++)
    {
        int _l = Blo_Size * k;
        int _r = _l + Blo_Size;
        if(k == TH_NUM-1) _r = size;
        for(int idx=_l;idx<_r;idx++){
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
                    ans -= prob * log(prob);
                }
            output[idx] = ans;
        }
    }
    // printf("%f %f %f\n", output[2], output[4], output[15]);
}

/*#####################################
## ompCallback
## 利用openmp计算子矩阵信息熵的callback函数
## params:
## width, height: 矩阵长宽
## input: 输入数据
## output: 运算结果
#####################################*/
void ompCallback(int width, int height, float *input, float * &output) {
    int size = width * height;
    float *input_d, *output_d;
    output = (float*)malloc(sizeof(float)*size);
    ompwork(width, height, input, output);
}

/*#####################################
## main
## 主函数，文件读取，输出输入输出
#####################################*/
int main() {
    // Timestamp variables
    long st, et;

    // Open the data file
    FILE *stream = fopen(dataPath, "rb");
    if (stream == NULL) {
        printf("failed to open the data file\n");
        return -1;
    }

    // Open a stream to write out results in text
    FILE *outStream = fopen(outputPath, "w");
    if (outStream == NULL) {
        printf("failed to open the output file\n");
        return -1;
    }
    
    int _first = 1;

    // Read in and process the samples one-by-one
    int width, height, size;
    float *sample, *result;
    float AllTime = 0;
    while (getNextSample(stream, &width, &height, &sample) != 0) {
        size = width * height;

        if (printSample) {
            // Print out a small portion of the sample
            printf("sample:\n");
            for (int j = height - 5; j < height; j++) {
                for (int i = width - 5; i < width; i++) {
                    printf("%8.5f ", sample[j*width+i]);
                }
                printf("\n");
            }
        }
        
        st = getTime();
        ompCallback(width, height, sample, result);    
        et = getTime();
        // Invoke the callback

        if (printResult) {
            // Print out a small portion of the result
            printf("result:\n");
            for (int j = height - 5; j < height; j++) {
                for (int i = width - 5; i < width; i++) {
                    printf("%8.5f ", result[j*width+i]);
                }
                printf("\n");
            }
        }
        
        printf("size: %d\n", width*height);
        printf("cudaCallback: %.3f ms\n", (et - st) / 1e6);
        if(!_first) AllTime += (et - st) / 1e6;
        else _first = 0;

        // Write the result to the output stream
        char buffer[128];
        sprintf(buffer, "%d,", width);
        W_CHK(fputs(buffer, outStream));
        sprintf(buffer, "%d,", height);
        W_CHK(fputs(buffer, outStream));
        for (int i = 0; i < size; i++) {
            sprintf(buffer, "%.5f,", result[i]);
            W_CHK(fputs(buffer, outStream));
        }
        W_CHK(fputs("\n", outStream));

        // De-allocate the sample and the result
        free(sample);
        sample = NULL;
        free(result);
        result = NULL;
    }
    
    printf("AllTime: %.3f ms\n\n", AllTime);

    // Close the output stream
    fclose(outStream);
    return 0;
}