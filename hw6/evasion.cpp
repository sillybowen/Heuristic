#include <cassert>
#include <iostream>
#include <sstream>
#include "evasion.h"

using std::stringstream;

Evasion::Evasion(Moveable* moveable, int n, int m, int srv_port)
    : N_(n), M_(m), my_obj_(moveable), arch_clt_(registerSrv(srv_port)) {
}

Evasion::~Evasion() {
  delete my_obj_;
}

int Evasion::startGame(string& teamName) {
  int gameResult = playing;
  string fromSrv, fromPly;
  teamName.push_back('\n');
  
  try {
    (*arch_clt_) << teamName;

    do {
      (*arch_clt_) >> fromSrv;
      if (fromSrv.empty() || fromSrv.compare("Bye") == 0)
        break;
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

  return gameResult;
}


void Evasion::readSrvUpdateStates(const string& fromSrv) {
  stringstream ss;
  ss << fromSrv;

  int tmp, wt, gamePos, ind = 0;
  while ((tmp = ss.get()) != '|');

  while (ss.peek() != '|') {
    ss >> wt;
    assert(ss.get() == int(','));
    ss >> gamePos;

    while (ind < (gamePos + board_len_ / 2))
      fl_board_[ind++] = 0;

    fl_board_[gamePos + board_len_ / 2] = wt;

    ++ind;
  }

  // Sum up to get f1crum and f2crum values
  f1_val_ = 0;
  f2_val_ = 0;
  for (int i = 0; i <= board_len_; ++i) {
    f1_val_ += (f1_pos_ - i) * fl_board_[i];
    f2_val_ += (f2_pos_ - i) * fl_board_[i];
  }
}

// Register with the architecture server
ClientSocket* Evasion::registerSrv(int srv_port) {
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
