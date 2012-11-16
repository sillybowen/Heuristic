#ifndef matchmaker_h
#define matchmaker_h
#include "someone.h"

class Matchmaker : public Someone {
public:
  Matchmaker(const string& plyName, int nFeatures);
  ~Matchmaker();

  bool isMatchmaker() const;
  void sendOutVector(double* aVector);

private:
};
#endif  // matchmaker_h
