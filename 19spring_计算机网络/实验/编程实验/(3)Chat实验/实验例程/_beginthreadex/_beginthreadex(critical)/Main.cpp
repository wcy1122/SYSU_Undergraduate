/* 独立共同求和的两个线程,并利用sumall求两个线程的总和 */

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

struct struparam{             /* 用于带入参数 */
	 int threadno;            /* 线程编号 */
	 int count;				  /* 计数值: 每个线程都是从0加到count */
};

CRITICAL_SECTION cs;		           // 临界区。每个时刻只有一个线程可以进入临界区
double sumone[]={0,0,0};               // 用于返回各线程最后的和值
double sumall=0;                       // 记录所有线程总和值。

// 求和1~p->count，结果存在sumone[p->threadno]。每次加数也加入全局共享变量sumall 
unsigned __stdcall SumAll(void * p)    // 用结构变量或全局变量带入参数
{ 
    struparam * p1 = (struparam *)p;   // 参数类型转换
	int i;
	double sum=0;

    for(i=0;i<p1->count;i++)
	{
  	    sum+=i;
		EnterCriticalSection(&cs);     // 等待进入临界区
		sumall+=i;                     //   临界区，每个时刻只能有一个线程进入临界区。如果不用临界区，就可能出现两个线程分别加数到sumall，实际只加了一个数。
		LeaveCriticalSection(&cs);     // 离开临界区
	    printf("%d'th thread: the sum from 1 to %d is %0.0f. total is %0.0f\n", p1->threadno, i, sum,sumall);
        fflush(stdout);
	}  
	sumone[p1->threadno]=sum;         //用全局变量返回和值 
    return 0;                         //返回时自动结束线程。_endthreadex(0)也可以用于结束线程
} 


void main()
{ 
    HANDLE hThread,hThread1;
    unsigned threadID,threadID1;
	struparam  p1, p2;
	struparam *ptr1, *ptr2;

	InitializeCriticalSection(&cs);   // 临界区初始化
    
	p1.threadno  = 1;                 // 线程编号
	p1.count = 80000;

    p2.threadno = 2;
	p2.count= 80000;

    ptr1= &p1;
    ptr2= &p2;

    hThread = (HANDLE)_beginthreadex(NULL, 0,&SumAll, (void *)ptr1, 0, &threadID);
    hThread1 = (HANDLE)_beginthreadex(NULL, 0,&SumAll,(void *)ptr2, 0, &threadID1);

    printf("ThreadID: %d\n", threadID);
    printf("ThreadID1: %d\n", threadID1);
 


    // wait for terminating of thread in order to prevent from the termination of the main thread by runing to the end 
    WaitForSingleObject(hThread1, INFINITE );
    WaitForSingleObject(hThread, INFINITE );
    printf("Finished! sums are %0.0f and %0.0f.the total is %0.0f. \n",sumone[1],sumone[2],sumone[1]+sumone[2]);
    printf("Finished! the shared total is %0.0f.\n",sumall);

    DeleteCriticalSection (&cs) ;  // 删除临界区

    // destroy the thread object.
    CloseHandle(hThread);
    CloseHandle(hThread1);
	getchar();
}
