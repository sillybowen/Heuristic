#ifndef HUNTER_HEADER_
#define HUNTER_HEADER_

#include "moveable.h"

class Hunter : public Moveable {
public:
  Hunter(int hx, int hy, int nn, int mm) : Moveable(hx, hy, nn, mm) { }
  ~Hunter();

  void output() const;
  bool isHunter() const;
  // Hunter cannot control his move directly. He can only change his direction by
  // bouncing, so this method is mainly for creating/removing walls
  HuntPreyOutput tryMove();

private:
};

#endif  // HUNTER_HEADER_
