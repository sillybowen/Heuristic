#ifndef parser_
#define parser_
#include"tree.h"
#include"loc.h"
#include "nano.h"
#include "work.h"
#include "randwork.h"
#include "nano_guess.h"
class Parser {
 private:
  vector<Location*> locs;
  vector<Nano*> nanos;
  Worker worker;          // Bowen's worker
  RandWorker rand_worker; // Random worker
  NanoGuess tao_worker_;  // Tao's worker
  std::string ourName_;
 public:
 Parser(int myteam, int k, std::string name)
   : worker(myteam,k), tao_worker_(myteam, k), rand_worker(myteam, k),
   ourName_(name) {};
  void  GetNodes(tree * srvTr);
  void  GetEdges(tree * srvTr);
  void  GetEaten(tree *srvTr);
  void  GetNanos(tree *srvTr);
  void  output();
  void work(tree*srvTr,vector<Location>&loc,vector<Nano>&nano, int worker_sel);
  const vector<Location*>& getLocations() const { return locs; }

  void taoGreedy(tree* pSrvTr, vector<Location>& retloc, vector<Nano>& retnano);
};
#endif
