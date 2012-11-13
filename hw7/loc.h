#ifndef loc_
#define loc_
#include <vector>
#include <iostream>
class Location {
 private:
  std::vector<Location *> children_;// 0 up 1 down 2 left 3 right
  bool occupied_;
  int id_;
  int x_,y_;
  int eatenBy_;
 public:
  bool tried_;  // for best orientation attempts

  int getID() const { return id_; }
  int getX() const {return x_;}
  int getY() const {return y_;}
  bool isOccupied() const { return occupied_; }
  std::vector<Location*>& getChildren() { return children_; }
  Location(int id,int x,int y);
  void AddChildren(Location* newChild);
  void setEaten(int x) {eatenBy_=x;}
  void output();
};
#endif
