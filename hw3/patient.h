#ifndef patient_h_
#define patient_h_
#include <cstdlib>
#include <string>
using namespace std;
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
  void setId(int i) {id = i;}
  int getAmbNumber() {
    return ambulance_number_;
  };
};
class Patient:public Position  {
 private:
  int resecue_time_;
  int id;
  bool saved;
  Hospital* nearest_hospital_;
 public:
  Patient(int x,int y,int t,int i);
  void output();
  bool isPatient();
  void resetSaved() {
    if (distance(nearest_hospital_)!=0)
      saved = false;
  }
  void setNearestHospital(Hospital* h) {nearest_hospital_ = h;};
  Hospital* getNearestHospital() { return nearest_hospital_;};
  void setSaved() { saved = true;}
  int disToNH() {return distance(nearest_hospital_);}
  bool getSaved() { return saved;}
  int getRescueTime() {return resecue_time_;}
  Patient* copy() {
    Patient* p = new Patient(x,y,resecue_time_,id);
    p->setNearestHospital(nearest_hospital_);
  }
};
#endif
