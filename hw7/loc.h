#ifndef loc_
#define loc_
#include <vector>
#include <iostream>
class Location {
 private:
  std::vector<Location *> children_;// 0 up 2 down 1 left 3 right
  bool occupied_;
  int id_;
  int x_,y_;
  int eatenBy_;
 public:
  Location(int id,int x,int y);


  bool tried_;  // for best orientation attempts
  
  int getID() const { return id_; }
  int getX() const {return x_;}
  int getY() const {return y_;}
  bool isOccupied() const { return occupied_; }
  std::vector<Location*>& getChildren() { return children_; }
  void AddChildren(Location* newChild);
  void setEaten(int x) {eatenBy_=x;}
  void setOccupied() {occupied_=true;}
  void output();
  Location* getUniqueChild();
  Location* getChild(int i) {return children_[i];}
  void outputKillers();
};
#endif
