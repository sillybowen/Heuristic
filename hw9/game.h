#ifndef game_h
#define game_h
#include <vector>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "gamble.h"
#include "ClientSocket.h"
#include "SocketException.h"

using std::string;
using std::vector;

class Game {
public:
  Game(const char* plyName, int srv_port, int mode_);
  ~Game() { }
  void startGame();

private:
  int           mode;
  ClientSocket* arch_clt_;
  const string  ply_name_;  // Player I am in control
  string        inFile_srv_;  // input file from server (server send messages here)
  vector<Gamble*> gambles;
  vector<Link*> links;
  ClientSocket* registerSrv(int srv_port);
  void          readSrvOutFile();
};

#endif  // game_h
