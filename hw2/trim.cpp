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
  bool r = true;
  while (r) {
    r = false;
  for (int i = 1; i<eulerOutput.size()-1; i++)
    for (int j = i+3; j<eulerOutput.size()-1; j++) 
      if (eulerOutput[i] == eulerOutput[j]) {
	if ( dis(i-1,i+1)+ dis(j-1,j+1) > dis(i-1,j-1)+dis(i+1,j+1)) {
	  revert(i+1,j-1);
	  r  = true;
	}
      }
  }
}

vector<int> Trim::work3(vector<int> currentSol) {
  bool findBetter = true;
  vector<int> ret;
  vector<bool> tried;
  tried.resize(currentSol.size());
  for (int i = 0; i<currentSol.size(); i++)
    ret.push_back(currentSol[i]);

  while (findBetter) {
    findBetter = false;
    for (int i = 0; i<ret.size();i++)
      tried[i] = false;
    double maxD=-1;
    double maxReduce=0;
    double curReduce;
    double removeCost;
    int f;
    int newPos;
    while (!findBetter) {
      maxD = -1;
      for (int i = 1; i<ret.size()-1;i++) 
	if (!tried[i]&&
	    (points[ret[i-1]].dis(points[ret[i]])+
	     points[ret[i+1]].dis(points[ret[i]]))>maxD) {
	  maxD = points[ret[i-1]].dis(points[ret[i]])+
	    points[ret[i+1]].dis(points[ret[i]]);
	  f = i;
	}
      if (maxD == -1) break;
      maxReduce=0;
      removeCost = points[ret[f-1]].dis(points[ret[f+1]]) - maxD;
      for (int i = 1;i<ret.size();i++)
	if (i!=f && i!=f+1) {
	  curReduce = points[ret[f]].dis(points[ret[i-1]]) +
	    points[ret[f]].dis(points[ret[i]]) - 
	    points[ret[i]].dis(points[ret[i-1]]) +
	    removeCost;
	  if (curReduce<maxReduce) {
	    maxReduce = curReduce;
	    newPos = i;
	  }
	}
      tried[f] = true;
      if (maxReduce<0) {
	findBetter = true;
	int tmp = ret[f];
	if (f>newPos) {
	  for (int i = f; i>newPos;i--)
	    ret[i] = ret[i-1];
	  ret[newPos] = tmp;
	}
	else {
	  for (int i = f;i<newPos-1;i++)
	    ret[i] = ret[i+1];
	  ret[newPos-1] = tmp; 
	}
      }
    }
  }
  return ret;
}
