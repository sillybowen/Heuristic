#ifndef board_h_
#define board_h_
#include <vector>
using namespace std;

class Board {
 private:
  std::vector<int> board;
  std::vector<bool> p1_used_block_;
  std::vector<bool> p2_used_block_;
  int len_;
  int num_blocks_;
  int center;
  int f1,f2;
 public:
  Board(int l, int n);
  bool add(int pl,int p, int w);
  bool remove(int p);

  // -1 error 0 correct 1 tipped
  int validateAdd(int pl,int p, int w);
  // -1 error 0 correct 1 tipped
  int validateRemove(int p);

  void updateAddf1f2(int p,int w,int &f1,int &f2);
  void updateRemovef1f2(int p,int &f1,int &f2);
};
#endif
