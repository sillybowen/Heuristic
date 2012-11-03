#ifndef EVASION_HEADER_
#define EVASION_HEADER_

#include <string>
#include <vector>
#include "ClientSocket.h"
#include "SocketException.h"
#include "moveable.h"

using std::string;
using std::vector;

struct Pos{
  int x, y;
};

class Evasion {
public:
  // Transfer ownership of @moveable to this class
  Evasion(Moveable* moveable, int n, int m, int srv_port);
  ~Evasion();

  int startGame(string& teamName);
  // Listen to socket to get other player's move
  bool otherPlyMove();
  void findWallCircles();

  enum game_status { playing, HunterWin, PreyWin, equal };

  // For testing
  void dumpAdjWallsMatrixGraph() const;

private:
  // -----------incomplete
  void readSrvUpdateStates(const string& fromSrv);
  ClientSocket* registerSrv(int srv_port);

  const int N_;  // Hunter creat/rm a wall no more frequently than every N timesteps
  const int M_;  // At any given time the maximum number of walls there can be is M
  Moveable*               my_obj_;  // Hunter/Prey that I am playing(Controlled here)
  ClientSocket*           arch_clt_;
  vector<Moveable::Wall*> hor_walls_;
  vector<Moveable::Wall*> ver_walls_;
  int**                   adj_walls_;
  Pos h_pos;  // Hunter Position
  Pos p_pos;  // Prey Position
  int wall_index;
};

#endif
