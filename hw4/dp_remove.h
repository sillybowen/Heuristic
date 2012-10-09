#ifndef dp_remove_h_
#define dp_remove_h_
#include <vector>

using namespace std;
class DPRemove {
 public:
  DPRemove( int len, int* board);
  int decision(int*b);
  void work();
 private:
  vector<int> weights_;
  vector<int> positions_;
  vector<short> action;
  vector<int> board_;
  vector<bool> winStat;
  int board_len_;
  int numOfWeights_;
  bool start_at_odd;
  int translateBoardToBit(int * board);
  int translateBoardToBit();
  bool tippedBoard(int stat);
  void generateSet(int d,int index,int ans,int d);
  void workOn(int bitN,int degree);
};

#endif
