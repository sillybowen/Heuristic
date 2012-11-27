#include "portfolio_jinil.h"

Portfolio_jinil::Portfolio_jinil(vector<Gamble*> gambles, vector<Link*> links, int alloc)
   :gambles_(gambles), links_(links), allocation(alloc)
{ 
  for(int i=0; i<gambles_.size(); i++)
    classInfo[ gambles_[i]->class_id ].member_list.push_back( gambles_[i]->gamble_id );
}

vector<double> Portfolio_jinil::betting(){
  
  
  return getResult();
}

void Portfolio_jinil::readResultFromSrv(string result){
  string str = result;
  int pos;
  resultInfo_.clear();
  str = str.substr(1, str.size()-1); // replace '['
  
  while((pos = str.find(":")) != string::npos){
    int t_id = atoi( str.substr(0,pos).c_str() );
    string t_attr = str.substr(pos+1, 1);
    int attr = 0;
    if(t_attr == "h")
      attr = 1;
    else if(t_attr == "m")
      attr = 0;
    else if(t_attr == "l")
      attr = -1;
    resultInfo_.push_back(new ResultInfo(t_id, attr));
    str = str.substr(pos+3, str.size()-4);
  }  
}

vector<double> Portfolio_jinil::getResult(){
  vector<double> ret;
  for(int i=0; i<gambles_.size(); i++) // Initialization
    ret.push_back(0.0);

  for(int i=0; i<bettingInfo_.size(); i++)
    ret[ bettingInfo_[i]->gambleId ] = bettingInfo_[i]->fraction;

  return ret;
}
