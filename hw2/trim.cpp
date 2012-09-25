#include "trim.h"
#include <iostream>
using namespace std;
Trim::Trim(vector<point> p, list<int>* e) {
  points.resize(p.size());
  for (int i = 0; i<p.size();i++)
    points[i] = p[i];
  list<int>::iterator it = e-> begin();
  while (it!=e->end()) {
    eulerOutput.push_back(*it);
    it++;
  }
}
void Trim::work2() {
  prework();
  work();
}
void Trim::work() {
  for (int i = 1; i<eulerOutput.size()-1;i++)
    if (eulerOutput[i] == 0)
      eulerOutput[i] = -1;

 for (int i = 1; i<points.size();i++) {
   int keep = pick(i);
   for (int j = 0; j<eulerOutput.size();j++) 
     if (j!=keep&& eulerOutput[j] == i) {     
       eulerOutput[j] = -1;
     }
  }
 for (int i = 0; i< eulerOutput.size();i++)
   if (eulerOutput[i] != -1)
     ansSeq.push_back(eulerOutput[i]);
}

vector<int> Trim::giveResult() {
  return ansSeq;
}

int Trim::pick(int x) {
  int ans = -1;
  for (int i = 0; i< eulerOutput.size();i++)
    if (eulerOutput[i] == x)
    if (ans == -1 || removeCost(i)<removeCost(ans))
      ans = i;
  return ans;  
}

double Trim::removeCost(int x) {
  double ans;
  ans = points[eulerOutput[x-1]].dis( points[eulerOutput[x]]) +
    points[eulerOutput[x]].dis(points[eulerOutput[x+1]]) -
    points[eulerOutput[x-1]].dis(points[eulerOutput[x+1]]);
  return ans;
}
void Trim::revert(int h,int t) {
  int p;
  //  cout<<"revert"<<endl;
  //  for (int i = h;i<=t;i++)
  //  cout<<eulerOutput[i]<<' ';
  // cout<<endl;
  for (int i = h; i<=(t+h)/2;i++) {
    p = eulerOutput[i];
    eulerOutput[i] = eulerOutput[t-(i-h)];
    eulerOutput[t-(i-h)] = p;
  }
  
  //  for (int i = h;i<=t;i++)
  //    cout<<eulerOutput[i]<<' ';
  //  cout<<endl;
}
double Trim::dis(int i,int j) {
  return
  points[eulerOutput[i]].dis(points[eulerOutput[j]]);
}
void Trim::prework() {
  for (int i = 1; i<eulerOutput.size()-1; i++)
    for (int j = i+3; j<eulerOutput.size()-1; j++) 
      if (eulerOutput[i] == eulerOutput[j]) {
	if ( dis(i-1,i+1)+ dis(j-1,j+1) > dis(i-1,j-1)+dis(i+1,j+1))
	  revert(i+1,j-1);
      }
}
