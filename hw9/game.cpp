#include <cassert>
#include <fstream>
#include <sstream>
#include "game.h"
using namespace std;

Game::Game(const char* plyName, int srv_port, int mode_)
  : ply_name_(string(plyName)), arch_clt_(registerSrv(srv_port)), mode(mode_) { }

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

      fromSrv.clear();

      sleep(1);
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

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

string Game::readSrvOutFileToStr() const {  // Default file is @inFile_srv_
  ifstream inF;
  inF.open(inFile_srv_.c_str(), ifstream::in);
  if (!inF.good()) {
    return string();
  }
  return string((istreambuf_iterator<char>(inF)), istreambuf_iterator<char>());
}
