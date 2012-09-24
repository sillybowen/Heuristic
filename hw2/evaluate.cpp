
#include "evaluate.h"

using namespace std;

Evaluate::Evaluate (vector<point>& p, int startVertex)
  : startVertex_(startVertex) {
  for (int i = 0; i<p.size(); i++) 
    points.push_back(p[i]);
};

double Evaluate::evaluate (vector<int> seq) {
  vector<bool> b;
  b.resize(points.size());
  for (int i = 0; i<b.size();i++)
    b[i] = false;
  double ans = 0;
  if (seq[0] != startVertex_ || seq[seq.size()-1]!= startVertex_)
    return -1;
  for (int i = 0; i<seq.size();i++) {
    cout<<i<<endl;
    cout<<"seq"<<seq[i]<<endl;
    b[seq[i]] = true;
    if (i > 0) {
      ans += points[seq[i]].dis(points[seq[i-1]]);
    }
  }
  for (int i = 0; i<b.size();i++) {
    if (!b[i]) {
      cout<<"dont have"<<i<<endl;
      return -2;
    }
  }
  return ans;
};
