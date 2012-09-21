#include "matching.h"
#include <iostream>
using namespace std;
Matching::Matching (vector<point> p) {
  points.resize(p.size());
  for (int i = 0; i<p.size();i++)
    points[i] = p[i];
};
void Matching::work() {
  createEdge();
  KM();
  output();
};


/*其实在求最大 最小的时候只要用一个模板就行了,把边的权值去相反数即可得到另外一个.求结果的时候再去相反数即可*/
/*最大最小有一些地方不同。。*/
//赤裸裸的模板啊。。
bool Matching::find(int x)
{
  visx[x] = true;
  for(int y = 0; y < ny; y++)
    {
      if(visy[y])
        continue;
      double t = lx[x] + ly[y] - w[x][y];
      if(t==0)
        {
          visy[y] = true;
          if(linky[y]==-1 || find(linky[y]))
            {
              linky[y] = x;
              return true;        //找到增广轨
            }
        }
      else if(slack[y] > t)
        slack[y] = t;
    }
  return false;                   //没有找到增广轨（说明顶点x没有对应的匹配，与完备匹配(相等子图的完备匹配)不符）
}

int Matching::KM()                //返回最优匹配的值
{
  int i,j;
  memset(linky,-1,sizeof(linky));
  memset(ly,0,sizeof(ly));
  for(i = 0; i < nx; i++)
    for(j = 0,lx[i] = -1; j < ny; j++)
      if(w[i][j] > lx[i])
        lx[i] = w[i][j];
  for(int x = 0; x < nx; x++)
    {
      for(i = 0; i < ny; i++)
        slack[i] = INF;
      while(true)
        {
          memset(visx,0,sizeof(visx));
          memset(visy,0,sizeof(visy));
          if(find(x))                     //找到增广轨，退出
            break;
          double d = INF;
          for(i = 0; i < ny; i++)          //没找到，对l做调整(这会增加相等子图的边)，重新找
            {
              if(!visy[i] && d > slack[i])
                d = slack[i];
            }
          for(i = 0; i < nx; i++)
            {
              if(visx[i])
                lx[i] -= d;
            }
          for(i = 0; i < ny; i++)
            {
              if(visy[i])
                ly[i] += d;
              else
                slack[i] -= d;
            }
        }
    }
  int result = 0;
  for(i = 0; i < ny; i++)
    if(linky[i]>-1)
      result += w[linky[i]][i];
  return result;
}

void Matching::output() {
  for (int i = 0; i<nx; i++)
    cout<<linky[i]<<' '<<i<<endl;
}
void Matching::createEdge() {
/*  nx = points.size();
  ny = points.size();
  double maxx;
  for (int i = 0; i< points.size();i++)
    for (int j = 0; j<points.size();j++) {
      w[i][j]=points[i].dis(points[j]);
      if (w[i][j]>maxx)
        maxx = w[i][j];
    }
  for (int i = 0; i<points.size();i++)
    for (int j = 0; j<points.size();j++)
      if (i!=j){
        w[i][j] = maxx+1 - w[i][j];
        }*/

  for (int i = 0; i<points.size();i++)
    for (int j = i+1; j< points.size();j++)
      {
        
      }
}
void Matching::greedy() {
  
}
