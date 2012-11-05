#ifndef BOWEN_HUNTER_HEADER_
#define BOWEN_HUNTER_HEADER_

#include "moveable.h"
#include <queue>
class Bowen_Hunter : public Moveable {
public:
  Bowen_Hunter(int hx, int hy, int nn, int mm) : Moveable(hx, hy, nn, mm) { 
    boarder[0] = 0;
    boarder[1] = 500;
    boarder[2] = 0;
    boarder[3] = 500;      
    nextWallAction = 0;
    wallNum = mm;
    inUsedWall.resize(4,NULL);
  }
  ~Bowen_Hunter();

  void output() const;
  bool isHunter() const;
  // Bowen_Hunter cannot control his move directly. He can only change his direction by
  // bouncing, so this method is mainly for creating/removing walls
  HuntPreyOutput tryMove();

private:
  std::queue<Wall*> outdatedWalls;
  std::vector<Wall*> inUsedWall;
  int boarder[4]; // 0- upper 1-lower 2 left 3 right  
  int dx,dy;
  int px,py;
  int hx,hy;
  int nextWallAction;
  int wallNum;
  bool bounced;
};

#endif  // HUNTER_HEADER_
