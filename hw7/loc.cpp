#include "loc.h"

using namespace std;

Location::Location(int id, int x,int y) :
  id_(id),x_(x),y_(y),occupied_(false) {
  children_.resize(4,NULL);
}

void Location::AddChildren(Location* newChild) {
}

void Location::output() {
  cout<<id_<<' '<<x_<<' '<<y_<<endl;
}
