#include<iostream>
using namespace std;
#define max 110
struct
{
  int r,c;
}man[10000],ho[10000];
int map[max][max],lx[max],ly[max],num_man,num_ho;
bool visx[max],visy[max];
int match[max];
bool dfs(int f)
{
  visx[f]=1;
  for(int i=1;i<num_ho;i++)
    if(!visy[i]&&map[f][i]==lx[f]+ly[i])
      {
        visy[i]=1;
        if(match[i]==-1||dfs(match[i]))
          {
            match[i]=f;
            return 1;
          }
      }
  return 0;
}
int main()
{
  int n,m;
  char c;
  while(cin>>n>>m&&n!=0&&m!=0)
    {
      int i,j,k;
      num_man=1,num_ho=1;
      for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
          {
            cin>>c;
            if(c=='m')
              {
                man[num_man].r=i;
                man[num_man++].c=j;
              }
            if(c=='H')
              {
                ho[num_ho].r=i;
                ho[num_ho++].c=j;
              }
          }
      memset(map,0,sizeof(map));
      memset(ly,0,sizeof(ly));
      memset(match,-1,sizeof(match));
      for(i=1;i<num_man;i++)
        {
          int min=1<<30;
          for(j=1;j<num_ho;j++)
            {
              map[i][j]=abs(man[i].r-ho[j].r)+abs(man[i].c-ho[j].c);
              if(map[i][j]<min)
                {
                  min=map[i][j];
                  lx[i]=min;
                }
            }
        }
      for(i=1;i<num_man;i++)
        {
          while(1)
            {
              memset(visx,0,sizeof(visx));
              memset(visy,0,sizeof(visy));
              if(dfs(i)) break;
              int mmi=1<<30;
              for(j=1;j<num_man;j++)
                if(visx[j])
                  for(k=1;k<num_ho;k++)
                    {
                      if(!visy[k]&&map[j][k]-lx[j]-ly[k]<mmi)
                        {
                          mmi=map[j][k]-lx[j]-ly[k];
                        }
                    }
              for(j=1;j<num_man;j++)if(visx[j])lx[j]+=mmi;
              for(j=1;j<num_ho;j++)if(visy[j])ly[j]-=mmi;
            }
        }
      int sum=0;
      for(i=1;i<num_ho;i++)
        sum+=map[match[i]][i];
      cout<<sum<<endl;
    }
  return 0;
}
