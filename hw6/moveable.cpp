#include "moveable.h"

/* Wall struct methods implementation */
Moveable::Wall::Wall(int stx, int sty, int endx, int endy, int index)
  : wid_(index), x1(stx), y1(sty), x2(endx), y2(endy) { }

// dx, dy moving direction
bool Moveable::Wall::willHitThisWall(const Moveable* obj, int dx, int dy) const {
  double newx = double(obj->x_ + dx), newy = double(obj->y_ + dy);
  if (newx <= double(x2) + 0.5 && newx >= double(x1) - 0.5 &&
      newy <= double(y2) + 0.5 && newy >= double(y1) - 0.5) {
    return true;
  }
  return false;
}

// p1 and p2 are two farthest away contact points, if only 1 contact point,
// (p2x, p2y) = (-1, -1)
int Moveable::Wall::isTwoWallAdjacent(const Wall* anoWall, int& p1x, int& p1y,
    int& p2x, int& p2y) const {
  if (x1 == x2 && (anoWall->x1 == anoWall->x2)) {  // Two vertical walls
    if (abs(x1 - anoWall->x1) == 1) {  // Adjacent
      return 1;
    } else if (abs(x1 - anoWall->x1) == 0 &&
        (anoWall->y2 - y1 == -1 || y2 - anoWall->y1 == -1)) { // two segments of a line
      return 1;
    } else if (abs(x1 - anoWall->x1) > 1) {  // Not adj
      return 0;
    } else {
      return -1;
    }
  } else if (y1 == y2 && (anoWall->y1 == anoWall->y2)) {  // Two horizontal walls
    if (abs(y1 - anoWall->y1) == 1 &&
      (anoWall->x2 - x1 >= -1 && x2 - anoWall->x1 >= -1)) {  // Adjacent
      return 1;
    } else if (abs(y1 - anoWall->y1) == 0 &&
        (anoWall->x2 - x1 == -1 || x2 - anoWall->x1 == -1)) { // two segments of a line
      return 1;
    } else if (abs(y1 - anoWall->y1) == 0 && (anoWall->x2 - x1 > -1 &&
        x2 - anoWall->x1 > -1)) { // Not adj
      return -1;
    } else {
      return 0;
    }
  } else if (x1 == x2 && (anoWall->y1 == anoWall->y2)) {  // anoWall hori, this vert
    if ((((y2 - anoWall->y1) == -1 || (anoWall->y1 - y1) == -1)) &&
        (x1 - anoWall->x1) >= -1 && (x1 - anoWall->x2) <= 1) {
      return 1;
    } else if ((y2 - anoWall->y1) > -1 && (y1 - anoWall->y1) < 1 &&
        (x1 - anoWall->x1) >= -1 && (x1 - anoWall->x2) <= 1) {  // Conflict
      return -1;
    } else {
      return 0;
    }
  } else {  // anoWall vertical, this horizontal
    if ((((y2 - anoWall->y1) == -1 || (anoWall->y1 - y1) == 1)) &&
        (x2 - anoWall->x1) >= -1 && (x1 - anoWall->x2) <= 1) {
      return 1;
    } else if ((y2 - anoWall->y1) > -1 && (anoWall->y2 - y1) > -1 &&
        (x2 - anoWall->x1) >= -1 && (x1 - anoWall->x2) <= 1) {
      return -1;
    } else {
      return 0;
    }
  }
}


/* Moveable class methods implementation */
Moveable::Moveable(int initx, int inity, int nn, int mm)
  : x_(initx), y_(inity), N_(nn), M_(mm), evade_game_(NULL) { }

Moveable::~Moveable(){
  hor_walls_.clear();
  ver_walls_.clear();
  h_pos_history.clear();
  p_pos_history.clear();
}

double Moveable::distance(const Moveable* anoObj) const {
  double sumSquares = double((anoObj->x_ - x_) * (anoObj->x_ - x_))
    + double((anoObj->y_ - y_) * (anoObj->y_ - y_));
  return sqrt(sumSquares);
}

void Moveable::setEvadeGamePtr(Evasion* pEva) { evade_game_ = pEva; }

void Moveable::updateStates(vector<Wall*> hor_walls, vector<Wall*> ver_walls, Pos H, Pos P){
  hor_walls_ = hor_walls;
  ver_walls_ = ver_walls;
  h_pos = H;
  p_pos = P;
  h_pos_history.push_back(H);
  p_pos_history.push_back(P);
}
