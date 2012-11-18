#ifndef someone_h
#define someone_h
#include <cassert>
#include <string>
#include <vector>
using std::string;
using std::vector;

// Abstract class for class Person and Matchmaker
class Someone {
public:
  Someone(const string& plyName, int nFeatures)
         : n_features_(nFeatures), cur_his_ind_(0), ply_name_(plyName) {
    vect_his_.reserve(20);
  }
  virtual ~Someone() {
    for (int i = 0; i < vect_his_.size(); ++i)
      delete [] vect_his_[i];
    vect_his_.clear();
  }

  virtual void feedRandCandsResults(const double* const* xxMatr,
      const vector<double>& scores, int numOfCands, double eta = 0.001,
      int leaveInd = -1, double* retGuessW = NULL) const {
    assert(this->isMatchmaker());  // Do NOT allow Person objects to call this method
  }
  virtual bool isMatchmaker() const = 0;
  // This is the exact W / noise vector if Someone is a @Person
  // or candidate x vector if Someone is a @Matchmaker
  // Sizeof @aVector is always @n_features_
  virtual void sendOutVector(double* aVector) = 0;
  const string& getPlyName() const { return ply_name_; }

protected:
  const int       n_features_;
  int             cur_his_ind_;  // The index to insert NEW history into @vect_his_
  const string    ply_name_;
  // Record every vector has been sent by sendOutVector()
  // For @Person, vect_his_[0] is exact W vector, vect_his_[1..20] are 20 diff noises
  // For @Matchmaker, vect_his_[0..19] are 20 diff candidates
  vector<double*> vect_his_;
};

#endif  // someone_h
