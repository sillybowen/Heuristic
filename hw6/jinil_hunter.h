#ifndef JINIL_HUNTER_HEADER_
#define JINIL_HUNTER_HEADER_

#include "moveable.h"

class Jinil_Hunter : public Moveable {
public:
  Jinil_Hunter(int hx, int hy, int nn, int mm) : Moveable(hx, hy, nn, mm) { }
  ~Jinil_Hunter();

  enum hunter_direction { LRD_UP, LRD_DOWN, RLU_UP, RLU_DOWN, LRU_UP, LRU_DOWN, RLD_UP, RLD_DOWN };
  // explain : LRD_UP : the hunter moved LEFT->RIGHT && DOWN->UP
  //                    this way is on the upper than the prey position.

  void output() const;
  bool isHunter() const;
  // Jinil_Hunter cannot control his move directly. He can only change his direction by
  // bouncing, so this method is mainly for creating/removing walls
  HuntPreyOutput tryMove();
  void algorithm1();
  void getHunterDirection();
  void updatePosition();
  int calculateRemainRange();
  void createWall(int x, int y);
  void removeWall(int index);
  int checkRemovableWall();
  bool checkCreateWallSafe(int x, int y);
  void updateTempBitmap(int x, int y);
  void getNextPosition();
  void updateTempBitmap();

private:
  Pos p_past, h_past;
  Pos p_cur, h_cur;
  Pos p_next, h_next;                
  int st_x, st_y, end_x, end_y;
  int hunterDirection;
  int p_vector_x, p_vector_y;   // the vector of the p's movement
  int h_vector_x, h_vector_y;
  Bitmap temp_bitmap[500][500];
  vector<Moveable::Wall*> hor_walls;
  vector<Moveable::Wall*> ver_walls;
};

#endif  // HUNTER_HEADER_
