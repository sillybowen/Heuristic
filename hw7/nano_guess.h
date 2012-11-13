#ifndef nano_guess_h
#define nano_guess_h
#include <vector>
#include "loc.h"
#include "nano.h"
using std::vector;
using std::cout;
using std::endl;

class NanoGuess {
public:
  NanoGuess(int teamID, int kmaxNanos);

  void makeDecision(vector<Location*>*locs,vector<Nano*>*nanos,
      vector<Location> &retloc,vector<Nano>&retnanos);
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
  const int team_id_;
  const int max_k_;
};

#endif  // nano_guess_h
