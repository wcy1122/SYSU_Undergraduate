/* 简单变量带入。创建两个独立的子线程进行求和(1+2+...+n),然后分别用一个数组元素返回结果。
   全局变量sumall保存所有线程的加数之和 */

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

double sums[]={0,0,0};               // 用于返回各线程最后的和值
double allsum=0;                       // 记录所有线程总和值。

// 求和1~40000，结果存在sumone[*p]中。每次加数时也加进全局共享变量sumall 
unsigned __stdcall sumAll(void *pno) 
{ 
    int  tno = *((int *)pno);
	double sum=0;

    for(int i=1;i<=3000;i++)
	{
  	    sum+=i;
		allsum+=i; 
	    printf("%d'th thread: the sum from 1 to %d is %0.0f. total is %0.0f\n", tno, i, sum, allsum);
        fflush(stdout);
	}  
	sums[tno]=sum;                   // 用全局变量返回和值 
    return 0;                         // 返回时自动结束线程。_endthreadex(0)也可以用于结束线程
} 

void main()
{ 
    HANDLE hThread1,hThread2;
	int  p1=1, p2=2;
	
    hThread1 = (HANDLE)_beginthreadex(NULL, 0,&sumAll, (void *)&p1, 0, NULL);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0,&sumAll, (void *)&p2, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE );   //等待线程hThread1结束
    WaitForSingleObject(hThread2, INFINITE );   //等待线程hThread2结束
    printf("Finished! The sums are %0.0f and %0.0f.the total is %0.0f. \n",sums[1],sums[2],sums[1]+sums[2]);
    printf("Finished! The shared sum is %0.0f.\n",allsum);

    CloseHandle(hThread1);          // 关闭线程句柄，释放线程资源
    CloseHandle(hThread2);
	getchar();                      // 按任意键退出
}
