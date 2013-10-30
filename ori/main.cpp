
#include <stdio.h>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <time.h>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#define input "input.txt"
#define output "output.txt"
#define OX 500
#define OY 500

///////////////////////////////////////////////////// 斜线之间是我写的部分 其他部分无修改 ////////////////////////////////////////////////////////////////////
/*
 * 評估函數各評估項權重。從以下各個方面評估，以便隨時調整權重，達到最佳估計效果。
*/
#define ESTIMATE_WEIGHT_AMNT 2 //这个主骨架直接连接较多单词数量的能力 的权重。 一定程度上也能体现主骨架长度。
#define ESTIMATE_WEIGHT_EXTND 2 //骨架可扩展性，骨架再连接较多单词数量的能力 的权重。一定程度上也能体现副骨架长度。
#define ESTIMATE_WEIGHT_SHP_H 1 //骨架水平方向方向分散均匀、重心不偏 的权重
#define ESTIMATE_WEIGHT_SHP_V 0 //骨架竖直方向方向分散均匀、重心不偏 的权重
//////////////////////////////////////////////////////////////////// END /////////////////////////////////////////////////////////


using  std::string;
using  std::wstring;
using namespace std;
string strs[60];
int n;
ifstream fin(input);
FILE * fout = fopen(output,"w");
int skeleton[60], core;
bool visited[60];
set<int> alphabet[26]; 

int minx, miny, maxx, maxy;

int compare (string pattern,string ith) 
{
    int now = 0;
    int start=-1;
    for (int i=0;i<ith.length();i++)
    {
        if (pattern[now]==ith[i]||pattern[now]==' ')
        {
            now++;
            if (now==1){start=i;}
            if (now==pattern.length()){return start;}
        }
        else{now=0;}
    }
    return -1;
}

//////////////////////////////////////////////// 斜线之间是我写的部分 其他部分无修改 //////////////////////////////////////////////////////////////
char pattern[240][60]; //storage tmp matrics for finding patterns
char skl_mtrcs[100][100];
/*
 * 把骨架画出来，供分析所有模式串使用
 * skl[i][j], j = 0 表示次骨架单词的序号，j = 1表示副骨架这个单词的第几个字母与主骨架结合。所有序号以0开始。
 */
void plot_skl_mtrcs(int num, int skl[60][2])
{
	int Lm, L, p, mid, i, j;
	int X, Y;
	char c;
	string s;
	//plot main skeleton
	s = strs[num];
	Lm = s.length();
	for(int mid = Lm / 2, k = 0; mid - k >=0; ++k)
	{
		skl_mtrcs[50][50 - k] = s[mid - k];
		skl_mtrcs[50][50 + k] = s[mid + k]; // 主骨架偶数个元素，最后一个该是 /0  再验证
	}
	//plot each word on main skeleton
	for (i = 0; i < strs[num].length(); ++i){
		if (skl[i][0] == -1){
			continue;
		}
		L = strs[skl[i][0]].length();
		p = skl[i][1];
		for (j = 0; j < L; j++){
			c = strs[skl[i][0]][j];
			X = 50 - p + j;
			Y = 50 - (Lm / 2 - i) - (p - j);
			skl_mtrcs[X][Y] = c;
		}
	}
}

/*
 * 找出四个方向全部模式串，写入string pattern[]，pattern[i][0] = '\0'表示后面没有了。
 * 四个方向为水平左到右，竖直上到下，还有左上到右下，和右上到左下。
 */
void find_all_prtn(int num, int skl[60][2])
{
	int i_ptn = 0, i, j, k;
	int beginX, beginY, endX, endY;
	char c;
    plot_skl_mtrcs(num, skl);
	//horizontal
	for (i = 0; i < 100; ++i){
		endX = 99;
		endY = 0;
		while (beginX <= endX){
			if (skl_mtrcs[beginX][i] == '\0'){
				beginX++;
			}
			if (skl_mtrcs[endX][i] == '\0'){
				endX--;
			}
			if (skl_mtrcs[beginX][i] != '\0' && skl_mtrcs[endX][i] != '\0'){
				break;
			}
		}
		if (skl_mtrcs[i][beginX] != '\0'){
			for (j = 0; j <= endX - beginX; j++){
				c = skl_mtrcs[beginX + j][i];
				if (c = '\0'){
					c = ' ';
				}
				pattern[i_ptn][j] = c;
			}
			pattern[i_ptn++][j] = '\0';
		}
	}
	//vertical
	for (i = 0; i < 99; ++i){
		beginY = 0;
		endY = 00;
		while (beginY <= endY){
			if (skl_mtrcs[i][beginY] == '\0'){
				beginY++;
			}
			if (skl_mtrcs[i][endY] == '\0'){
				endY--;
			}
			if (skl_mtrcs[i][beginY] != '\0' && skl_mtrcs[i][endY] != '\0'){
				break;
			}
		}
		if (skl_mtrcs[beginY][i] != '\0'){
			for (j = 0; j <= endY - beginY; j++){
				c = skl_mtrcs[beginY + j][i];
				if (c = '\0'){
					c = ' ';
				}
				pattern[i_ptn][j] = c;
			}
			pattern[i_ptn++][j] = '\0';
		}
	}
	//左上到右下
	//stage 1
	for (i = 99; i >= 0; --i){
		beginX = i;
		beginY = 0;
		endX = i;
		endY = 99;
		while (beginX <= endX && beginY <= endY){
			if (skl_mtrcs[beginY][beginX] == '\0' && beginX < endX && beginY < endY){
				beginX++;
				beginY++;
			}
			if (skl_mtrcs[endY][endX] == '\0' && endX > beginX && endY > endY){
				endX--;
				endY--;
			}
			if (skl_mtrcs[beginX][beginY] != '\0' && skl_mtrcs[endX][endY] != '\0'){
				break;
			}
		}
		if (skl_mtrcs[beginY][beginX] != '\0'){
			j = 0;
			while (beginX <= endX && beginY <= endY){
				c = skl_mtrcs[beginY][beginX];
				if (c = '\0'){
					c = ' ';
				}
				pattern[i_ptn][j] = c;
				j++;
				beginX++;
				beginY++;
			}
			pattern[i_ptn++][j] = '\0';
		}
	}
	//stage 2
	for (i = 1; i <= 99; ++i){
		beginX = 0;
		beginY = i;
		endX = 99 - i;
		endY = 99;
		while (beginX <= endX && beginY <= endY){
			if (skl_mtrcs[beginY][beginX] == '\0' && beginX < endX && beginY < endY){
				beginX++;
				beginY++;
			}
			if (skl_mtrcs[endY][endX] == '\0' && endX > beginX && endY > beginY){
				endX--;
				endY--;
			}
			if (skl_mtrcs[beginX][beginY] != '\0' && skl_mtrcs[endX][endY] != '\0'){
				break;
			}
		}
		if (skl_mtrcs[beginY][beginX] != '\0'){
			j = 0;
			while (beginX <= endX && beginY <= endY){
				c = skl_mtrcs[beginY][beginX];
				if (c = '\0'){
					c = ' ';
				}
				pattern[i_ptn][j] = c;
				j++;
				beginX++;
				beginY++;
			}
			pattern[i_ptn++][j] = '\0';
		}
	}
	//右上到左下
	//stage 1
	for (i = 0; i <= 99; ++i){
		beginX = i;
		beginY = 0;
		endX = 0;
		endY = i;
		while (beginX <= endX && beginY >= endY){
			if (skl_mtrcs[beginY][beginX] == '\0' && beginX < endX && beginY > endY){
				beginX--;
				beginY++;
			}
			if (skl_mtrcs[endY][endX] == '\0' && endX > beginX && endY < endY){
				endX++;
				endY--;
			}
			if (skl_mtrcs[beginX][beginY] != '\0' && skl_mtrcs[endX][endY] != '\0'){
				break;
			}
		}
		if (skl_mtrcs[beginY][beginX] != '\0'){
			j = 0;
			while (beginX >= endX && beginY <= endY){
				c = skl_mtrcs[beginY][beginX];
				if (c = '\0'){
					c = ' ';
				}
				pattern[i_ptn][j] = c;
				j++;
				beginX--;
				beginY++;
			}
			pattern[i_ptn++][j] = '\0';
		}
	}
	//stage 2
	for (i = 98; i >= 0; --i){
		beginX = 99;
		beginY = 99 - i;
		endX = i;
		endY = 99;
		while (beginX <= endX && beginY >= endY){
			if (skl_mtrcs[beginY][beginX] == '\0' && beginX < endX && beginY > endY){
				beginX--;
				beginY++;
			}
			if (skl_mtrcs[endY][endX] == '\0' && endX > beginX && endY < endY){
				endX++;
				endY--;
			}
			if (skl_mtrcs[beginX][beginY] != '\0' && skl_mtrcs[endX][endY] != '\0'){
				break;
			}
		}
		if (skl_mtrcs[beginY][beginX] != '\0'){
			j = 0;
			while (beginX >= endX && beginY <= endY){
				c = skl_mtrcs[beginY][beginX];
				if (c = '\0'){
					c = ' ';
				}
				pattern[i_ptn][j] = c;
				j++;
				beginX--;
				beginY++;
			}
			pattern[i_ptn++][j] = '\0';
		}
	}
	pattern[i_ptn][0] = '\0';
}
/*
 * 评估函数。返回值越大越好，正负都有可能。
 * num主骨架序号，skl[][0]次骨架序号，skl[][1]次骨架与主骨架连接的字母在次骨架串中的下标。
 */
 int esimate(int num, int skl[60][2])
 {
    string s;
    string skl_main = strs[num]; // 主骨架
    int i, j, k;
    int est_rslt, est_amnt = 0, est_extnd = 0, est_shp_h = 0, est_shp_v; // 各个小方面的评估值
    char c;

    //主骨架连接出的次骨架的个数
    if(ESTIMATE_WEIGHT_AMNT != 0)
    {
        for(i = 0; i < skl_main.length(); ++i)
        {
            if(skl[i][0] != -1)
                est_amnt++;
        }
    }

    //骨架可拓展性
    int flag;
    if(ESTIMATE_WEIGHT_EXTND != 0)
    {
        find_all_prtn(num, skl);
        for(i = 0; pattern[i][0] != '\0'; i++) // every pattern
        {
            for(j = 0; j < n; j++) // every pattern with every word
            {
                
                //omit main skeleton
                if(j == num)
                    continue;
                //omit deputy skeletons
                flag = 0;
                for(k = 0; k < skl_main.length(); ++k)
                {
                    if(j == skl[k][0])
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                    continue;

                //compare
                if(compare(pattern[i], strs[j]) != -1 && compare(pattern[i],strrev( &(strs[j][0]) )) != -1)
                {
                    est_extnd++;
                    break;
                }
            }
        }
    }

    //骨架形状 est_shp < 0,绝对值越小越好
    //考虑水平方向失衡程度
    if(ESTIMATE_WEIGHT_SHP_H != 0)
    {
        int unbalance_h, p, l;
        for(i = 0; i <= est_amnt; ++i)
        {
            s = strs[ skl[i][0] ];
            l = s.length();
            for(int mid = l / 2, j = 0; mid - j >=0; ++j)
            {
                if(s[mid - j] == skl_main[i])
                {
                    p = mid - j;
                    break;
                }    
                else if(s[mid + j] == skl_main[i])                      
                {
                    p = mid + j;
                    break;
                }
            }
            unbalance_h = 2 * p - l + 1;
            if(unbalance_h < 0)
                unbalance_h *= -1;
            est_shp_h -= unbalance_h;            
        }
        est_shp_h /= est_amnt;
    }

    return ESTIMATE_WEIGHT_AMNT * est_amnt 
            + ESTIMATE_WEIGHT_EXTND * est_extnd 
            + ESTIMATE_WEIGHT_SHP_H * est_shp_h
            + ESTIMATE_WEIGHT_SHP_V * est_shp_v;

}

 //////////////////////////////////////////// END ////////////////////////////////////////
void plot()
{
    int i,j,crux;
    char a[1000][1000];
    memset(visited,0,60*sizeof(bool));
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
    for (i=0;i<strs[core].length();i++)
    {
        visited[skeleton[i]]=true;
        a[OX+i][OY]=strs[core][i];
        if (skeleton[i]==-1) {continue;}
        cout<<strs[skeleton[i]]<<endl;
        crux = strs[skeleton[i]].find(strs[core][i]);
        for (j=0;j<strs[skeleton[i]].length();j++)
        {
            a[OX+i+j-crux][OY+j-crux]=(char)strs[skeleton[i]][j];
            if (OX+i+j-crux<minx) {minx = OX+i+j-crux;}
            if (OX+i+j-crux>maxx) {maxx = OX+i+j-crux;}
            if (OY+j-crux<miny) {miny = OY+j-crux;}
            if (OY+j-crux>maxy) {maxy = OY+j-crux;}
        }
    }
    for (i=minx;i<=maxx;i++)
    {
        for (j=miny;j<=maxy;j++)
        {
            fprintf(fout,"%c",a[i][j]);
        }
        fprintf(fout,"\n");
    }
}

void init() // 读入
{
    int i,j,randomi;
    string tmp;
    srand((int)time(0));
    fin>>n;
    for (i=0;i!=n;i++){fin>>strs[i];}
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
    int i,j,tmpsum, maxestimation=0;
    bool chosen[60];
    set<int>::iterator index;
    int tmpskeleton[60];
    for (i=0;i<n;i++)
    {
        tmpsum=0;
        for (j=0;j<n;j++) {chosen[j]=false;tmpskeleton[j]=-1;}
        chosen[i] = true;
        for (j=0;j<strs[i].length();j++)
        {
            for (index = alphabet[strs[i][j]-65].begin();index != alphabet[strs[i][j]-65].end();index++)
            {
                if (chosen[*index]) {continue;}
                //cout<<strs[i][j]<<" "<<strs[*index]<<endl;
                chosen[*index] = true;
                tmpskeleton[j] = *index;
                tmpsum++;
                break;
            }
        }
        if (tmpsum>maxestimation) {for (j=0;j<n;j++) {maxestimation=tmpsum;core=i;skeleton[j]=tmpskeleton[j];}}

    }
}
int main()
{
    init();
    set_skeleton();
    plot();
}
// int main()
// {
// 	strs[0] = "ASDF";
// 	strs[1] = "AAAA";
// 	strs[2] = "SP";
// 	strs[3] = "ED";
// 	strs[4] = "FFFF";
// 	int num = 0;
// 	int skl[60][2];
// 	skl[0][0] = 1;
// 	skl[1][0] = 2;
// 	skl[2][0] = 3;
// 	skl[3][0] = -1;
// 	skl[0][1] = 1;
// 	skl[1][1] = 0;
// 	skl[2][1] = 1;
// 	skl[3][1] = 0;
// 	plot_skl_mtrcs(num, skl);
// }

