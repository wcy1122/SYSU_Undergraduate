/* �����������������߳̽������(1+2+...+n),Ȼ��ֱ���һ������Ԫ�ط��ؽ����
ȫ�ֱ���sumall���������̵߳�������֮�� */

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

struct struparam{             /* ���ڴ������ */
	 int threadno;            /* �̱߳�� */
	 int count;				  /* ����ֵ: ÿ���̶߳��Ǵ�0�ӵ�count */
};

double sumone[]={0,0,0};               // ���ڷ��ظ��߳����ĺ�ֵ
double sumall=0;                       // ��¼�����߳��ܺ�ֵ��

// ���1~p->count���������sumone[p->threadno]��ÿ�μ���Ҳ����ȫ�ֹ������sumall 
unsigned __stdcall SumAll(void * p)    // �ýṹ������ȫ�ֱ����������
{ 
    struparam * p1 = (struparam *)p;   // ��������ת��
	int i;
	double sum=0;
	int x;

    for(i=0;i<p1->count;i++)
	{
  	    sum+=i;
		sumall+=i;                     //   �ٽ�����ÿ��ʱ��ֻ����һ���߳̽����ٽ�������������ٽ������Ϳ��ܳ��������̷ֱ߳������sumall��ʵ��ֻ����һ������		
	    printf("%d'th thread: the sum from 1 to %d is %0.0f. total is %0.0f\n", p1->threadno, i, sum,sumall);
        fflush(stdout);
	}  
	sumone[p1->threadno]=sum;         //��ȫ�ֱ������غ�ֵ 
    return 0;                         //����ʱ�Զ������̡߳�_endthreadex(0)Ҳ�������ڽ����߳�
} 


void main()
{ 
    HANDLE hThread1,hThread2;
	struparam  p1, p2;
	struparam *ptr1, *ptr2;

	p1.threadno  = 1;                 // �̱߳�ţ��Զ��壩����������
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
	getchar();                      // ��������˳�
}
