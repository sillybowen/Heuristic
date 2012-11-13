#ifndef nano_
#define nano_
#include <vector>
#include <string>
#include <map>
#include "loc.h"
using namespace std;
class Nano {
 private:
  static map<string,int> dirMap_;
  std::vector<int> seq_;
  bool live_;
  int id_;
  int team_;
  int lastDir_;
 public:
  Nano (int id,string lastDir_,vector<string> seq, bool live, int team);
  static void initializeMap () ;
  static int lookFor(string);
  const int getTeam() const {return team_;}
  const bool live() const {return live_;}
  const int getId() const {return id_;}
  const int getLastDir_() const {return lastDir_;}
  void output();
  void die() {live_=false;}
  void move(vector<Location*> *locs);
};

#endif  // nano_
