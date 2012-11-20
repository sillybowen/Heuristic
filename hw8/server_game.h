#ifndef server_game_h
#define server_game_h
#include "game.h"
#include "ClientSocket.h"
#include "SocketException.h"

class ServerGame : public Game {
public:
  ServerGame(const char* plyName, Someone* myPly, int nFeatures, int numThrs,
      int srv_port);
  ~ServerGame();

  void startGame();
  bool readSrvOutput(const string& fromSrv);

  string printNVectorToSrv(const double* nVect) const;
  // For testing
  
  void parserFromSrv(string fromSrv);

private:
  ClientSocket* registerSrv(int srv_port);

  Someone*      my_ply_;  // My player, either a Person or Matchmaker
  double*       noises_;
  // 1: sent Exact weights, 2: first noise vector sent, 3: second noise vector sent
  int           person_step_;
  // 1: received candidates, 2: sent first proposed cand, 3: got first proposal score
  int           matcher_step_;  
  ClientSocket* arch_clt_;
};

#endif  // server_game_h
