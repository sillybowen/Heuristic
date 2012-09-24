#ifndef NN_gready_h_
#define NN_gready_h_
#include <vector>
#include <iostream>
#include <algorithm>
#include "basic_geo.h"

using namespace std;

struct point_NN{
  point p;
  int conn_p[2];

  bool conn_empty(){
    bool flag = false;
    if(conn_p[0] == -1 || conn_p[1] == -1){
      flag =  true;
    }
    return flag;
  }

  bool input_conn_p(int p){
    if(conn_p[0] == -1)
      conn_p[0]=p;
    else if(conn_p[1] == -1)
      conn_p[1]=p;
    else
      return false;
    return true;
  }
};

class NN{
 private:
  vector<point_NN> points;
  vector<edge> edges;
  vector<edge> ans_edges;
  vector<point>ans_sequence;
  void createEdges();
  void findNN();
 public:
  NN(vector<point> p);
  void work();
  vector<point> give_result();
};
#endif
