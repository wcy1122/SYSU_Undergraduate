/* 创建两个独立的子线程进行求和(1+2+...+n),然后分别用一个数组元素返回结果。
全局变量sumall保存所有线程的所加数之和 */

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

struct struparam{             /* 用于带入参数 */
	 int threadno;            /* 线程编号 */
	 int count;				  /* 计数值: 每个线程都是从0加到count */
};

double sumone[]={0,0,0};               // 用于返回各线程最后的和值
double sumall=0;                       // 记录所有线程总和值。

// 求和1~p->count，结果存在sumone[p->threadno]。每次加数也加入全局共享变量sumall 
unsigned __stdcall SumAll(void * p)    // 用结构变量或全局变量带入参数
{ 
    struparam * p1 = (struparam *)p;   // 参数类型转换
	int i;
	double sum=0;
	int x;

    for(i=0;i<p1->count;i++)
	{
  	    sum+=i;
		sumall+=i;                     //   临界区，每个时刻只能有一个线程进入临界区。如果不用临界区，就可能出现两个线程分别加数到sumall，实际只加了一个数。		
	    printf("%d'th thread: the sum from 1 to %d is %0.0f. total is %0.0f\n", p1->threadno, i, sum,sumall);
        fflush(stdout);
	}  
	sumone[p1->threadno]=sum;         //用全局变量返回和值 
    return 0;                         //返回时自动结束线程。_endthreadex(0)也可以用于结束线程
} 


void main()
{ 
    HANDLE hThread1,hThread2;
	struparam  p1, p2;
	struparam *ptr1, *ptr2;

	p1.threadno  = 1;                 // 线程编号（自定义），用于数组
	p1.count = 40000;

    p2.threadno = 2;
	p2.count= 40000;

    ptr1= &p1;
    ptr2= &p2;

    hThread1 = (HANDLE)_beginthreadex(NULL, 0,&SumAll, (void *)ptr1, 0, NULL);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0,&SumAll,(void *)ptr2, 0, NULL);

    // wait for terminating of thread in order to prevent from the termination of the main thread by runing to the end 
    WaitForSingleObject(hThread1, INFINITE );
    WaitForSingleObject(hThread2, INFINITE );
    printf("Finished! sums are %0.0f and %0.0f.the total is %0.0f. \n",sumone[1],sumone[2],sumone[1]+sumone[2]);
    printf("Finished! the shared total is %0.0f.\n",sumall);

    // destroy the thread object.
    CloseHandle(hThread1);
    CloseHandle(hThread2);
	getchar();                      // 按任意键退出
}
