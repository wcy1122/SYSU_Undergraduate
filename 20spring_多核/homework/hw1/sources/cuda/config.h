/*###################################################
##  文件说明：
##  配置文件
#####################################################*/

#ifndef _INCL_CONFIG
#define _INCL_CONFIG

// Whether to print a portion of each sample (disable: 0)
int printSample = 0;
// Whether to print a portion of each result (disable: 0)
int printResult = 1;
// Whether to print running time of all matrix
int printAllTime = 1;
// whether method to run
// 0: baseline
// 1: memory_time
// 2: precount log
// 3: share memory
// 4: static memory
int MethodFlag = 3;

/*
 * Don't change the settings below if you are going to use the rexec utility
 */

// The relative path to the data file
char *dataPath = "./data.bin";
// The relative path to the output results
char *outputPath = "./results.csv";

#endif