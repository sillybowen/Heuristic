#ifndef patient_h_
#define patient_h_
#include <cstdlib>
#include <vector>

using std::vector;

class Position {
 public:
  int x;
  int y;
  Position() {};
 Position(int xx,int yy): x(xx),y(yy){};
  virtual void output() {};
  virtual void outputValues(vector<int>&) { }
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
  void outputValues(vector<int>&);
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
  void outputValues(vector<int>&);
  bool isPatient();
  void setNearestHospital(Hospital* h) {nearest_hospital_ = h;};
  Hospital* getNearestHospital() { return nearest_hospital_;};
  void setSaved() { saved = true;}
  bool getSaved() { return saved;}
  int getRescueTime() {return resecue_time_;}
};
#endif
