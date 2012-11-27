#ifndef game_h
#define game_h
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "gamble.h"
#include "engine.h"
#include "init_choice.h"
#include "ClientSocket.h"
#include "SocketException.h"

using std::string;
using std::vector;

class Gamble;

class Game {
public:
  Game(const char* plyName, int srv_port, int mode_);
  ~Game() { }
  void startGame(int user);
  int readSrvGamebleReturns(const string& fromSrv, vector<int>& roundRets,
      double& totalAssets) const;
  string convertBettingListToString(vector<double> betting_list_);

private:
  ClientSocket* registerSrv(int srv_port);
  void          readSrvOutFile();

  int             mode_;
  ClientSocket*   arch_clt_;
  const string    ply_name_;  // Player I am in control
  string          inFile_srv_;  // input file from server (server send messages here)
  Engine          engine_;  // Bowen's engine class
  InitChoice*     init_choi_;  // My InitChoice class object
  vector<Gamble*> gambles_;
  vector<Link*>   links_;
};

#endif  // game_h
