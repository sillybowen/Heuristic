#ifndef MOVEABLE_HEADER_
#define MOVEABLE_HEADER_
#include <cstdlib>
#include <cmath>

class Moveable {
public:
  Moveable(int initx, int inity, int nn, int mm)
    : x_(initx), y_(inity), N_(nn), M_(mm) { }
  virtual ~Moveable();
  virtual void output() const = 0;
  virtual bool isHunter() const = 0;
  virtual bool tryMove() = 0;

  double distance(Moveable* anoObj) {
    double sumSquares = double((anoObj->x_ - x_) * (anoObj->x_ - x_))
      + double((anoObj->y_ - y_) * (anoObj->y_ - y_));
    return sqrt(sumSquares);
  };

  struct Wall {
    Wall(int stx, int sty, int endx, int endy, int index)
      : wid_(index), x1(stx), y1(sty), x2(endx), y2(endy) {
        if (y1 == y2) {  // Wall is horizontal

        }
      }
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

protected:
  int x_;
  int y_;
  const int N_;
  const int M_;
};

#endif  // MOVEABLE_HEADER_
