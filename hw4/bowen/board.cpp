#include<vector>
#include"board.h"
using namespace std;

Board::Board(int l, int n) :
  len_(l),num_blocks_(n) {
  board.resize(l,0);
  p1_used_block_.resize(n+1,false);
  p2_used_block_.resize(n+2,false);

  center = -2;
  f1 = -6;
  f2 = 6;
}
int Board::validateAdd(int pl, int p, int w) {
  int pos = p+len_/2;
  if (board[pos]!=0)
    return -1;
  int ff1,ff2;
  updateAddf1f2(p,w,ff1,ff2);
  if (ff1>0||ff2<0)
    return 1;
  return 0;
}
int Board::validateRemove(int p) {
  int pos = p+len_/2;
  if (board[pos]==0)
    return -1;
  int ff1,ff2;
  updateRemovef1f2(p,ff1,ff2);
  if (ff1>0||ff2<0)
    return 1;
  return 0;
}
bool Board::add(int pl,int p,int w) {
  if (validateAdd(pl,p,w)!=0)
    return false;
  int pos = p+ len_/2;
  if (pl == 1)
    p1_used_block_[w] = true;
  else
    p2_used_block_[w] = true;
  board[pos] = w;
  updateAddf1f2(p,w,f1,f2);
  return true;
}
bool Board::remove(int p) {
  if (validateRemove(p)!=0)
    return false;
  int pos = p+len_/2;
  board[pos] = 0;
  updateRemovef1f2(p,f1,f2);
}
void Board::updateRemovef1f2(int p,int &ff1,int &ff2) {
  int w = board[p+len_/2];
  int ansf1,ansf2;
  if (p<-3) {
    ansf1 = f1 + (-3 -p)*w;
    ansf2 = f2 + (-1 -p)*w;
  }
  if (p>-1) {
    ansf1 = f1 - (p + 3) *w;
    ansf2 = f2 - (p + 1) *w;
  }
  if (p == -2) {
    ansf1 = f1-w;
    ansf2 = f2+w;
  }
  if (p == -1) {
    ansf1 = f1-2*w;
  }
  if (p == -3) {
    ansf2 = f2 + 2*w;
  }
  ff1 = ansf1;
  ff2 = ansf2;
}
void Board::updateAddf1f2(int p,int w,int &ff1,int &ff2) {
  int ansf1,ansf2;
  if (p<-3) {
    ansf1 = f1 - (-3 -p)*w;
    ansf2 = f2 - (-1 -p)*w;
  }
  if (p>-1) {
    ansf1 = f1 + (p + 3) *w;
    ansf2 = f2 + (p + 1) *w;
  }
  if (p == -2) {
    ansf1 = f1+w;
    ansf2 = f2-w;
  }
  if (p == -1) {
    ansf1 = f1+2*w;
  }
  if (p == -3) {
    ansf2 = f2 - 2*w;
  }
  ff1 = ansf1;
  ff2 = ansf2;
}

