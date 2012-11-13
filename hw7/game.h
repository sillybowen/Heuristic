#ifndef game_h
#define game_h
#include <vector>
#include <string>
#include "ClientSocket.h"
#include "SocketException.h"
#include "loc.h"
#include "nano.h"

using std::string;
using std::vector;

class Game {
public:
  Game(const char* plyName, const char* srvOutFile, int globalMaxK, int srv_port);
  ~Game() { }

  void startGame();
  void printLocsToSrv(const vector<Location>& newPlacements,
      const vector<Nano>& newNanos) const;
  bool readSrvOutput(const string& fromSrv);
  string nanoSeqsToStr(const Nano& aNano) const;
private:
  ClientSocket* registerSrv(int srv_port);

  const int     max_k_;  // Max num of nanomunchers in global scale
  ClientSocket* arch_clt_;
  const string  ply_name_;  // Player I am in control
  const string  inFile_srv_;  // input file from server (server send messages here)
};

#endif  // game_h
