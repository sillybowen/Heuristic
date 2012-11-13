#include "loc.h"

using namespace std;

Location::Location(int id, int x,int y) :
  id_(id),x_(x),y_(y),occupied_(false),tried_(false), eatenBy_(0) {
  children_.resize(4,NULL);
}

void Location::AddChildren(Location* newChild) {
  if (newChild->getX()>x_)
    children_[3] = newChild;
  if (newChild->getX()<x_)
    children_[1] = newChild;
  if (newChild->getY()>y_)
    children_[0] = newChild;
  if (newChild->getY()<y_)
    children_[2] = newChild;
}

void Location::output() {
  Location* test = getUniqueChild();
  if (test!=NULL) {
    cerr<<"has unique child"<<test->getID()<<endl;
  }
  cerr<<"Node"<<id_<<' '<<x_<<' '<<y_<<endl;
  if (occupied_) cerr<<"occ"<<endl;
  else cerr<<"not occ"<<endl;
  if (children_[0]!=NULL) {
    cerr<<"Up children:"<<
      children_[0]->getX()<<' '<<
      children_[0]->getY()<<endl;
  }
  if (children_[1]!=NULL) {
    cerr<<"left children:"<<
      children_[1]->getX()<<' '<<
      children_[1]->getY()<<endl;
  }
  if (children_[2]!=NULL) {
    cerr<<"down children:"<<
      children_[2]->getX()<<' '<<
      children_[2]->getY()<<endl;
  }
  if (children_[3]!=NULL) {
    cerr<<"right children:"<<
      children_[3]->getX()<<' '<<
      children_[3]->getY()<<endl;
  }
}
Location* Location::getUniqueChild() {
  int count = 0;
  Location * ret=NULL;
  for (int i = 0; i<4; i++) 
    if (children_[i]!=NULL && !children_[i]->isOccupied()){
      count++;
      ret = children_[i];
    }
  if (count == 1)
    return ret;
  return NULL;
}
