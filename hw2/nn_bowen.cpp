#include "nn_bowen.h"

using namespace std;

NN_Bowen::NN_Bowen(vector<point> p) {
  for (int i = 0; i<p.size(); i++) 
    points.push_back(p[i]);
}
void NN_Bowen::work() {
  vector<bool> visited;
  visited.resize(points.size());
  for (int i = 0;i<visited.size();i++)
    visited[i] = false;
  ans.push_back(0);
  visited[0] = true;
  for (int i = 1; i<points.size();i++) {
    int np = -1;
    for (int j = 1; j<points.size();j++)
      if (!visited[j]) {
	if (np == -1 || 
	    points[ans[i-1]].dis(points[j])
	    <points[ans[i-1]].dis(points[np]))
	  np = j;
      }   
    ans.push_back(np);
    visited[np] = true;
  }
  ans.push_back(0);
}
vector<int> NN_Bowen::giveResult() {
  return ans;
}
