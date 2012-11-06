#ifndef MOVEABLE_HEADER_
#define MOVEABLE_HEADER_
#include <cstdlib>
#include <cmath>
#include <vector>

using std::vector;

class Evasion;
class Moveable {
public:
  union HuntPreyOutput {
    struct {
      int x1, y1, x2, y2;
    };
    struct {
      int dx, dy;
    };
    HuntPreyOutput(int xx1, int yy1, int xx2, int yy2)
      : x1(xx1), y1(yy1), x2(xx2), y2(yy2) { }
    HuntPreyOutput(int ddx, int ddy) : dx(ddx), dy(ddy) { }
  private:
    HuntPreyOutput();  // Forbidden this method
  };

  struct Wall {
    // If 0 <= index <= 3, this wall is game border, NO thickness, eg: (0, 0, 500, 0)
    Wall(int stx, int sty, int endx, int endy, int index);
    bool willHitThisWall(const Moveable* obj, int dx, int dy) const;
    int isTwoWallAdjacent(const Wall* anoWall, int& p1x, int& p1y, int& p2x,
        int& p2y) const;  // return 1 is adj, 0 Not adj, -1 error(two wall collide)

    const int wid_;
    // (x1, y1) on the left or on the top
    const int x1, y1, x2, y2;  // Wall starts from (x1, y1) to (x2, y2)
  };

  struct Pos{
    int x, y;
    Pos(){
      x = 0;
      y = 0;
    }
    void set(int x_, int y_){
      x = x_;
      y = y_;
    }
  };

  struct Bitmap{
    bool isWall;      // true : it is wall
    bool isAvail;     // true : the same area with prey & hunter
    int weight;

    Bitmap()
    : isWall(false), isAvail(false), weight(0) { }

    void clear(){
      isWall = false;
      isAvail = false;
      weight = -1;
    }
  };
  
  Bitmap bitmap[501][501];   // Save all information of each position (500 by 500)

  Moveable(int initx, int inity, int nn, int mm);
  virtual ~Moveable(){}
  virtual void output() const = 0;
  virtual bool isHunter() const = 0;
  virtual HuntPreyOutput tryMove() = 0;

  double distance(const Moveable* anoObj) const;
  void setEvadeGamePtr(Evasion* pEva);
  Pos getNextHunterPosition(int n);   // the position of hunter after 'n' time stemp
  // Hunter's future route with 'prey's guess about wall create/rm, return #of steps
  // if could capture prey (== nstep is could NOT capture)
  int hunterNStepPrediction(int nstep, vector<Pos>& hFutureRoute,
			    const vector<Wall*>& hor_walls, const vector<Wall*>& ver_walls);

protected:
  int       x_;
  int       y_;
  const int N_;  // Hunter creat/rm a wall no more frequently than every N timesteps
  const int M_;  // At any given time the maximum number of walls there can be is M
  Evasion*  evade_game_;

};

#endif  // MOVEABLE_HEADER_
