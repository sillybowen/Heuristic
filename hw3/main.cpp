#include<iostream>
#include<string>
#include<sstream>
#include "patient.h"
#include "ambulance.h"
#include <vector>
#include <algorithm>
using namespace std;
vector<Patient*> patients;
vector<int> ambNumber;
vector<Hospital*> hospitals;
bool comparePos(Position p1, Position p2) {
  if (p1.x<p2.x) return true;
  if (p1.x>p2.x) return false;
  return p1.y<p2.y;
}


bool change(vector<Position> &n, vector<Position> &o) {
  sort(n.begin(),n.end(),comparePos);
  sort(o.begin(),o.end(),comparePos);
  for (int i = 0; i<n.size();i++)
    if (n[i].x!=o[i].x||n[i].y!=o[i].y)
      return true;
  return false;
}

void kmeansToFindHospital() {
  vector<Position> center;
  vector<int> cluster;
  center.resize(ambNumber.size());
  cluster.resize(patients.size());
  int mxx,mxy,mix,miy;
  mxx = patients[0]->x;
  mix = patients[0]->x;
  mxy = patients[0]->y;
  miy = patients[0]->y;
  for (int i = 0; i<patients.size(); i++) {
    if (patients[i]->x>mxx) mxx = patients[i]->x;
    if (patients[i]->x<mix) mix = patients[i]->x;
    if (patients[i]->y>mxy) mxy = patients[i]->y;
    if (patients[i]->y<miy) miy = patients[i]->y;
  }
  for (int i = 0; i<center.size();i++) {
    center[i].x = mix+(mxx-mix)*i/(center.size()-1);
    center[i].y = miy+(mxy-miy)*i/(center.size()-1);
  }
  vector<Position> oldcenter;
  vector<int>count;
  oldcenter.resize(center.size());
  count.resize(center.size());
  for (int i = 0;i<oldcenter.size();i++) {
    oldcenter[i].x = 0;
    oldcenter[i].y = 0;
  }
  while (change(oldcenter,center)) {
    oldcenter = center;
    for (int i = 0; i<center.size();i++){
      center[i].x = 0;
      center[i].y = 0;
      count[i] = 0;
    }
    for (int i = 0; i<patients.size();i++) {
      int c = 0;
      for (int j = 1; j<center.size();j++)
        if (patients[i]->distance(&oldcenter[j])<patients[i]->distance(&oldcenter[c]))
          c = j;
      count[c] ++;
      center[c].x+= patients[i]->x;
      center[c].y+= patients[i]->y;
    }
    for (int i = 0; i<center.size();i++){
      center[i].x/= count[i];
      center[i].y/= count[i];
    }
  }

  int head = 0;
  int tail = ambNumber.size()-1;
  int id = 0;

  while (head<=tail) {
    id ++;
    Hospital * h = new Hospital(center[head].x,center[head].y,id,ambNumber[id-1]);
    hospitals.push_back(h);
    head++;
    if (tail<head) break;
    id ++;
    h = new Hospital(center[tail].x,center[tail].y,id,ambNumber[id-1]);
    hospitals.push_back(h);
    tail--;
  }
  for (int i = 0; i<hospitals.size();i++)
    hospitals[i]->outputHos();
}

void init() {
  string s;
  cin>>s;
  while(s[0]<'0'||s[0]>'9') cin>>s;
  int x,y,t;
  Patient*p;
  int i=0;
  while (s[0]>='0'&& s[0]<='9') {
    stringstream ss(s);
    char c;
    ss>>x>>c>>y>>c>>t;
    i++;
    p = new Patient(x,y,t,i);
    patients.push_back(p);
    cin>>s;
  }
  int ambNum;
  while(cin>>ambNum) {
    ambNumber.push_back(ambNum);
  }
  sort(ambNumber.begin(),ambNumber.end());
}
void work() {
  Hospital * h = new Hospital(0,0,1,1);
  Ambulance amb(h);
  amb.addRountine(patients[0]);
  amb.addRountine(patients[100]);
  amb.addRountine(h);
  amb.output();
  kmeansToFindHospital();
}
void workdone() {
  for (int i = 0; i<patients.size();i++)
    delete patients[i];
}
int main() {
  init();
  work();
  workdone();
}
