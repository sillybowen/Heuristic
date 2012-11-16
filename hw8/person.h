#ifndef person_h
#define person_h
#include <vector>
using std::vector;

class Person {
public:
  Person(int nFeatures);
  virtual ~Person();

  virtual void getExactW(double* wVector);
  virtual void getWChgVector(double* wChgVector, int index);

  // For testing
  void printExactW() const;

protected:
  const int       n_features_;
  int             cur_his_ind_;  // The index to insert NEW history into @wChg_his_
  double*         exact_w_;   // Secret original W vector
  vector<double*> wChg_his_;  // Record every changed W vector sent
};

#endif  // person_h
