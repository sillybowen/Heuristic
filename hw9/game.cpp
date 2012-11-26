#include <cassert>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
using namespace std;

Game::Game(const char* plyName, int srv_port, int mode_)
  : ply_name_(string(plyName)), arch_clt_(registerSrv(srv_port)), mode(mode_) { }

void Game::startGame() {
  string fromPly = ply_name_, fromSrv, srvEndMark("]");
  fromPly.push_back('\n');

  try {
    (*arch_clt_) << fromPly;  // Send server our player's name

    do {
      string tmpFromSrv;
      (*arch_clt_) >> fromPly;

      // Get File Path / mode / num_rounds
      string file_path, mode, num_str;
      int num_rounds;
      stringstream ss;
      ss << fromPly;
      ss >> file_path >> mode >> num_str;
      num_rounds = atoi(num_str.c_str());

      cout << "File Path : " << file_path << endl << "Mode : " << mode << endl << "# of rounds : " << num_rounds << endl << endl;
      while(true) {
        (*arch_clt_) >> tmpFromSrv;
        fromSrv += tmpFromSrv;

	if((fromSrv.find("[")!=string::npos && fromSrv.find("]")!=string::npos) ||
	   (fromSrv.find("[")==string::npos && fromSrv.find("]")==string::npos))
	  break;
      }
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
