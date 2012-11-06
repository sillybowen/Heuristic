#include "jinil_prey.h"
#include "evasion.h"

Jinil_Prey::~Jinil_Prey() {
  
}

void Jinil_Prey::output() const {
}

bool Jinil_Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Jinil_Prey::tryMove() {
  // Initialization
  hor_walls = evade_game_->hor_walls_;
  ver_walls = evade_game_->ver_walls_;
  result_x = result_y = 0;

  // Update
  updatePosition();
  getHunterDirection();
  updateTempBitmap();
  updateHunterFuturePosition();

  // Algorithm
  algorithm1();

  return Moveable::HuntPreyOutput(result_x, result_y);
}

void Jinil_Prey::algorithm1(){
  
}

void Jinil_Prey::updateTempBitmap(){
  // Initialization
  for(int i=0; i<=500; i++){
    for(int j=0; j<=500; j++){
      bitmap[i][j].weight = -1;
    }
  }

  for(int s=0; s<h_future_pos.size(); s++){
    int x = h_future_pos[s].x;
    int y = h_future_pos[s].y;
    int x1, x2, y1, y2;
    
    if(x >= 4) x1 = x-4;
    else       x1 = x;
    if(x <= 496) x2 = x+4;
    else       x2 = x;
    if(y >= 4) y1 = y-4;
    else       y1 = y;
    if(y <= 496) y2 = y+4;
    else       y2 = y;

    for(int i=x1-1; i<=x2-1; i++){
      for(int j=y1-2; j<=y2-2; j++){
	if(i>=0 && i<=500 && j>=0 && j<=500 && bitmap[i][j].weight==-1)
	  bitmap[i][j].weight = s/2;   // because hunter can move 2 times than prey move
      }
    }
    if(y1+1 == y-3){
      for(int i=x1-2; i<=x2-2; i++){
	if(i>=0 && i<=500 && bitmap[i][y-3].weight==-1)
	  bitmap[i][y-3].weight = s/2;
      }
    }
    if(y2-1 == y+3){
      for(int i=x1-2; i<=x2-2; i++){
	if(i>=0 && i<=500 && bitmap[i][y+3].weight==-1)
	  bitmap[i][y+3].weight = s/2;
      }
    }
    if(y1 == y-4 && bitmap[x][y1].weight==-1)
      bitmap[x][y1].weight = s/2;
    if(y2 == y+4 && bitmap[x][y2].weight==-1)
      bitmap[x][y2].weight = s/2;
    if(x1 == x-4 && bitmap[x1][y].weight==-1)
      bitmap[x1][y].weight = s/2;
    if(x2 == x+4 && bitmap[x2][y].weight==-1)
      bitmap[x2][y].weight = s/2;
  }
}

void Jinil_Prey::updateHunterFuturePosition(){
  int Max_future = 10;    // After some test, change the number 10.
  h_future_pos.clear();
  for(int i=0; i<Max_future; i++){
    h_future_pos.push_back( getNextHunterPosition(i) );
  }
}

void Jinil_Prey::updatePosition(){
  vector<Pos> h_pos_history = evade_game_->h_pos_history_;
  vector<Pos> p_pos_history = evade_game_->p_pos_history_;
  // Current position
  h_cur = evade_game_->h_pos;
  p_cur = evade_game_->p_pos;

  // Past position
  if(h_pos_history.size()>=3){
    h_past = h_pos_history[ h_pos_history.size()-2 ];
    h_past_past = h_pos_history[ h_pos_history.size()-3 ];
  }else{
    h_past.x = h_cur.x-1;
    h_past.y = h_cur.y-1;
    h_past_past.x = h_cur.x-2;
    h_past_past.y = h_cur.x-2;
  }
  if(p_pos_history.size()>=2)
    p_past = p_pos_history[ p_pos_history.size()-2];
  else
    p_past = p_cur;
  
  // Vector
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

  p_vector_x = p_cur.x - p_past.x;
  p_vector_y = p_cur.y - p_past.y;

  // anticipated next prey's position
  // next = current + (current - past)       // Need more specific, especially reflex on the wall
  h_next = getNextHunterPosition(1);
  p_next.x = p_cur.x + p_vector_x;
  p_next.y = p_cur.y + p_vector_y;  
}

void Jinil_Prey::getHunterDirection(){
  vector<Pos> h_pos_history = evade_game_->h_pos_history_;
  vector<Pos> h_pos_future;
  Pos h_next, h_cur, h_past, h_past_past;
  int h_vector_x, h_vector_y;
  for(int i=0; i<3; i++)
    hunterDirection[i].direction = -1;
  int index = 0;
  int Max_future = 10;    // After some test, change the number 10.

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

  h_pos_future.push_back(h_past_past);
  h_pos_future.push_back(h_past);
  h_pos_future.push_back(h_cur);

  for(int s=0; s<Max_future; s++){
    h_pos_future.push_back( getNextHunterPosition(s+1) );
  }
  
  for(int s=0; s<Max_future; s++){
    h_past_past = h_pos_future[s];
    h_past = h_pos_future[s+1];
    h_cur = h_pos_future[s+2];

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

    // Set hunter direction
    if(h_vector_x>0 && h_vector_y>0){
      if(hunterDirection[index-1].direction != LRD){
	hunterDirection[index].direction = LRD;
	hunterDirection[index++].start_pos = h_cur;
      }
    }else if(h_vector_x<0 && h_vector_y<0){
      if(hunterDirection[index-1].direction != RLU){
	hunterDirection[index].direction = RLU;
	hunterDirection[index++].start_pos = h_cur;
      }
    }else if(h_vector_x>0 && h_vector_y<0){
      if(hunterDirection[index-1].direction != LRU){
	hunterDirection[index].direction = LRU;
	hunterDirection[index++].start_pos = h_cur;
      }
    }else if(h_vector_x<0 && h_vector_y>0){
      if(hunterDirection[index-1].direction != RLD){
	hunterDirection[index].direction = RLD;
	hunterDirection[index++].start_pos = h_cur;
      }
    }

  }
  
}
