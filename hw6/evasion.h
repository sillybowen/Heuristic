#ifndef EVASION_HEADER_
#define EVASION_HEADER_

#include <string>
#include "ClientSocket.h"
#include "SocketException.h"
#include "moveable.h"

using std::string;

class Evasion {
public:
  // Transfer ownership of @moveable to this class
  Evasion(Moveable* moveable, int n, int m, int srv_port);
  ~Evasion();

  int startGame(string& teamName);
  // Listen to socket to get other player's move
  bool otherPlyMove();

  enum game_status { playing, HunterWin, PreyWin, equal };

private:
  void readSrvUpdateStates(const string& fromSrv);
  ClientSocket* registerSrv(int srv_port);

  const int N_;
  const int M_;
  ClientSocket* arch_clt_;
  Moveable* my_obj_;  // Hunter or Prey that I am playing (Controlled by me)
};

#endif
