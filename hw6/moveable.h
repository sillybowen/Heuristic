#ifndef MOVEABLE_HEADER_
#define MOVEABLE_HEADER_
#include <cstdlib>
#include <cmath>

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
    Wall(int stx, int sty, int endx, int endy, int index)
        : wid_(index), x1(stx), y1(sty), x2(endx), y2(endy) { }
    bool willHitThisWall(Moveable* obj, int dx, int dy) {  // dx, dy moving direction
      double newx = double(obj->x_ + dx), newy = double(obj->y_ + dy);
      if (newx <= double(x2) + 0.5 && newx >= double(x1) - 0.5 &&
          newy <= double(y2) + 0.5 && newy >= double(y1) - 0.5) {
        return true;
      }
      return false;
    }

    const int wid_;
    // (x1, y1) on the left or on the top
    const int x1, y1, x2, y2;  // Wall starts from (x1, y1) to (x2, y2)
  };

  Moveable(int initx, int inity, int nn, int mm)
    : x_(initx), y_(inity), N_(nn), M_(mm) { }
  virtual ~Moveable() { }
  virtual void output() const = 0;
  virtual bool isHunter() const = 0;
  virtual HuntPreyOutput tryMove() = 0;

  double distance(Moveable* anoObj) {
    double sumSquares = double((anoObj->x_ - x_) * (anoObj->x_ - x_))
      + double((anoObj->y_ - y_) * (anoObj->y_ - y_));
    return sqrt(sumSquares);
  };

protected:
  int x_;
  int y_;
  const int N_;  // Hunter creat/rm a wall no more frequently than every N timesteps
  const int M_;  // At any given time the maximum number of walls there can be is M
};

#endif  // MOVEABLE_HEADER_
