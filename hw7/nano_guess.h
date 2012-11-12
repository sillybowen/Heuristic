#ifndef nano_guess_h
#define nano_guess_h
#include <vector>
#include "loc.h"
using std::vector;
using std::cout;
using std::endl;

class NanoGuess {
public:
  // Search for a best orientation strategy for puting a NEW Nano at given location
  // Initially, @tryingSeq should be EMPTY, and best orient will be returned from
  // @tryingSeq after call, also the largest Location Nano can eat(NO consideration
  // of Adversaries)
  static int recNanoGoWithOrient(Location* startLoc, const vector<int>& orient,
      vector<int>& nanoDirPath, vector<int>& nanoLocIDPath);
  static int searchOrientAtALoc(Location* startLoc, vector<int>& tryingSeq);

  // This should be consistent with Nano::initializeMap()
  static void printDirPath(const vector<int>& dirPath, const vector<int>& locIDPath);

private:

};

#endif  // nano_guess_h
