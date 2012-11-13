#ifndef parser_
#define parser_
#include"tree.h"
#include"loc.h"
#include "nano.h"
#include "work.h"
#include "randwork.h"
class Parser {
 private:
  vector<Location*> locs;
  vector<Nano*> nanos;

  //Worker worker;
  RandWorker worker;

 public:
 Parser(int myteam, int k):worker(myteam,k){};
  void  GetNodes(tree * srvTr);
  void  GetEdges(tree * srvTr);
  void  GetEaten(tree *srvTr);
  void  GetNanos(tree *srvTr);
  void  output();
  void work(tree*srvTr,vector<Location>&loc,vector<Nano>&nano);
  const vector<Location*>& getLocations() const { return locs; }
};
#endif
