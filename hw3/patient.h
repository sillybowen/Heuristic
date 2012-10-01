#ifndef patient_h_
#define patient_h_
#include <cstdlib>
class Position {
 public:
  int x;
  int y;
  Position() {};
 Position(int xx,int yy): x(xx),y(yy){};
  virtual void output() {};
  virtual bool isPatient(){return false;};
  int distance(Position* p) {
    return abs(p->x-x)+ abs(p->y-y);
  };
};
class Hospital:public Position {
 private:
  int id;
  int ambulance_number_;
 public:
  Hospital(int x,int y,int id,int an);
  void output();
  void outputHos();
  bool isPatient();
};
class Patient:public Position  {
 private:
  int resecue_time_;
  int id;
 public:
  Patient(int x,int y,int t,int i);
  void output();
  bool isPatient();
};
#endif
