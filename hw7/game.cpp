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

const string dirArr[4] = { "Up", "Left", "Down", "Right" };

Game::Game(const char* plyName, const char* srvOutFile, int globalMaxK, int srv_port)
  : max_k_(globalMaxK), ply_name_(string(plyName)), inFile_srv_(srvOutFile),
  arch_clt_(registerSrv(srv_port)) { }

void Game::startGame() {
  string fromPly = ply_name_, fromSrv, srvEndMark("))\n");
  fromPly.push_back('\n');

  try {
    (*arch_clt_) << fromPly;  // Send server our player's name

    do {
      string tmpFromSrv;
      do {
        (*arch_clt_) >> tmpFromSrv;
        fromSrv += tmpFromSrv;
      } while (fromSrv.find(srvEndMark) == string::npos);
      cout << "#FromSrv: " << fromSrv << endl;

      readSrvOutput(fromSrv);

      sleep(10);
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

}

void Game::printLocsToSrv(const vector<Location>& newPlacements,
    const vector<Nano>& newNanos) const {
  int numOfNewLocs = newPlacements.size();
  ostringstream ost;
  ost << '(';
  for (int i = 0; i < numOfNewLocs; ++i) {
    ost << "((current_node((id " << newPlacements[i].getID() << ")(loc((x "
      << newPlacements[i].getX() << ")(y " << newPlacements[i].getY()
      << ")))))(last_direction " << dirArr[newNanos[i].getLastDir_()]
      << ")(precedence(" << nanoSeqsToStr(newNanos[i]) << "))(player "
      << ply_name_ << "))";
  }
  ost << ")\n";

  cout << "#ToSrv: " << ost.str() << endl;
  (*arch_clt_) << ost.str();  // Add newline and flush
}

bool Game::readSrvOutput(const string& fromSrv) {
  /*ifstream inF;

  inF.open(inFile_srv_.c_str(), ifstream::in);
  if (!inF.good()) {
    return false;
  }
  vector<char> srvStrVect((istreambuf_iterator<char>(inF)),
      istreambuf_iterator<char>());
  */
  vector<char> srvStrVect(fromSrv.begin(), fromSrv.end());
  vector<char>::const_iterator srvIter = srvStrVect.begin();

  Nano::initializeMap();
  tree srvTr(srvIter);

  // interface to work
  int myteam = 1;
  Parser p(1, max_k_);
  vector<Location> retloc;
  vector<Nano> retNano;
  p.work(&srvTr,retloc,retNano);
  //ret are the location to deploy new nanomuncher
  /* Tests
  retloc.push_back(Location(0, 12, 5));
  retloc.push_back(Location(6, 14, 4));
  vector<string> dir;
  dir.push_back("Down");
  dir.push_back("Left");
  dir.push_back("Right");
  dir.push_back("Up");
  retNano.push_back(Nano(111, "Up", dir, true, 1));
  retNano.push_back(Nano(112, "Left", dir, true, 1));
  */
  printLocsToSrv(retloc, retNano);

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

string Game::nanoSeqsToStr(const Nano& aNano) const {
  ostringstream ost;
  for (int i = 0; i<4; ++i) {
    ost << dirArr[aNano.getNanoSeqs().at(i)];
    if (i != 3)
      ost << " ";
  }
  return ost.str();
}
