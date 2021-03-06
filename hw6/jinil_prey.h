#ifndef JINIL_PREY_HEADER_
#define JINIL_PREY_HEADER_

#include "moveable.h"

class Jinil_Prey : public Moveable {
public:
  Jinil_Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm) { }
  ~Jinil_Prey();

  enum hunter_direction { LRD, RLU, LRU, RLD };
  // explain : LRD_UP : the hunter moved LEFT->RIGHT && DOWN->UP
  //                    this way is on the upper than the prey position.

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();

  void algorithm1();
  void updatePosition();
  void getHunterDirection();
  void updateTempBitmap();
  void updateHunterFuturePosition(int n);
  struct h_dir_info{
    int direction;
    Moveable::Pos start_pos;
    h_dir_info(){
      direction = -1;
    }
  };

private:
  Pos p_past, h_past;
  Pos h_past_past;
  Pos p_cur, h_cur;
  Pos p_next, h_next;                
  h_dir_info hunterDirection[3];
  int p_vector_x, p_vector_y;   // the vector of the p's movement
  int h_vector_x, h_vector_y;
  int result_x, result_y;       // Return value
  Bitmap temp_bitmap[500][500];
  vector<Moveable::Wall*> hor_walls;
  vector<Moveable::Wall*> ver_walls;
  vector<Moveable::Pos> h_future_pos;
  int Max_future;
  int we;
};

#endif  // PREY_HEADER_
