#include "dp_remove.h"
int main() {
  int boardLen = 30;
  int x[30];
  for (int i = 0;i<12;i++) {
    x[i*2] = i+1;
    x[i*2+1] = i+1;
  }
  x[24] = 3;
  DPRemove dd(30,25,x);
  dd.work();
  
  return 0;
}
