#include "portfolio_jinil.h"
#include <fstream>
#include <cassert>
#include <math.h>

Portfolio_jinil::Portfolio_jinil(){
  roundNum = 0;  
  mode = 2;
  totalAssets = 1.0;
  STOCKS = 20;
  benefit = 1.0;
}

void Portfolio_jinil::setMode(int mode_){
  mode = mode_;
}

void Portfolio_jinil::ParseFile(string inFile_srv_){
  ifstream inF(inFile_srv_.c_str());
  string buf;
  bool data_type = false;
  int SIZE = 10;
  while(getline(inF, buf)){

    if(buf.find("#")!=string::npos){
      data_type = !data_type;
      continue;
    }else if(buf == "")
      continue;
    
    if(data_type){   // data type : Gamble
      char buf1[SIZE], buf2[SIZE], buf3[SIZE], buf4[SIZE], 
	buf5[SIZE], buf6[SIZE], buf7[SIZE], buf8[SIZE];
      sscanf(buf.c_str(), "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", 
	     buf1, buf2, buf3, buf4, buf5, buf6, buf7, buf8);  
      gambles_.push_back(new Gamble(atoi(buf1), atoi(buf2), atof(buf3), atof(buf4),
				   atof(buf5), atof(buf6), atof(buf7), atof(buf8)));
    }else{           // data type : Link
      char gi[SIZE], gj[SIZE];
      sscanf(buf.c_str(), "%[^,],%[^,]", gi, gj);
      links_.push_back(new Link(atoi(gi), atoi(gj)));
    }
  }

  for(int i=0; i<gambles_.size(); i++)
    classInfo[ gambles_[i]->class_id ].member_list.push_back( gambles_[i]->gamble_id );
}

vector<double> *Portfolio_jinil::makeDecision(){
  cout << " [Round] " << roundNum << endl;
  cout << " [mode] " << mode << endl;
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

  // update expected ret of gambles
  for(int i=0; i<t_gambles_.size(); i++){
    t_gambles_[i]->calculateExpectRet();
  }

  // Reset the expect_ret
  for(int i=0; i<t_gambles_.size(); i++)
    t_gambles_[i]->calculateExpectRet();

  // 4> step : Select betting gambles
  selectGambles();
  roundNum++;

  getResult();

  cout << "benefit : " << benefit << endl;

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
  // bubble sorting : t_gambles_
  // The last one has the largest expect_ret
  for(int i=t_gambles_.size()-1; i>0; i--){
    for(int j=0; j<i; j++){
      if(t_gambles_[j]->expect_ret > t_gambles_[j+1]->expect_ret){
	Gamble *gamble = t_gambles_[j];
	t_gambles_[j] = t_gambles_[j+1];
	t_gambles_[j+1] = gamble;
      }
    }    
  }

  vector<BettingInfo*> pre_bettingInfo_;
  for(int i=0; i<bettingInfo_.size(); i++){
    if(bettingInfo_[i]->fraction == 0.0)
      continue;
    int gambleId = bettingInfo_[i]->gambleId;
    printf("previous betting [id]:%d, [fraction]:%f >> ", gambleId, bettingInfo_[i]->fraction);
    
    int resultType = -2;
    for(int j=0; j<resultInfo_.size(); j++){
      if(resultInfo_[j]->gambleId == gambleId){
	resultType = resultInfo_[j]->type;
	break;
      }
    }
    if(resultType == 1)
      cout << "type:high\t";
    else if(resultType == 0)
      cout << "type:middle\t"; 
    else if(resultType == -1)
      cout << "type:low\t";
    else
      cout << "ERROR" << endl;

    printf("h_ret:%f\tm_ret:%f\tl_ret:%f\n", gambles_[gambleId]->h_ret, gambles_[gambleId]->m_ret, gambles_[gambleId]->l_ret);

    pre_bettingInfo_.push_back( bettingInfo_[i] );
  }

  bettingInfo_.clear();

  cout << endl;

  /*
  for(int i=0; i<resultInfo_.size(); i++){
    if(resultInfo_[i]->type == 1){
      for(int j=0; j<pre_bettingInfo_.size(); j++){
	if(resultInfo_[i]->gambleId == pre_bettingInfo_[j]->gambleId){
	  printf("Favorable [%d] : %f -> %f\n", pre_bettingInfo_[j]->gambleId, pre_bettingInfo_[j]->fraction, pre_bettingInfo_[j]->fraction * 1.5);
	  pre_bettingInfo_[j]->fraction *= 1.5;
	  bettingInfo_.push_back( pre_bettingInfo_[j] );
	  break;
	}
      }
    }
  }
  */
  cout << endl;

  double total_invest_fraction = 0.0;
  for(int i=0; i<bettingInfo_.size(); i++)
    total_invest_fraction += bettingInfo_[i]->fraction;

  // If the value is more than 1?
  // remove some stocks until less than 1
  while(total_invest_fraction > 1){
    total_invest_fraction -= bettingInfo_[ bettingInfo_.size()-1 ]->fraction;
    bettingInfo_.pop_back();
  }

  int remainStocks = STOCKS - bettingInfo_.size();
  int count = remainStocks;
  double remainFraction = 1 - total_invest_fraction;
  for(int i=t_gambles_.size()-1; i>=0; i--){
    if(count <= 0)
      break;
    else{
      bool isSkip = false;
      // if this is already exist on the bettingInfo_,
      // skip this
      for(int j=0; j<bettingInfo_.size(); j++){
	if(t_gambles_[i]->gamble_id == bettingInfo_[j]->gambleId){
	  isSkip = true;
	  break;
	}
      }
      if(isSkip)
	continue;

      bool notValuable = false;

      if(notValuable)
	continue;
      else{
	int id = t_gambles_[i]->gamble_id;
	double fraction = remainFraction / remainStocks;
	bettingInfo_.push_back(new BettingInfo(id, fraction));
	count--;
      }
    }
  }
}

void Portfolio_jinil::giveRoundInfo(vector<int> &info){
  resultInfo_.clear();
  for(int i=0; i<info.size(); i++){
    int type;
    if(info[i] == 0)
      type = 1;
    else if(info[i] == 1)
      type = 0;
    else
      type = -1;
    resultInfo_.push_back(new ResultInfo(i, type));
  }

  cout << "From Server : " << resultInfo_.size() << " received" << endl;

  // update gamble_ information
  for(int i=0; i<resultInfo_.size(); i++){
    int gambleId = resultInfo_[i]->gambleId;
    gambles_[gambleId]->updatePreviousRet( resultInfo_[i]->type );
  }

  return_money.clear();
  for(int i=0; i<gambles_.size(); i++)
    return_money.push_back(0);

  double temp_benefit = 0.0;
  // calculate benefits
  for(int i=0; i<bettingInfo_.size(); i++){
    for(int j=0; j<resultInfo_.size(); j++){
      if(bettingInfo_[i]->gambleId == resultInfo_[j]->gambleId){
	int betting_id = bettingInfo_[i]->gambleId;
	double returnValue;
	if(resultInfo_[j]->type == 1)
	  returnValue += bettingInfo_[i]->fraction * benefit * gambles_[betting_id]->h_ret;
	else if(resultInfo_[j]->type == 0)
	  returnValue += bettingInfo_[i]->fraction * benefit * gambles_[betting_id]->m_ret;
	else
	  returnValue += bettingInfo_[i]->fraction * benefit * gambles_[betting_id]->l_ret;
	temp_benefit += returnValue;
	return_money[betting_id] = returnValue;
	break;
	}
    }
  }
  benefit = temp_benefit;
}

void Portfolio_jinil::getResult(){
  decision_.clear();

  for(int i=0; i<gambles_.size(); i++){ // Initialization
    decision_.push_back(0);
  }

  for(int i=0; i<bettingInfo_.size(); i++){
    int bet_id = bettingInfo_[i]->gambleId;
    double bet_fraction = bettingInfo_[i]->fraction;
    decision_[bet_id] = bet_fraction;
    printf("betting [id]:%d, [fraction]:%f\n", bettingInfo_[i]->gambleId, bettingInfo_[i]->fraction);
  }
}

void Portfolio_jinil::chooseBestGambles(int &begin, int &end){
  double best_SR = 0.0;
  int best_begin = 0;
  int best_end = 0;
  for(int j=0; j<t_gambles_.size(); j++){
    double temp_SR = calSharpRatio(best_begin, j);
    if(temp_SR > best_SR){
      best_end = j;
      best_SR = temp_SR;
    }
  }
  /*
  for(int i=0; i<t_gambles_.size(); i++){
    for(int j=i; j<t_gambles_.size(); j++){
      double temp_SR = calSharpRatio(i, j);
      if(temp_SR > best_SR){
	best_begin = i;
	best_end = j;
	best_SR = temp_SR;
      }
    }
  }
  */
  begin = best_begin;
  end = best_end;

  cout << "begin: " << begin << ", end: " << end << endl;
  cout << "best_SR : " << best_SR << endl;
}

double Portfolio_jinil::calSharpRatio(int begin, int end){
  double mean = 0.0;
  double var = 0.0;
  double sum = 0.0;
  int size = end - begin + 1;
  if(size < 10)
    return 0;
  
  // mean
  for(int i=begin; i<=end; i++)
    sum += t_gambles_[i]->expect_ret;
  
  mean = sum / size;
  
  // var
  for(int i=begin; i<=end; i++)
    var += pow(t_gambles_[i]->expect_ret - mean, 2);
  var /= size;
  
  /*
  cout << "begin: " << begin << ", end: " << end << endl;
  cout << "mean:" << mean << ", var:" << var << ", sum:" << sum << endl;
  cout << "Sharp Ration: " << sum/var << endl;
  */

  if(var == 0)
    return 0;
  else
    return sum / sqrt(var);
}
