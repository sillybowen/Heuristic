#ifndef nano_
#define nano_
#include <vector>
#include <string>
#include <map>
#include "loc.h"
using namespace std;
class Nano {
 private:
  static map<string,int> dirMap_;
  std::vector<int> seq_;
  bool live_;
  int id_;
  int belong_;
  int lastDir_;
 public:
  Nano (int id,string lastDir_,vector<string> seq, bool live, int team);
  static void initializeMap () ;
  static int lookFor(string);
  void output();
  static void printDirPath(const vector<int>& dirPath);

  // Search for a best orientation strategy for puting a NEW Nano at given location
  // Initially, @tryingSeq should be EMPTY, and best orient will be returned from
  // @tryingSeq after call, also the largest Location Nano can eat(NO consideration
  // of Adversaries)
  static int recNanoGoWithOrient(Location* startLoc, const vector<int>& orient,
      vector<int>& nanoDirPath);
  static int searchOrientAtALoc(Location* startLoc, vector<int>& tryingSeq);
};
#endif  // nano_
