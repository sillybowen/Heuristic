#ifndef PREY_HEADER_
#define PREY_HEADER_

#define InitHunterPredPathDanger 2000
#define PreyPreferDirMaxPreference 1000

#include "moveable.h"

class Prey : public Moveable {
public:
  Prey(int px, int py, int nn, int mm);
  ~Prey();

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();

private:
  void preyAddPreferOnHunterDir(int h_dir_x, int h_dir_y, Pos hunter, Pos prey);
  void addPreferNearPrey(Pos prey, int p_dir_x, int p_dir_y);
  void preyAddDangerOnHunterPredPath(int stepsWent, const vector<Pos>& hFutureSteps);

  const int MaxNumOfSteps_;
  long dangerMap[501][501];  // high value means high chance to be captured
};

#endif  // PREY_HEADER_
