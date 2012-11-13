#include <fstream>
#include "parser.h"
#include "loc.h"
#include "tree.h"
#include "nano.h"
#include "nano_guess.h"
#include "game.h"

using namespace std;

void Game::startGame() {
  readSrvOutput();
}

bool Game::readSrvOutput() {
  ifstream inF;

  inF.open(inFile_srv_.c_str(), ifstream::in);
  if (!inF.good()) {
    return false;
  }
  vector<char> srvStrVect((istreambuf_iterator<char>(inF)),
      istreambuf_iterator<char>());
  vector<char>::const_iterator srvIter = srvStrVect.begin();

  Nano::initializeMap();
  tree srvTr(srvIter);
  inF.close();
  // interface to work
  int myteam = 1;
  Parser p(1, max_k_);
  vector<Location> ret;
  ret = p.work(&srvTr);
  //ret are the location to deploy new nanomuncher

  // vector<int> tryseq;
  // NanoGuess::searchOrientAtALoc(p.getLocations().at(0), tryseq);

  return true;
}
