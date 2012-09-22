#include<iostream>
#include <fstream>
#include <vector>
#include "matching.h"
using namespace std;

vector<point> p;
void init() {
  ifstream inputfile;
  p.resize(1000);
  inputfile.open("input_travel");
  for (int i = 0; i<1000; i++) {
    int x;
    inputfile>>p[i].id_at_input>>p[i].x>>p[i].y>>p[i].z;
    p[i].id_at_main = i;
  }
  inputfile.close();
}

void work() {
  Mst m_(p);
  m_.work();
  Matching ma_(m_.giveOddDegreePoints());
  ma_.work();

  vector<edge> edgesSet1 = m_.givenAnsEdges();
  vector<edge> edgesSet2 = ma_.givenAnsEdges();
//   Matching ma_(ps);
//  ma_.work();

//  euler();

/*  vector<point> ps;
  point p; 
  p.set(0,0,1,1,11);
  point p2;
  p2.set(1,1,1,1,1);
  point p3;
  p3.set(2,2,1,1,2);
  point p4;
  p4.set(3,3,101,101,101);
  point p5;
  p5.set(4,4,101,101,102);
  point p6;
  p6.set(5,5,101,101,103);
  ps.push_back(p);
  ps.push_back(p2);
  ps.push_back(p3);
  ps.push_back(p4);
  ps.push_back(p5);
  ps.push_back(p6);
  Matching ma_(ps);
  ma_.work();
*/

}

void outit() {
}

int main() {
  init();
  work();
  outit();
}
