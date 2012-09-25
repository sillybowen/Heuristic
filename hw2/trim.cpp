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
