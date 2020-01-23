/* ������ͬ��͵������߳�,������sumall�������̵߳��ܺ� */

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

struct struparam{             /* ���ڴ������ */
	 int threadno;            /* �̱߳�� */
	 int count;				  /* ����ֵ: ÿ���̶߳��Ǵ�0�ӵ�count */
};

CRITICAL_SECTION cs;		           // �ٽ�����ÿ��ʱ��ֻ��һ���߳̿��Խ����ٽ���
double sumone[]={0,0,0};               // ���ڷ��ظ��߳����ĺ�ֵ
double sumall=0;                       // ��¼�����߳��ܺ�ֵ��

// ���1~p->count���������sumone[p->threadno]��ÿ�μ���Ҳ����ȫ�ֹ������sumall 
unsigned __stdcall SumAll(void * p)    // �ýṹ������ȫ�ֱ����������
{ 
    struparam * p1 = (struparam *)p;   // ��������ת��
	int i;
	double sum=0;

    for(i=0;i<p1->count;i++)
	{
  	    sum+=i;
		EnterCriticalSection(&cs);     // �ȴ������ٽ���
		sumall+=i;                     //   �ٽ�����ÿ��ʱ��ֻ����һ���߳̽����ٽ�������������ٽ������Ϳ��ܳ��������̷ֱ߳������sumall��ʵ��ֻ����һ������
		LeaveCriticalSection(&cs);     // �뿪�ٽ���
	    printf("%d'th thread: the sum from 1 to %d is %0.0f. total is %0.0f\n", p1->threadno, i, sum,sumall);
        fflush(stdout);
	}  
	sumone[p1->threadno]=sum;         //��ȫ�ֱ������غ�ֵ 
    return 0;                         //����ʱ�Զ������̡߳�_endthreadex(0)Ҳ�������ڽ����߳�
} 


void main()
{ 
    HANDLE hThread,hThread1;
    unsigned threadID,threadID1;
	struparam  p1, p2;
	struparam *ptr1, *ptr2;

	InitializeCriticalSection(&cs);   // �ٽ�����ʼ��
    
	p1.threadno  = 1;                 // �̱߳��
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

    DeleteCriticalSection (&cs) ;  // ɾ���ٽ���

    // destroy the thread object.
    CloseHandle(hThread);
    CloseHandle(hThread1);
	getchar();
}
