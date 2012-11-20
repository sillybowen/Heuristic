#include <cassert>
#include <iostream>
#include <sstream>
#include "server_game.h"

using std::cout;
using std::endl;

ServerGame::ServerGame(const char* plyName, int nFeatures, int numThrs, int srv_port)
          : Game(plyName, nFeatures, numThrs), arch_clt_(registerSrv(srv_port)) { }

ServerGame::~ServerGame() { }

void ServerGame::startGame() {
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
      fromSrv.clear();

      sleep(1);
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

}

bool ServerGame::readSrvOutput(const string& fromSrv) {
  int myteam = 1;

  return true;
}

ClientSocket* ServerGame::registerSrv(int srv_port) {
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
