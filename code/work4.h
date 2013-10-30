#include <stdio.h>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <time.h>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <algorithm>
#define input "input.txt"
#define output "output.txt"
#define OX 500
#define OY 500
#define ndebug
int FURY=1;//!!!!!
using  std::string;
using  std::wstring;
using namespace std;
string strs[60];
string patch[3];
int n;
ifstream fin(input);
FILE * fout = fopen(output,"w");
int skeleton[60],core;
bool visited[60];
set<int> alphabet[26];
int minx,miny,maxx,maxy;
int sum=0;
char a[1000][1000],ta[1000][1000];
int Tolerance=3;
bool untrim;
string pattern,sample;
string reverse(string sample)
{
    string tmp;
    int i;
    for (i=sample.length()-1;i>=0;i--){tmp.append(1,sample[i]);}
    return tmp;
}
string trim(string sample)
{
    int t;
    if (untrim) return sample;
    t = 0;
    while(sample[t]==' '){t++;}
    sample = sample.erase(0,t);
    t = 0;
    while(sample[sample.length()-1-t]==' '){t++;}
    sample = sample.erase(sample.length()-t,t);
    return sample;
}
int compare(string pattern,string ith)
{
    int i,now = 0,l;
    int start=-1;
    l = (int)(ith.length());
    if (l<=1){return -1;}
    for (i=0;i<l;i++)
    {
        if (pattern[i]==ith[now]||ith[i]==' ')
        {
            now++;
            if (now==1){start=i;}
            if (now==l){return start;}
        }
        else{now=0;}
    }
    return -1;
}
void ExpandArea(int x,int y,int z,int w)
{
    if (min(x,z)<minx) {minx = min(x,z);}
    if (max(x,z)>maxx) {maxx = max(x,z);}
    if (min(y,w)<miny) {miny = min(y,w);}
    if (max(y,w)>maxy) {maxy = max(y,w);}
}
bool whether_ExpandArea(int x,int y,int z,int w)
{
    if (min(x,z)<minx-Tolerance) {return false;}
    if (max(x,z)>maxx+Tolerance) {return false;}
    if (min(y,w)<miny-Tolerance) {return false;}
    if (max(y,w)>maxy+Tolerance) {return false;}
    ExpandArea(x,y,z,w);
    return true;
}
void printG()
{
    int i,j;
    for (i=minx;i<=maxx;i++)
    {
        for(j=miny;j<=maxy;j++)
        {
            if (a[i][j]!=' ')cout<<a[i][j];
            else cout<<'-';
        }
        cout<<endl;
    }
    cout<<endl;
}
bool Estimate_And_Paint(int x,int y,int z,int w,string s)
{
    int i,j,vx,vy;
    vx=(z-x)/((int)s.length()-1);
    vy=(w-y)/((int)s.length()-1);
    if (whether_ExpandArea(x,y,z,w))
    {
        sum++;
        for (i=0;i<s.length();i++)
        {
            if  (a[x+i*vx][y+i*vy] != s[i]&&a[x+i*vx][y+i*vy]!=' ')
            {
                printG();
                cout<<x-minx<<' '<<y-miny<<' '<<a[x][y]<<endl;
                cout<<s<<endl;
                system("pause");
            }
            a[x+i*vx][y+i*vy] = s[i];
        }
    }
    return whether_ExpandArea(x,y,z,w);
}
void Transv()
{
    int i,j,tmp;
    for (i=minx;i<=maxx;i++)
    {
        for (j=miny;j<(miny+maxy)/2;j++)
        {
            tmp= a[i][j];a[i][j]=a[i][miny+maxy-j];a[i][miny+maxy-j]=tmp;
        }
    }
}
void Transh()
{
    int i,j,tmp;
    for (i=miny;i<=maxy;i++)
    {
        for (j=minx;j<(minx+maxx)/2;j++)
        {
            tmp= a[j][i];a[j][i]=a[minx+maxx-j][i];a[minx+maxx-j][i]=tmp;
        }
    }
}
void Twist()
{
    int i,j,tmp;
    for (i=minx;i<=maxx;i++)
    {
        for (j=miny;j<=maxy;j++)
        {
            ta[i][j] = a[i][j];
            a[i][j] = ' ';
        }
    }
    for (j=0;j<=999;j++)
    {
        for (i=0;i<=999;i++)
        {
            if (i>=minx&&i<=maxx&&j>=miny&&j<=maxy)
            {a[j][i] = ta[i][j];}
        }
    }
    tmp =maxx;maxx=maxy;maxy=tmp;
    tmp =minx;minx=miny;miny=tmp;
}
void lap()
{
    int i,j,k,left,right;
    for (i=minx;i<=maxx;i++)
    {
        left = -1;right = -1;
        for (j=miny;j<=maxy;j++)
        {
            if (a[i][j]!=' '&&left == -1){left = j;}
            if (a[i][j]!=' ') {right = j;}
        }
        if (left!=-1)
        {
            for (k=0;k<n;k++)
            {
                if (visited[k]) continue;
                if (strs[k][0]==a[i][left])
                {
                    if (Estimate_And_Paint(i,left,i,left-strs[k].length()+1,strs[k])) {visited[k]=true;break;}
                }
                if (strs[k][strs[k].length()-1]==a[i][left])
                {
                    if (Estimate_And_Paint(i,left,i,left-strs[k].length()+1,reverse(strs[k]))) {visited[k]=true;break;}
                }
            }
        }
        if (right!=-1)
        {
            for (k=0;k<n;k++)
            {
                if (visited[k]) continue;
                if (strs[k][0]==a[i][right])
                {
                    if (Estimate_And_Paint(i,right,i,right+strs[k].length()-1,strs[k])) {visited[k]=true;break;}
                }
                if (strs[k][strs[k].length()-1]==a[i][right])
                {
                    if (Estimate_And_Paint(i,right,i,right+strs[k].length()-1,reverse(strs[k]))) {visited[k]=true;break;}
                }
            }
        }
    }
}
void reinforce()
{
    int i,j,k,crux,product;
    visited[core]=true;
    for (i=0;i<1000;i++)
    {
        for (j=0;j<1000;j++)
        {
            a[i][j]=' ';
        }
    }
    minx=OX;
    maxx=OX+strs[core].length()-1;
    miny=OY;
    maxy=OY;
    for (i=strs[core].length()-1;i>=0;i--)
    {
        a[OX+i][OY]=strs[core][i];
        if (skeleton[i]==-1) {continue;}
        if (visited[skeleton[i]]){system("pause");}
        visited[skeleton[i]]=true;
        crux = 0;product = 1000000;
        for (j=0;j<strs[skeleton[i]].length();j++)
        {
            if (strs[skeleton[i]][j]==strs[core][i])
            {
                if ((j-strs[skeleton[i]].length()/2)*(j-strs[skeleton[i]].length()/2)<product)
                {
                    crux = j;
                    product = (j-strs[skeleton[i]].length()/2)*(j-strs[skeleton[i]].length()/2);
                }
            }
        }
        crux = strs[skeleton[i]].find(strs[core][i]);
        for (j=0;j<strs[skeleton[i]].length();j++)
        {
            a[OX+i+j-crux][OY+j-crux]=(char)strs[skeleton[i]][j];
            ExpandArea(OX+i+j-crux,OY+j-crux,OX+i+j-crux,OY+j-crux);
        }
    }
}
void plot()
{
    //extend diag
    int i,j,k;
    string t;
    int start,pos,shift;
    for (i=minx;i<=maxx;i++)
    {
        sample = "";
        for (j=maxy;j>=max(miny,maxy-(maxx-i));j--)
        {
            sample.append(1,a[i+maxy-j][j]);
        }
        sample = trim(sample);
        bool letgo=false;
        for (j=0;j<n;j++)
        {
            if (letgo) {break;}
            pattern = strs[j];
            for (int twice=0;twice<=1;twice++)
            {
                if (visited[j]) {continue;}
                pos = compare(pattern,sample);
                if (pos>=0)
                {
                    for (k=maxy;k>=max(miny,maxy-(maxx-i));k--)
                    {
                        if (a[i+maxy-k][k]!=' '){shift = k;break;}
                    }
                    shift =untrim?0:maxy-shift;
                    if(Estimate_And_Paint(i-pos+shift,
                    maxy+pos-shift,
                    i-pos+pattern.length()-1+shift,maxy+pos-pattern.length()+1-shift,pattern)) {visited[j]=true;};
                    letgo=true;
                    break;
                }
                pattern = reverse(strs[j]);
            }
        }
    }
    //extend horizontal
    Tolerance=1;
    for (i=minx;i<=maxx;i++)
    {
        sample = "";
        for (j=miny;j<=maxy;j++)
        {
            sample.append(1,a[i][j]);
        }
        sample = trim(sample);
        bool letgo=false;
        for (j=0;j<n;j++)
        {
            pattern = strs[j];
            if (letgo) break;
            for (int twice=0;twice<=1;twice++)
            {
                if (visited[j]) {continue;}
                pos = compare(pattern,sample);
                if (pos>=0)
                {
                    for (k=miny;k<=maxy;k++)
                    {
                        if (a[i][k]!=' '){shift = k;break;}
                    }
                    shift =untrim?0:shift-miny;
                    if (Estimate_And_Paint(i,miny+shift-pos,i,miny+pattern.length()-1+shift-pos,pattern))  {visited[j]=true;};
                    letgo=true;
                    break;
                }
                pattern = reverse(strs[j]);
            }
        }
    }
    //extend vertical
    for (i=miny;i<=maxy;i++)
    {
        sample = "";
        for (j=minx;j<=maxx;j++)
        {
            sample.append(1,a[j][i]);
        }
        sample = trim(sample);
        bool letgo=false;
        for (j=0;j<n;j++)
        {
            pattern = strs[j];
            if (letgo) break;
            for (int twice=0;twice<=1;twice++)
            {
                if (visited[j]) {continue;}
                pos = compare(pattern,sample);
                if (pos>=0)
                {
                    for (k=minx;k<=maxx;k++)
                    {
                        if (a[k][i]!=' '){shift = k;break;}
                    }
                    shift =untrim?0:shift-minx;
                    if (Estimate_And_Paint(minx+shift-pos,i,minx+pattern.length()-1+shift-pos,i,pattern))  {visited[j]=true;};
                    letgo=true;
                    break;
                }
                pattern = reverse(strs[j]);
            }
        }
    }
    //printG();
}
int ToUpper(int c)
{
    return toupper(c);
}

void init()
{
    int i,j,randomi;
    string tmp;
    srand((int)time(0));
    fin>>n;
    for (i=0;i!=n;i++){fin>>strs[i];transform(strs[i].begin(), strs[i].end(), strs[i].begin(), ToUpper);}
    for (i=0;i!=n;i++)
    {
        randomi = rand()%n;
        tmp = strs[randomi];strs[randomi] = strs[i];strs[i] = tmp;
    }
    for (i=0;i!=n;i++)
    {
        for (j=0;j<strs[i].length();j++)
        {
            if (alphabet[int(strs[i][j])-65].find(i)==alphabet[int(strs[i][j])-65].end())
            {
                alphabet[int(strs[i][j])-65].insert(i);
            }
        }
    }

}
void set_skeleton()
{
    int i,j,seed,tmpsum,maxestimation=0;
    bool chosen[60];
    set<int>::iterator index;
    int tmpskeleton[60];
    memset(visited,0,60*sizeof(bool));
    for (i=0;i<3;i++)
    {
        patch[i] = strs[n-1-i];
        visited[n-1-i] = true;
    }
    srand((int)time(0));
    for (i=0;i<n;i++)
    {
        if (visited[i]) {continue;}
        tmpsum=0;
        for (j=0;j<n;j++) {chosen[j]=false;tmpskeleton[j]=-1;}
        chosen[i] = true;
        for (j=0;j<strs[i].length();j++)
        {
            for (index = alphabet[strs[i][j]-65].begin();index != alphabet[strs[i][j]-65].end();index++)
            {
                if (chosen[*index]||visited[*index]) {continue;}
                chosen[*index] = true;
                tmpskeleton[j] = *index;
                tmpsum++;
                break;
            }
        }
        seed = rand()%n/2;
        if (tmpsum+seed>maxestimation) {for (j=0;j<n;j++) {maxestimation=tmpsum+seed;core=i;skeleton[j]=tmpskeleton[j];sum=tmpsum+1;}}
    }
}

bool check()
{
    if (a[minx][miny]==' '&&a[minx][maxy]==' '&&a[maxx][miny]==' '&&a[maxx][maxy]==' ')
    {
        return false;
    }
    if (((maxx-minx)-(maxy-miny))*((maxx-minx)-(maxy-miny))>=4) return false;
    Tolerance = 0;
    untrim = 1;
    plot();
    if (sum!=n-3) return false;
    return true;
}
void deal(char *oa,int *ominx,int *ominy,int *omaxx,int *omaxy,int *osum,string &s1,string &s2,string &s3)
{
    init();
    set_skeleton();
    reinforce();
    untrim=0;
    Tolerance=3;plot();Tolerance=2;lap();Twist();lap();
    Tolerance=2;plot();Tolerance=1;lap();Twist();lap();
    untrim=1;
    Tolerance=1;plot();Tolerance=2;lap();Twist();lap();
    untrim=0;
    int i,j;
    int x,y,x0,y0,sqrt,sqrt0;
    x=maxx-minx;y=maxy-miny;sqrt=(x-y)*(x-y);
    x0=*omaxx-*ominx;y0=*omaxy-*ominy;sqrt0=(x0-y0)*(x0-y0);
    if (!check()) return;
    if (sum-x/2-y/2-sqrt<=sum-x0/2-y0/2-sqrt0) return;
    s1 = strs[n-1];s2 = strs[n-2];s3 = strs[n-3];
    for (i=minx;i<=maxx;i++)
    {
        for (j=miny;j<=maxy;j++)
        {
            oa[i*1000+j] = a[i][j];
        }
    }
    *ominx=minx;
    *ominy=miny;
    *omaxx=maxx;
    *omaxy=maxy;
    *osum =sum;
}

