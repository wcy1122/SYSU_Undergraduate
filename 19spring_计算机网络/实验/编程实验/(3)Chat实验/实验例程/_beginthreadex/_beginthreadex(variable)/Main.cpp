/* �򵥱������롣�����������������߳̽������(1+2+...+n),Ȼ��ֱ���һ������Ԫ�ط��ؽ����
   ȫ�ֱ���sumall���������̵߳ļ���֮�� */

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <math.h>

double sums[]={0,0,0};               // ���ڷ��ظ��߳����ĺ�ֵ
double allsum=0;                       // ��¼�����߳��ܺ�ֵ��

// ���1~40000���������sumone[*p]�С�ÿ�μ���ʱҲ�ӽ�ȫ�ֹ������sumall 
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
	sums[tno]=sum;                   // ��ȫ�ֱ������غ�ֵ 
    return 0;                         // ����ʱ�Զ������̡߳�_endthreadex(0)Ҳ�������ڽ����߳�
} 

void main()
{ 
    HANDLE hThread1,hThread2;
	int  p1=1, p2=2;
	
    hThread1 = (HANDLE)_beginthreadex(NULL, 0,&sumAll, (void *)&p1, 0, NULL);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0,&sumAll, (void *)&p2, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE );   //�ȴ��߳�hThread1����
    WaitForSingleObject(hThread2, INFINITE );   //�ȴ��߳�hThread2����
    printf("Finished! The sums are %0.0f and %0.0f.the total is %0.0f. \n",sums[1],sums[2],sums[1]+sums[2]);
    printf("Finished! The shared sum is %0.0f.\n",allsum);

    CloseHandle(hThread1);          // �ر��߳̾�����ͷ��߳���Դ
    CloseHandle(hThread2);
	getchar();                      // ��������˳�
}
