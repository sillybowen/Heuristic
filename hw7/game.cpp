#include <cassert>
#include <fstream>
#include <sstream>
#include "parser.h"
#include "loc.h"
#include "tree.h"
#include "nano.h"
#include "nano_guess.h"
#include "game.h"

using namespace std;

Game::Game(const char* plyName, const char* srvOutFile, int globalMaxK, int srv_port)
  : max_k_(globalMaxK), ply_name_(string(plyName)), inFile_srv_(srvOutFile),
  arch_clt_(registerSrv(srv_port)) { }

void Game::startGame() {
  string fromPly = ply_name_;
  fromPly.push_back('\n');

  (*arch_clt_) << fromPly;  // Send server our player's name
  readSrvOutput();

  sleep(1000);
}

void Game::printLocsToSrv(const vector<Location>& newPlacements) const {
  int numOfNewLocs = newPlacements.size();
  ostringstream ost;
  ost << '(';
  for (int i = 0; i < numOfNewLocs; ++i) {
    ost << "((current_node((id " << newPlacements[i].getID() << ")(loc((x "
      << newPlacements[i].getX() << ")(y " << newPlacements[i].getY()
      << ")))))(last_direction " << "Right" << ")(precedence("
      << "Up Down Left Right" << "))(player " << ply_name_ << "))";
  }
  ost << ")\n";

  (*arch_clt_) << ost.str();  // Add newline and flush
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
  vector<Location> retloc;
  vector<Nano> retNano;
  p.work(&srvTr,retloc,retNano);
  //ret are the location to deploy new nanomuncher
  //  ret.push_back(Location(0, 12, 5));
  //  printLocsToSrv(ret);

  // vector<int> tryseq;
  // NanoGuess::searchOrientAtALoc(p.getLocations().at(0), tryseq);

  return true;
}
ClientSocket* Game::registerSrv(int srv_port) {
  ClientSocket* pClt_socket = NULL;

  try {
    pClt_socket = new ClientSocket("localhost", srv_port);
  } catch (SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << " PortNum: "
      << srv_port << std::endl;
    assert(0);
  }

  return pClt_socket;
}
