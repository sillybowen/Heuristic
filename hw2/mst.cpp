#include "mst.h"
#include <iostream>
#include <algorithm>
using namespace std;
Mst::Mst(vector<point> p) {
  points.resize(p.size());
  for (int i = 0; i<p.size();i++)
    points[i] = p[i];
};
int Mst::getRoot(int x) {
  if (root[x] != x)
    root[x] = getRoot(root[x]);
  return root[x];
};
void Mst::merge(int x,int y) {
  root[getRoot(x)] = getRoot(y);
};
void Mst::findMst() {
  root.resize(points.size());
  for (int i = 0; i<points.size();i++)
    root[i] = i;
  int nodes = points.size();
  int index = 0;
  while (nodes>1) {
    if (getRoot(edges[index].p[0])!=getRoot(edges[index].p[1])) {
      merge(edges[index].p[0],edges[index].p[1]);
      ans_edges.push_back(edges[index]);
      nodes --;
    }
    index ++;
  }
};
void Mst::calculateDegree() {
  degree.resize(points.size());
  for (int i = 0; i<degree.size();i++)
    degree[i] = 0;
  for (int i = 0; i<ans_edges.size();i++) {
    degree[ans_edges[i].p[0]]++;
    degree[ans_edges[i].p[1]]++;
  }
};
void Mst::work() {
  createEdges();
  findMst();
  calculateDegree();
};
void Mst::createEdges() {
  for (int i = 0; i<points.size();i++)
    for (int j = i+1; j<points.size();j++) {
      edge e(i,j, points[i].dis(points[j]));
      edges.push_back(e);
    }
  sort(edges.begin(),edges.end(),comparee);
};
vector<point>  Mst::giveOddDegreePoints() {
  vector<point> ret;
  for (int i = 0; i< degree.size();i++)
    if (degree[i]%2==1)
      ret.push_back(points[i]);
  return ret;
};

vector<edge> Mst::givenAnsEdges() {
  return ans_edges;
}
void Mst::output() {
  cout<<"MST"<<endl;
  for (int i = 0; i<ans_edges.size();i++)
    cout<<ans_edges[i].p[0]<<' '<<ans_edges[i].p[1]<<endl;
}
