#ifndef patient_h_
#define patient_h_
class Position {
 public:
  int x;
  int y;
 Position(int xx,int yy): x(xx),y(yy){};
  virtual void  output() = 0;
  virtual bool isPatient() = 0;
};
class Hospital:public Position {
 private:
  int id;
 public:
  Hospital(int x,int y,int id);
  void output();
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
