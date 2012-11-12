#ifndef parser_
#define parser_
#include"tree.h"
#include"loc.h"
#include "nano.h"
class Parser {
 private:
  vector<Location*> locs;
  vector<Nano*> nanos;
 public:
  void  GetNodes(tree * srvTr);
  void  GetEdges(tree * srvTr);
  void  GetEaten(tree *srvTr);
  void  GetNanos(tree *srvTr);
  void  output();

  const vector<Location*>& getLocations() const { return locs; }
};
#endif
