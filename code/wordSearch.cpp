#include "work4.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#define output "output.txt"

using namespace std;
int main()
{
    FILE * fout = fopen(output,"w");
    int i,j;
    char *a;
    bool visited[60];
    string ss1,ss2,ss3;
    srand(time(0));
    int minx=0,miny=0,maxx=1000,maxy=1000,sum,maxsum;
    a = (char *)malloc(sizeof(char)*1000000);
    for (int times=0;times<100;times++)
    {
        deal(a,&minx,&miny,&maxx,&maxy,&sum,ss1,ss2,ss3);
    }
    for (i=minx;i<=maxx;i++)
    {

        for (j=miny;j<=maxy;j++)
        {
            if (a[i*1000+j]!=' ')
            {
                fprintf(fout,"%c",a[i*1000+j]);
            }
            else {fprintf(fout," ");}
        }
        fprintf(fout,"\n");

    }
    cout<<ss1<<endl<<ss2<<endl<<ss3<<endl;
}
