#include <iostream>
#include "bowen_hunter.h"

using std::cout;
using std::endl;

Bowen_Hunter::~Bowen_Hunter() { }

void Bowen_Hunter::output() const {
  cout << "Hunter pos: (" << x_ << ", " << y_ << ")" << endl;
}

bool Bowen_Hunter::isHunter() const {
  return true;
}

Moveable::HuntPreyOutput Bowen_Hunter::tryMove() {
  if (nextWallAction != 0) {
    nextWallAction --;
    Moveable::HuntPreyOutput ret(0,0);
    return ret;
  }
  if (wallNum == 0) {
    wallNum++;
    nextWallAction = N_;
    // only valid when wallNum>4
    Wall* w = outdatedWalls.front();
    outdatedWalls.pop();
    HuntPreyOutput ret(w->x1,w->y1,w->x2,w->y2);
    return ret;
  }
  // create wall at upper boundary
  if (dy>0 && // move down
      py>=hy && // prey below me
      py - hy<3 &&  // prey is nearly to get above me
      (hy-boarder[0])*3
      >boarder[1]-boarder[0] // not near boarder
      ) {
    HuntPreyOutput ret(boarder[2],
			 hy-1,
			 boarder[3],
			 hy-1);
    Wall * w = new Wall(boarder[2],
			hy-1,
			boarder[3],
			hy-1,
			0);
    if (inUsedWall[0] != NULL) {
      outdatedWalls.push(inUsedWall[0]);
    }
    inUsedWall[0] = w;
    boarder[0] = hy-1;
    wallNum --;
    nextWallAction = N_;
    return ret;    
  }
  // create wall at lower boundary
  if (dy<0 && // move up
      py<=hy && // prey above me
      hy - py<3 &&  // prey is nearly to get above me
      (boarder[1]-hy)*3
      >boarder[1]-boarder[0] // not near boarder
      ) {
    HuntPreyOutput ret(boarder[2],
			 hy+1,
			 boarder[3],
			 hy+1);
    Wall * w = new Wall(boarder[2],
			hy+1,
			boarder[3],
			hy+1,
			0);
    if (inUsedWall[1] != NULL) {
      outdatedWalls.push(inUsedWall[1]);
    }
    inUsedWall[1] = w;
    boarder[1] = hy+1;
    wallNum --;
    nextWallAction = N_;
    return ret;    
  }
  // create wall at left boundary
  if (dx>0 && // move right
      px>=hx && // prey below me
      px - hx<3 &&  // prey is nearly to get above me
      (hx-boarder[2])*3
      >boarder[3]-boarder[2] // not near boarder
      ) {
    HuntPreyOutput ret(hx-1,
			 boarder[0],
			 hx-1,
			 boarder[1]);
    Wall * w = new Wall(hx-1,
			boarder[0],
			hx-1,
			boarder[1],
			0);
    if (inUsedWall[2] != NULL) {
      outdatedWalls.push(inUsedWall[2]);
    }
    inUsedWall[2] = w;
    boarder[2] = hx-1;
    wallNum --;
    nextWallAction = N_;
    return ret;    
  }
  // create wall at right boundary
  if (dx<0 && // move left
      px<=hx && // prey below me
      hx - px<3 &&  // prey is nearly to get above me
      (boarder[3]-hx)*3
      >boarder[3]-boarder[2] // not near boarder
      ) {
    HuntPreyOutput ret(hx+1,
			 boarder[0],
			 hx+1,
			 boarder[1]);

    Wall * w = new Wall(hx+1,
			boarder[0],
			hx+1,
			boarder[1],
			0);
    if (inUsedWall[3] != NULL) {
      outdatedWalls.push(inUsedWall[3]);
    }
    inUsedWall[3] = w;
    boarder[3] = hx+1;
    wallNum --;
    nextWallAction = N_;
    return ret;    
  }

}
