#include <stdio.h>
#define col 4
#define row 3

typedef struct
{
	int cost;
	int trip;
	int val;
}
Cost;

int supply[row]={50,75,25};//TO FILL
int demand[col]={20,20,50,60};//TO FILL

int tcost[row][col]={{3,5,7,6},{2,5,8,2},{3,6,9,2}};//TO FILL

Cost costNtrip[row][col];
void create(void)
{
  for (int i=0;i<row;i++)
  {
     for (int j=0;j<col;j++)
     {
       costNtrip[i][j].cost=tcost[i][j];
       costNtrip[i][j].trip=0;
       costNtrip[i][j].val=0;
     }
  }
}

int stop(int i,int j)
{
  if (supply[i]==0 || demand[j]==0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

void min(int i,int j)
{
  /*to subtract smaller of supply and demand from larger ones and update trip every time that happens*/
  int c=0;
  if (supply[i]>demand[j])
  {
    supply[i]-=demand[j];
    costNtrip[i][j].val=demand[j];
    demand[j]-=demand[j];
    c=1;
  }
  else
  {
    demand[j]-=supply[i];
    costNtrip[i][j].val=supply[i];
    supply[i]-=supply[i];
    c=1;
  }
  costNtrip[i][j].trip+=c;
}

void nwm(void) 
{
  for (int i=0;i<row;i++)
  {
    for (int j=0;j<col;j++)
    {
      while (stop(i,j))
      {
        min(i,j);
      }
    }
  }
}

int calc(void)
{
    /*it returns minimised cost by North West algorithm*/
  int totalcost=0;
  for (int i=0;i<row;i++)
  { 
    for (int j=0;j<col;j++)
    {
      if (costNtrip[i][j].trip>0)
      {
        totalcost+=costNtrip[i][j].cost*costNtrip[i][j].val;
      }
    }
  }
  return totalcost;
}

void display(void)
{
//to display cost matrix
  for (int i=0;i<row;i++)
  {   
       for (int j=0;j<col;j++)
       {       
         printf("  %d  ",costNtrip[i][j].cost);
       }
       printf("\n");
  }
}
int main(void)
{
	create();
	nwm();
	display();
	printf("\n\n\n");
	
	printf("Minimised transportation cost =%d",calc());
	return 0;
}
 
 
 
 
 
 
 
 
 
 
