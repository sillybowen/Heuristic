#ifndef person_h
#define person_h
#include "someone.h"

class Person : public Someone {
public:
  Person(const string& plyName, int nFeatures);
  virtual ~Person();

  static int seed_;  // For srand in the following function
  static void randWeightsGenerator(double* aVector, int len);

  bool isMatchmaker() const;
  void sendOutVector(double* aVector);
  void gotValueForJustSentCand(double newValue);

  // For testing
  void printExactW() const;
  const double* getExactW_() const { return exact_w_; }

private:
  void getExactW(double* wVector);
  void getWChgVector(double* wChgVector, int index);
  bool addNewVectHistory(const double* aVector);  // New memory allocated
  bool addNewVectHistory();  // Add new Noise array

  double*         exact_w_;   // Secret original W vector
};

#endif  // person_h
