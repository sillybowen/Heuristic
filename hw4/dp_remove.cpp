#include "dp_remove.h"
#include<iostream>
using namespace std;

DPRemove::DPRemove(int len, int* board):
  board_len_(len){
  for (int i = 0; i<len; i++)
    if (board[i]!=0) {
      weights_.push_back(board[i]);
      positions_.push_back(i);
    }
  int x = 1<<(weights_.size()+1);
  cout<<x<<endl;
  board_.resize(weights_.size());
  winStat.resize(x);
  action.resize(x);
  if (weights_.size()%2==0)
    start_at_odd = true;
  else
    start_at_odd = false;
}
int DPRemove::translateBoardToBit(int *board) {
  int ret = 0;
  for (int i = 0;i<weights_.size();i++) {
    ret*=2;
    if (board[positions_[i]] != 0)
      ret++;
  }
  return ret;
}
int DPRemove::translateBoardToBit() {
  int ret = 0;
  for (int i = 0;i<weights_.size();i++) {
    ret*=2;
    if (board_[i] != 0)
      ret++;
  }
  return ret;
}
bool DPRemove::tippedBoard(int stat) {
  int f1 = 0;
  int f2 = 3;
  int tmp = stat;
  int index = weights_.size()-1;
  while (tmp>0) {
    if (tmp%2==1) {
      f1+= (board_len_/2-3-positions_[index])*weights_[index];
      f2+= (positions_[index]-(board_len_/2-1))*weights_[index];
    }
    tmp/=2;
    index--;
  }
  if (f1>0||f2>0)
    return true;
  return false;
}

void DPRemove::workOn(int bitN, int degree) {
  if (tippedBoard(bitN)) {
    winStat[bitN] = true;
    return;
  }

  int bb;
  winStat[bitN] = false;
  for (int i = 0; i<board_.size();i++)
    if (board_[i] !=0) {
      board_[i] = 0;
      bb = translateBoardToBit();
      if (!winStat[bb]) {
        winStat[bitN] = true;
        action[bitN] = positions_[i];
        board_[i] = 1;
        return;
      }
      board_[i] = 1;
    }
// if the state is not must win state
  action[bitN] = -1;
  if (!winStat[bitN])
    for (int i = 0; i<board_.size();i++)
      if (board_[i] !=0) {
        if (action[bitN]==-1)
          action[bitN] = positions_[i];
        board_[i] = 0;
        bb = translateBoardToBit();
        if (!tippedBoard(bb)) {
          action[bitN] = positions_[i];
          board_[i] = 1;
          return;
        }
        board_[i] = 1;
      }
}

void DPRemove::generateSet(int d, int index, int ans,int degree) {
  if (index == weights_.size()) {
    if (d==0)
      workOn(ans,degree);
    return ;
  }
  generateSet(d,index+1,ans*2,degree);
  board_[index] = 1;
  generateSet(d-1,index+1,ans*2+1,degree);
  board_[index] = 0;
}

void DPRemove::work() {
  for (int i = 0; i<weights_.size();i++)
    board_[i] = 0;

  winStat[0] = true;

  for (int d = 1;d<=weights_.size();d++) {
    generateSet(d,0,0,d);
  }
}
int DPRemove::decision(int*board) {
  int b = translateBoardToBit(board);
  if (winStat[b])
    cout<<"win"<<endl;
  else
    cout<<"lose"<<endl;
  if (tippedBoard(b))
    return -1;
  return action[b];
}
