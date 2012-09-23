#include "matching.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include "match/GEOM/GeomPerfectMatching.h"
using namespace std;

Matching::Matching (vector<point> p) {
  points.resize(p.size());
  for (int i = 0; i<p.size();i++)
    points[i] = p[i];
};
void Matching::work() {
  createEdge();
  greedy();
};
void Matching::work2() {
  createEdge();
  struct PerfectMatching::Options options;
  PerfectMatching *pm = new PerfectMatching(points.size(),edges.size());
  for (int i = 0; i<edges.size();i++) {
    pm->AddEdge(edges[i].p[0],edges[i].p[1],ceil(edges[i].dis));
  }

  pm->options = options;
  pm->Solve();
  minCost = 0;
  for (int i=0; i<points.size(); i++) {
    int j = pm->GetMatch(i);
    if (i < j) {
      double len = points[i].dis(points[j]);
      edge e(i,j,len);
      minCost+= len;
      ans_edges.push_back(e);
    }
  }
  delete pm;
};

void Matching::output() {
  cout<<"matching:"<<endl;
  for (int i = 0; i<ans_edges.size(); i++)
    cout<<ans_edges[i].p[0]<<' '<<ans_edges[i].p[1]<<" "<<ans_edges[i].dis<<endl;
};
void Matching::createEdge() {
  for (int i = 0; i<points.size();i++)
    for (int j = i+1; j< points.size();j++)
      {
        edge e(i,j,points[i].dis(points[j]));
        edges.push_back(e);
      }
  sort(edges.begin(),edges.end(),comparee);
  visited.resize(points.size());
  for (int i = 0; i< points.size();i++)
    visited[i] = false;
}
void Matching::greedy() {
  int left = points.size();
  int index = 0;
  greedyCost = 0;
  while (left>0) {
    if (!visited[edges[index].p[0]]&& !visited[edges[index].p[1]]) {
      ans_edges.push_back(edges[index]);
      visited[edges[index].p[0]] = true;
      visited[edges[index].p[1]] = true;
      left -= 2;
      greedyCost+= edges[index].dis;
    }
    index ++;
  }
}

vector<edge> Matching::givenAnsEdges() {
  for (int i = 0; i<ans_edges.size();i++) {
    ans_edges[i].p[0] = points[ans_edges[i].p[0]].id_at_main;
    ans_edges[i].p[1] = points[ans_edges[i].p[1]].id_at_main;
  }
  return ans_edges;
}

void Matching::outCost() {
//  cout<<"greedy cost"<<greedyCost<<endl;
  cout<<"Matching cost:"<<minCost<<endl;
}
