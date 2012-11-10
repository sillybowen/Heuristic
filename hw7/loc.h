#ifndef loc_
#define loc_
#include <vector>
#include <iostream>
class Location {
 private:
  std::vector<Location *> children_;// 0 up 1 down 2 left 3 right
  int id_;
  bool occupied_;
  int x_,y_;
 public:
  Location(int id,int x,int y);
  void AddChildren(Location* newChild);
  void output();
};
#endif
