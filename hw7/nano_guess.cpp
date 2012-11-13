#include <cassert>
#include "nano_guess.h"

NanoGuess::NanoGuess(int teamID, int kmaxNanos)
  : team_id_(teamID), max_k_(kmaxNanos) { }

// Got input from server for all current updates
// Decide best place to put nanomunchers and their orientation preference
void NanoGuess::makeDecision(vector<Location*>*locs,vector<Nano*>*nanos,
    vector<Location> &retloc,vector<Nano>&retnanos) {
  int numOfLocs = locs->size(), maxCanGoLen = -1;
  vector<int> maxOrientSeq;
  Location* pMaxLenGoLoc = NULL;

  for (int i = 0; i < numOfLocs; ++i) {
    int tmpLenCanGo;
    vector<int> tmpTrySeq;
    if (!((*locs)[i]->isOccupied())) {  // unoccupied
      tmpLenCanGo = searchOrientAtALoc((*locs)[i], tmpTrySeq);
      if (tmpLenCanGo > maxCanGoLen) {
        pMaxLenGoLoc = (*locs)[i];
        maxCanGoLen = tmpLenCanGo;
        maxOrientSeq = tmpTrySeq;
      }
    }
  }

  if (pMaxLenGoLoc != NULL) {
    retloc.push_back(*pMaxLenGoLoc);
  }
}

int NanoGuess::recNanoGoWithOrient(Location* startLoc, const vector<int>& orient,
    vector<int>& nanoDirPath, vector<int>& nanoLocIDPath) {
  assert(orient.size() == 4);
  if (startLoc->isOccupied())
    return 0;

  startLoc->tried_ = true;
  nanoLocIDPath.push_back(startLoc->getID());
  int lastDir, i, counter = 0;
  if (nanoDirPath.empty()) {
    lastDir = orient[3];
    i = 0;
  } else {
    lastDir = nanoDirPath.back();
    for (i = 0; i < orient.size(); ++i) {
      if (orient[i] == lastDir) {
        i = (i + 1) % 4;
        break;
      }
    }
  }
  assert(i != orient.size());
  Location* pNextLoc;
  for ( ; counter < 4; i = (i + 1) % 4, ++counter) {
    if (startLoc->getChildren().at(orient[i]) != NULL &&
        !(startLoc->getChildren().at(orient[i])->tried_)) {
      pNextLoc = startLoc->getChildren().at(orient[i]);
      nanoDirPath.push_back(orient[i]);
      break;
    }
  }
  if (counter == 4) {  // No more way to go
    return 1;
  } else {
    return 1 + recNanoGoWithOrient(pNextLoc, orient, nanoDirPath, nanoLocIDPath);
  }
}

int NanoGuess::searchOrientAtALoc(Location* startLoc, vector<int>& tryingSeq) {
  int myints[] = {2, 1, 0, 3};
  vector<int> tmpOrient(myints, myints + sizeof(myints) / sizeof(int));
  vector<int> nanoDirPath;
  vector<int> nanoLocIDPath;
  nanoDirPath.clear();
  nanoLocIDPath.clear();
  int maxLocEaten = recNanoGoWithOrient(startLoc, tmpOrient, nanoDirPath,
      nanoLocIDPath);
  cout << "maxLocEaten = " << maxLocEaten << endl;
  cout << "size of LocPath = " << nanoLocIDPath.size() << endl;
  printDirPath(nanoDirPath, nanoLocIDPath);
  return maxLocEaten;
}

void NanoGuess::printDirPath(const vector<int>& dirPath,
    const vector<int>& locIDPath) {
  assert(locIDPath.size() == (dirPath.size() + 1));
  int i;
  for (i = 0; i < dirPath.size(); ++i) {
    cout << locIDPath[i] << " ->";
    switch (dirPath[i]) {
      case 0: cout << "Up | "; break;
      case 1: cout << "Down | "; break;
      case 2: cout << "Left | "; break;
      case 3: cout << "Right | "; break;
      default: cout << "Error!";
    }
  }
  cout << locIDPath[i] << endl;
}
