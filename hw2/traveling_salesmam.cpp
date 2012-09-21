#include<iostream>
#include <fstream>
#include <vector>
#include "mst.h"
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
  // Matching ma_(ps);
//  ma_.work();

//  euler();
}

void outit() {
}

int main() {
  init();
  work();
  outit();
}
