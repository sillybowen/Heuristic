#include "trim.h"
#include <iostream>
#include <algorithm>
#include "evaluate.h"
using namespace std;
Trim::Trim(const vector<point>& p, list<int>* e,vector<double>*d) : numOfPoints_(p.size()) {
  points.resize(p.size());
  for (int i = 0; i<p.size();i++)
    points[i] = p[i];
  list<int>::iterator it = e-> begin();
  while (it!=e->end()) {
    eulerOutput.push_back(*it);
    it++;
  }
  distance = d;
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
  for (int i = h; i<=(t+h)/2;i++) {
    p = eulerOutput[i];
    eulerOutput[i] = eulerOutput[t-(i-h)];
    eulerOutput[t-(i-h)] = p;
  }
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
	  //	  revert(i+1,j-1);
	  reverse(eulerOutput.begin()+i+1,eulerOutput.begin()+j);
	  r  = true;
	}
      }
  }
}

vector<int> Trim::work3(vector<int> currentSol) {
  bool findBetter = true;
  vector<int> ret;
  vector<bool> tried;
  int count = 0;
  std::cout<<"work3"<<endl;
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
	    ((*distance)[ret[i-1]*numOfPoints_+ret[i]] +
	     (*distance)[ret[i+1]*numOfPoints_+ret[i]]
	     )>maxD) {
	  maxD = (*distance)[ret[i-1]*numOfPoints_+ret[i]] + 
	    (*distance)[ret[i+1]*numOfPoints_+ret[i]];
	  f = i;
	}
      if (maxD == -1) break;
      maxReduce=0;
      removeCost = (*distance)[ret[f-1]*numOfPoints_+ret[f+1]] - maxD;
      for (int i = 1;i<ret.size();i++)
	if (i!=f && i!=f+1) {
	  curReduce = 
	    (*distance) [ret[f]*numOfPoints_+ret[i-1]] +
	    (*distance) [ret[f]*numOfPoints_+ret[i]] -
	    (*distance) [ret[i]*numOfPoints_+ret[i-1]] +
	    removeCost;
	  if (curReduce<maxReduce) {
	    maxReduce = curReduce;
	    newPos = i;
	  }
	}
      tried[f] = true;
      if (maxReduce<0) {
	findBetter = true;
	updated = true;
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
vector<int> Trim::swapHeu(vector<int>v) {
  bool r = true;
  vector<int> ans = v;
  int count = 0;
  std::cout<<"inswap"<<endl;
  Evaluate e(points,0);
  cout<<e.evaluate(ans)<<endl;
  while (r) {
    r = false;
    for (int i = 1; i<ans.size()-1; i++)
      for (int j = i+2; j<ans.size()-1; j++) {
	double d1 = (*distance)[ans[i]*numOfPoints_+ans[i-1]] +
		     (*distance)[ans[i]*numOfPoints_+ans[i+1]] +
		     (*distance)[ans[j]*numOfPoints_+ans[j-1]] +
		     (*distance)[ans[j]*numOfPoints_+ans[j+1]];

	double d2 = (*distance)[ans[i]*numOfPoints_+ans[j+1]] +
	  (*distance)[ans[i]*numOfPoints_+ans[j-1]] +
	  (*distance)[ans[j]*numOfPoints_+ans[i+1]] +
	  (*distance)[ans[j]*numOfPoints_+ans[i-1]] ;
	if ( d2<d1) {
	  cout<<"dd"<<d1<<' '<<d2<<endl;	
	  cout<<i<<' '<<j<<endl;
	  //  cout<<ans[i]<<' '<<ans[j]<<endl;
	  int p = ans[j];
	  ans[j] = ans[i];
	  ans[i] = p;
	  cout<<e.evaluate(ans)<<endl;
	  //  cout<<ans[i]<<' '<<ans[j]<<endl;
	  r  = true;
	  updated = true;
	}
      }
    cout<<e.evaluate(ans)<<endl;
  }
  return ans;
}
vector<int> Trim::strongRevert(vector<int> v) {
  bool r = true;
  vector<int> ans = v;
  int count = 0;
  std::cout<<"strongRever"<<endl;
  while (r) {
    r = false;
    for (int i = 1; i<ans.size()-1; i++)
      for (int j = i+1; j<ans.size()-1; j++) 
	if (( (*distance)[ans[i]*numOfPoints_+ans[i-1]] 
	      + (*distance)[ans[j]*numOfPoints_+ans[j+1]])
	    > ( (*distance)[ans[i]*numOfPoints_+ans[j+1]] 
		+ (*distance)[ans[j]*numOfPoints_+ans[i-1]])) {
	  reverse(ans.begin()+i,ans.begin()+j+1);
	  r  = true;
	  updated = true;
	}
  }
  return ans;
}

vector<int> Trim::work4(vector<int> v) {
  updated = true;
  vector<int> tmp1,tmp,ans;
  ans = v;
  int count = 0;
  while (updated)  {
    cout<<"count"<<count++<<endl;;
    updated = false;
    tmp = ans;
    tmp = strongRevert(tmp);
    tmp = swapHeu(tmp);
    ans = work3(tmp);
  }
  return ans;
}
