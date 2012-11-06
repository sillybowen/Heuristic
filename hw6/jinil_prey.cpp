#include "jinil_prey.h"
#include "evasion.h"

Jinil_Prey::~Jinil_Prey() {
  
}

void Jinil_Prey::output() const {
}

bool Jinil_Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Jinil_Prey::tryMove() {

  hor_walls = evade_game_->hor_walls_;
  ver_walls = evade_game_->ver_walls_;

  updatePosition();
  getHunterDirection();

  result_x = result_y = 0;

  algorithm1();

  return Moveable::HuntPreyOutput(result_x, result_y);
}

void Jinil_Prey::algorithm1(){
  
}

void Jinil_Prey::updateTempBitmap(){
  // Initialization
  for(int i=0; i<=500; i++){
    for(int j=0; j<=500; j++){
      bitmap[i][j].weight = 0;
    }
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
    hunterDirection[i] = -1;
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
      if(hunterDirection[index-1] != LRD)
	hunterDirection[index++] = LRD;
    }else if(h_vector_x<0 && h_vector_y<0){
      if(hunterDirection[index-1] != RLU)
	hunterDirection[index++] = RLU;
    }else if(h_vector_x>0 && h_vector_y<0){
      if(hunterDirection[index-1] != LRU)
	hunterDirection[index++] = LRU;
    }else if(h_vector_x<0 && h_vector_y>0){
      if(hunterDirection[index-1] != RLD)
	hunterDirection[index++] = RLD;
    }

  }
  
}
