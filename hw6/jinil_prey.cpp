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
  // Past position
  h_past = evade_game_->h_pos_history_[ evade_game_->h_pos_history_.size()-2];  
  p_past = evade_game_->p_pos_history_[ evade_game_->p_pos_history_.size()-2];
  
  // Current position
  h_cur = evade_game_->h_pos;
  p_cur = evade_game_->p_pos;

  // Vector
  h_vector_x = h_cur.x - h_past.x;
  h_vector_y = h_cur.y - h_past.y;
  p_vector_x = p_cur.x - p_past.x;
  p_vector_y = p_cur.y - p_past.y;

  // anticipated next prey's position
  // next = current + (current - past)       // Need more specific, especially reflex on the wall
  getNextPosition();
  
}

void Jinil_Prey::getNextPosition(){
  
  h_next.x = h_cur.x + h_vector_x;
  h_next.y = h_cur.y + h_vector_y;

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
}

void Jinil_Prey::getHunterDirection(){
  Moveable::Pos h_next_pos = h_cur;
  int h_vector_x, h_vector_y;
  for(int i=0; i<3; i++)
    hunterDirection[i] = -1;
  int index = 0;

  // next position 부 분필 요

  // After some test, change the number 10.
  for(int s=0; s<10; s++){
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
