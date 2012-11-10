#include "loc.h"

using namespace std;

Location::Location(int id, int x,int y) :
  id_(id),x_(x),y_(y),occupied_(false),eatenBy_(0) {
  children_.resize(4,NULL);
}

void Location::AddChildren(Location* newChild) {
  if (newChild->getX()>x_)
    children_[3] = newChild;
  if (newChild->getX()<x_)
    children_[2] = newChild;
  if (newChild->getY()>y_)
    children_[0] = newChild;
  if (newChild->getY()<y_)
    children_[1] = newChild;
}

void Location::output() {
  cout<<"Node"<<id_<<' '<<x_<<' '<<y_<<endl;
  if (children_[0]!=NULL) {
    cout<<"Up children:"<<
      children_[0]->getX()<<' '<<
      children_[0]->getY()<<endl;
  }
  if (children_[1]!=NULL) {
    cout<<"down children:"<<
      children_[1]->getX()<<' '<<
      children_[1]->getY()<<endl;
  }
  if (children_[2]!=NULL) {
    cout<<"left children:"<<
      children_[2]->getX()<<' '<<
      children_[2]->getY()<<endl;
  }
  if (children_[3]!=NULL) {
    cout<<"right children:"<<
      children_[3]->getX()<<' '<<
      children_[3]->getY()<<endl;
  }
}
