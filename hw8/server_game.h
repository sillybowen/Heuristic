#ifndef server_game_h
#define server_game_h
#include "game.h"
#include "ClientSocket.h"
#include "SocketException.h"

class ServerGame : public Game {
public:
  ServerGame(const char* plyName, int nFeatures, int srv_port);
  ~ServerGame();

  void startGame();
  bool readSrvOutput(const string& fromSrv);

  // For testing

private:
  ClientSocket* registerSrv(int srv_port);

  ClientSocket* arch_clt_;
};

#endif  // server_game_h
