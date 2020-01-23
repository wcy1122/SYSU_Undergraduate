#define __CLOCK_H
#define __CLOCK_H

void sleep(int cnt)
{
  int i,j;
  for(i=1;i<=cnt;i++)
    for(j=1;j<=10000;j++);
}

