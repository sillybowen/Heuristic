#include <iostream>
#include "moveable.h"
#include "evasion.h"
using std::cout;

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
    if (abs(x1 - anoWall->x1) == 1 && anoWall->y2 + 1 >= y1 &&
        anoWall->y1 - 1 <= y2) {  // Adjacent
      return 1;
    } else if (abs(x1 - anoWall->x1) == 0 &&
        (anoWall->y2 - y1 == -1 || y2 - anoWall->y1 == -1)) { // two segments of a line
      return 1;
    } else if (abs(x1 - anoWall->x1) == 0 && anoWall->y2 >= y1 &&
        anoWall->y1 <= y2) {  // Collision
      return -1;
    } else {
      return 0;
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
    if (((((y2 - anoWall->y1) == -1 || (anoWall->y1 - y1) == -1)) &&
        (x1 - anoWall->x1) >= -1 && (x1 - anoWall->x2) <= 1) || (anoWall->y1 >= y1 &&
        anoWall->y1 <= y2 && (anoWall->x2 == x1 - 1 || anoWall->x1 == x1 + 1))) {
      return 1;
    } else if (anoWall->y1 <= y2 && anoWall->y1 >= y1 &&
        anoWall->x1 <= x1 && anoWall->x2 >= x1) {  // Conflict
      return -1;
    } else {
      return 0;
    }
  } else {  // anoWall vertical, this horizontal
    if (((((y2 - anoWall->y2) == 1 || (anoWall->y1 - y1) == 1)) &&
        (x2 - anoWall->x1) >= -1 && (x1 - anoWall->x1) <= 1) ||
        ((x2 - anoWall->x1 == -1 || x1 - anoWall->x1 == 1) && y2 - anoWall->y1 >= -1
         && y2 - anoWall->y2 <= 1)) {
      return 1;
    } else if ((y2 - anoWall->y1) > -1 && (anoWall->y2 - y1) > -1 &&
        (x2 - anoWall->x1) > -1 && (x1 - anoWall->x2) < 1) {
      return -1;
    } else {
      return 0;
    }
  }
}


/* Moveable class methods implementation */
Moveable::Moveable(int initx, int inity, int nn, int mm)
  : x_(initx), y_(inity), N_(nn), M_(mm), evade_game_(NULL) { 

  /*
  for(int i=0; i<=500; i++){
    for(int j=0; j<=500; j++)
      bitmap[i][j] = new Bitmap;
  }
  */
}

double Moveable::distance(const Moveable* anoObj) const {
  double sumSquares = double((anoObj->x_ - x_) * (anoObj->x_ - x_))
    + double((anoObj->y_ - y_) * (anoObj->y_ - y_));
  return sqrt(sumSquares);
}

void Moveable::setEvadeGamePtr(Evasion* pEva) { evade_game_ = pEva; }

// the position of hunter after 'n' time stemp
// Contain bounce of walls.
Moveable::Pos Moveable::getNextHunterPosition(int n){
  int count = n;
  vector<Wall*> hor_walls = evade_game_->hor_walls_;
  vector<Wall*> ver_walls = evade_game_->ver_walls_;
  vector<Pos> h_pos_history = evade_game_->h_pos_history_;
  Pos h_next, h_cur, h_past, h_past_past;
  int h_vector_x, h_vector_y;

  // Initialization
  h_cur = evade_game_->h_pos;
      
  if(h_pos_history.size()>=3){
    h_past = h_pos_history[ h_pos_history.size()-2 ];
    h_past_past = h_pos_history[ h_pos_history.size()-3 ];
  }else{
    h_past.x = h_cur.x-1;
    h_past.y = h_cur.y-1;
    h_past_past.x = h_cur.x-2;
    h_past_past.y = h_cur.x-2;
  }

  while(true){
    // Set Unit Vector
    if(h_cur.x == h_past.x || h_cur.y == h_past.y){ // Just previously bounced
      if(h_cur.x == h_past.x){   // Just previously bounced on a vertical wall
	if(h_past.x > h_past_past.x)
	  h_vector_x = -1;
	else if(h_past.x < h_past_past.x)
	  h_vector_x = 1;
      }
      if(h_cur.y == h_past.y){    // Just previously bounced on a horizontal wall
	if(h_past.y > h_past_past.y)
	  h_vector_y = -1;
	else if(h_past.y < h_past_past.y)
	  h_vector_y = 1;
      }
    }else{ // not bounced on a wall
      h_vector_x = h_cur.x - h_past.x;
      h_vector_y = h_cur.y - h_past.y;
    }

    // horizontal wall check
    for(int k=0; k<hor_walls.size(); k++){
      if(hor_walls[k]->y1 == h_next.y){
	h_next.y = h_cur.y;
      }
    }

    // vertical wall check
    for(int k=0; k<ver_walls.size(); k++){
      if(ver_walls[k]->x1 == h_next.x){
	h_next.x = h_cur.x;
      }
    }
  
    if(count > 1){
      h_past_past = h_past;
      h_past = h_cur;
      h_cur = h_next;
      count--;
    }else{
      break;
    }
  }
  return h_next;
}

// Changed api of jinil's getNextHunterPosition for prey.cpp
int Moveable::hunterNStepPrediction(int nstep, vector<Pos>& hFutureRoute,
    const vector<Wall*>& hor_walls, const vector<Wall*>& ver_walls, Pos h_cur, 
    const vector<Pos>& h_pos_history) {
  int count = nstep;
  // vector<Wall*> hor_walls = evade_game_->hor_walls_;
  // vector<Wall*> ver_walls = evade_game_->ver_walls_;
  // vector<Pos> h_pos_history = evade_game_->h_pos_history_;
  Pos h_next, h_past, h_past_past;
  int h_vector_x, h_vector_y;

  // Initialization -- @hFutureRoute starts with hunter's current position
  // h_cur = evade_game_->h_pos;
  hFutureRoute.push_back(h_cur);

  if(h_pos_history.size()>=3){
    h_past = h_pos_history[ h_pos_history.size()-2 ];
    h_past_past = h_pos_history[ h_pos_history.size()-3 ];
  }else{
    h_past.x = h_cur.x-1;
    h_past.y = h_cur.y-1;
    h_past_past.x = h_cur.x-2;
    h_past_past.y = h_cur.x-2;
  }

  while(true){
    // Set Unit Vector
    if(h_cur.x == h_past.x || h_cur.y == h_past.y){ // Just previously bounced
      if(h_cur.x == h_past.x){   // Just previously bounced on a vertical wall
        if(h_past.x > h_past_past.x)
          h_vector_x = -1;
        else if(h_past.x < h_past_past.x)
          h_vector_x = 1;
      }
      if(h_cur.y == h_past.y){    // Just previously bounced on a horizontal wall
        if(h_past.y > h_past_past.y)
          h_vector_y = -1;
        else if(h_past.y < h_past_past.y)
          h_vector_y = 1;
      }
    }else{ // not bounced on a wall
      h_vector_x = h_cur.x - h_past.x;
      h_vector_y = h_cur.y - h_past.y;
    }
    // Copy h_vector_x/y values to h_next
    h_next.set(h_cur.x + h_vector_x, h_cur.y + h_vector_y);

    // horizontal wall check
    for (int k=0; k<hor_walls.size(); k++) {
      // collision on horizontal border
      if (hor_walls[k]->wid_ < 4) {  // special wall -- borders
        if (hor_walls[k]->y1 == h_cur.y && (h_next.y < 0 || h_next.y > 500)) {
          h_next.y = h_cur.y - h_vector_y;
        }
      } else if (hor_walls[k]->y1 == h_next.y && h_next.x >= hor_walls[k]->x1 &&
          h_next.x <= hor_walls[k]->x2) {
        h_next.y = h_cur.y;
      }
    }

    // vertical wall check
    for (int k=0; k<ver_walls.size(); k++) {
      if (ver_walls[k]->wid_ < 4) {
        if (ver_walls[k]->x1 == h_cur.x && (h_next.x < 0 || h_next.x > 500)) {
          h_next.x = h_cur.x - h_vector_x;
        }
      } else if (ver_walls[k]->x1 == h_next.x && h_next.y >= ver_walls[k]->y1 &&
          h_next.y <= ver_walls[k]->y2) {
        h_next.x = h_cur.x;
      }
    }

    if(count > 1){
      hFutureRoute.push_back(h_next);  // Push_back next predicted route
      h_past_past = h_past;
      h_past = h_cur;
      h_cur = h_next;
      count--;
    }else{
      hFutureRoute.push_back(h_next);  // Push_back next predicted route
      break;
    }
  }

  return nstep - count;
}

