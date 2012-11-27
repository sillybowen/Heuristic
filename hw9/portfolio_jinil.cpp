#include "portfolio_jinil.h"

Portfolio_jinil::Portfolio_jinil(){
  roundNum = 0;  
}

void Portfolio_jinil::getData(vector<Gamble*> gambles, vector<Link*> links){
  gambles_ = gambles;
  links_ = links;
  for(int i=0; i<gambles_.size(); i++)
    classInfo[ gambles_[i]->class_id ].member_list.push_back( gambles_[i]->gamble_id );
}

vector<double> *Portfolio_jinil::makeDecision(float alloc){
  allocation = alloc;
  // 0> step : Copy gambles_ => t_gambles_;
  //            to modify gambles' probabilities for this round
  init();

  if(roundNum != 0){
    // 1> step : If this is not first round (roundNum != 0), 
    //           re-set up of class type (favorable, unfavorable or neutral)
    //           based on previous result (resultInfo_)
    setupClassType();
  
    // 2> step : Change probability of each gambles 
    //             depend on their class type (favorable, unfavorable)
    changeProbOnClass();

    // 3> step : Change probability of each gambles 
    //             depend on their link relationship.
    changeProbOnLink();
  }

  // 4> step : Select betting gambles
  selectGambles();

  roundNum++;
  getResult();
  return &decision_;
}

// Step 0
void Portfolio_jinil::init(){
  t_gambles_.clear();
  for(int i=0; i<gambles_.size(); i++){
    t_gambles_.push_back(new Gamble( gambles_[i]->gamble_id, gambles_[i]->class_id,
				     gambles_[i]->h_ret, gambles_[i]->h_prob,
				     gambles_[i]->m_ret, gambles_[i]->m_prob,
				     gambles_[i]->l_ret, gambles_[i]->l_prob));
  }
}

// Step 1
void Portfolio_jinil::setupClassType(){
  if(resultInfo_.size() != 0){
    for(int i=0; i<16; i++){
      int majority = 0;
      for(int j=0; j<resultInfo_.size(); j++){
	int class_id = gambles_[ resultInfo_[j]->gambleId ]->class_id;
	if(class_id == i){
	  majority += resultInfo_[j]->type;
	}
      }
      if(majority > 0)
	classInfo[i].type = 1;
      else if(majority < 0)
	classInfo[i].type = -1;
      else
	classInfo[i].type = 0;
    }
  }
}

// Step 2
void Portfolio_jinil::changeProbOnClass(){
  for(int i=0; i<t_gambles_.size(); i++){
    int class_id = t_gambles_[i]->class_id;
    switch(classInfo[ class_id ].type){
    case 1:  // favorable class
      t_gambles_[i]->increaseHighProb();
      break;
    case -1: // unfavorable class
      t_gambles_[i]->increaseLowProb();
      break;
    default: // neutral class
      // nothing change
      break;
    }
  }
}

// Step 3
void Portfolio_jinil::changeProbOnLink(){
  for(int i=0; i<t_gambles_.size(); i++){
    int Hi, Mi, Li;
    Hi = Mi = Li = 0;
    for(int j=0; j<links_.size(); j++){
      if(links_[j]->gi == i || links_[j]->gj == i){
	int gj;
	if(links_[j]->gi == i)
	  gj = links_[j]->gj;
	else
	  gj = links_[j]->gi;
	int class_gj = t_gambles_[gj]->class_id;
	if(classInfo[class_gj].type == 1) // Hi
	  Hi++;
	else if(classInfo[class_gj].type == -1) // Li
	  Li++;
	else if(classInfo[class_gj].type == 0) // Mi
	  Mi++;
      }
    }
    if(Hi > Mi + Li)
      t_gambles_[i]->increaseHighProb();
    else if(Li > Hi + Mi)
      t_gambles_[i]->increaseLowProb();
  }
}

// Step 4
void Portfolio_jinil::selectGambles(){
  bettingInfo_.clear();
  
  // bubble sorting : t_gambles_
  for(int i=t_gambles_.size()-1; i>0; i--){
    for(int j=0; j<i; j++){
      if(t_gambles_[j]->expect_ret > t_gambles_[j+1]->expect_ret){
	Gamble *gamble = t_gambles_[j];
	t_gambles_[j] = t_gambles_[j+1];
	t_gambles_[j+1] = gamble;
      }
    }    
  }
  
  int class_count[16];
  for(int i=0; i<16; i++)
    class_count[i] = 0;

  for(int i=t_gambles_.size()-1; i>=0; i--){
    int gamble_id = t_gambles_[i]->gamble_id;
    int class_id = t_gambles_[i]->class_id;
    if(class_count[ class_id ] < 3){
      bettingInfo_.push_back(new BettingInfo(gamble_id));
      class_count[ class_id ]++;
    }
  }

  int total_betting_num = bettingInfo_.size();
  float common_fraction = 1.0 / (total_betting_num);
  for(int i=0; i<bettingInfo_.size(); i++)
    bettingInfo_[i]->fraction = common_fraction;
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

void Portfolio_jinil::getResult(){
  decision_.clear();
  for(int i=0; i<gambles_.size(); i++) // Initialization
    decision_.push_back(0.0);

  for(int i=0; i<bettingInfo_.size(); i++)
    decision_[ bettingInfo_[i]->gambleId ] = bettingInfo_[i]->fraction;
}
