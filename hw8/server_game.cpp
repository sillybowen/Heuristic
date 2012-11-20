#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "server_game.h"
#include "btrim.hpp"
#include "gradient_matcher.h"

using std::cout;
using std::endl;
using std::setprecision;
using std::stringstream;
using std::ostringstream;
using std::istringstream;

ServerGame::ServerGame(const char* plyName, Someone* myPly, int nFeatures,
    int numThrs, int srv_port) : Game(plyName, nFeatures, numThrs),
                                 my_ply_(myPly),
                                 noises_(new double[nFeatures]),
                                 person_step_(0),
                                 matcher_step_(0),
                                 arch_clt_(registerSrv(srv_port)) { }

ServerGame::~ServerGame() { }

void ServerGame::startGame() {
  string fromPly = ply_name_, fromSrv, srvEndMark("\n");
  fromPly.push_back('\n');

  try {
    (*arch_clt_) << fromPly;  // Send server our player's name

    do {
      string tmpFromSrv;
      do {
        (*arch_clt_) >> tmpFromSrv;
        fromSrv += tmpFromSrv;
      } while (fromSrv.find(srvEndMark) == string::npos);
      // cout << "#FromSrv: " << fromSrv << "-----End of #FromSrv" << endl;

      // Save information into 'xx_matr', 'match_score_'
      // parserFromSrv(fromSrv);

      if (!readSrvOutput(fromSrv)) {
        break;
      }
      fromSrv.clear();

      sleep(1);
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

}

bool ServerGame::readSrvOutput(string& fromSrv) {
  int nn;
  string toSrv;

  while (1) {
    if (!fromSrv.compare(0, 2, "M ")) {
      istringstream ist(fromSrv.substr(2));
      fromSrv.clear();
      ist >> nn >> fromSrv;
      cout << "I'm (expect: Matcher): " << ((my_ply_->isMatchmaker())?
          "Matcher":"Person") << " N(expect: " << n_features_ << "): " << nn << endl;
      ltrim(fromSrv);
    } else if (!fromSrv.compare(0, 2, "P ")) {
      istringstream ist(fromSrv.substr(2));
      fromSrv.clear();
      ist >> nn >> fromSrv;
      cout << "I'm (expect: Person): " << ((my_ply_->isMatchmaker())?
          "Matcher":"Person") << " N(expect: " << n_features_ << "): " << nn << endl;
      ltrim(fromSrv);
    } else if (!fromSrv.compare(0, 7, "WEIGHTS")) {
      cout << "I'm (expect: Person): " << ((my_ply_->isMatchmaker())?
          "Matcher":"Person") << " Sending Exact Weights." << endl;
      my_ply_->sendOutVector(w_arr_);
      toSrv = printNVectorToSrv(w_arr_);
      cout << "#Person sent to Srv: " << toSrv << endl;
      ++person_step_;
      (*arch_clt_) << toSrv;
      break;
    } else if (!fromSrv.compare(0, 5, "NOISE")) {
      cout << "I'm (expect: Person): " << ((my_ply_->isMatchmaker())?
          "Matcher":"Person") << " Sending Noises." << endl;
      my_ply_->sendOutVector(noises_);
      toSrv = printNVectorToSrv(noises_);
      cout << "#Person sent to Srv: " << toSrv << endl;
      ++person_step_;
      (*arch_clt_) << toSrv;
      break;
    } else if (!fromSrv.compare(0, 1, "[")) {
      cout << "I'm (expect: Matcher): " << ((my_ply_->isMatchmaker())?
          "Matcher":"Person") << " Receiving 20 srv random candidates." << endl;
      // Receiving 20 random candidates from server, update xx_matr_ and match_score_
      parserFromSrv(fromSrv);  // parse and update @xx_matr_ and @match_score_
      dynamic_cast<GradientMatcher*>(my_ply_)->importRandCandsAndScores(xx_matr_,
          match_score_, 20);
      cout << "#fromSrv: " << fromSrv << endl;
      fromSrv = fromSrv.substr(fromSrv.find("\n"));
      ltrim(fromSrv);
      ++matcher_step_;
    } else if (!fromSrv.compare(0, 9, "CANDIDATE")) {
      cout << "I'm (expect: Matcher): " << ((my_ply_->isMatchmaker())?
          "Matcher":"Person") << " Sending Candidate." << endl;
      double tmpArr[n_features_];
      my_ply_->sendOutVector(tmpArr);
      toSrv = printNVectorToSrv(tmpArr);
      cout << "#Matcher sent to Srv: " << toSrv << endl;
      (*arch_clt_) << toSrv;
      ++matcher_step_;
      break;
    } else if (!fromSrv.compare(0, 2, "OK")) {
      cout << "Got msg from Srv: " << fromSrv << endl;
      fromSrv = fromSrv.substr(3);
      ltrim(fromSrv);
    } else if (!fromSrv.compare(0, 8, "GAMEOVER") ||
        !fromSrv.compare(0, 5, "ERROR")) {
      cout << "Got msg from Srv: " << fromSrv << endl;
      return false;
    } else if (fromSrv.empty()) {
      return true;
    } else {
      double value;
      istringstream ist(fromSrv);
      ist >> value >> fromSrv;
      my_ply_->gotValueForJustSentCand(value);  // send value to GradientMatcher
      cout << "Got value from srv: " << value << endl;
      ltrim(fromSrv);
    }
  }

  return true;
}

// Array @nVect has to be length of "N"
string ServerGame::printNVectorToSrv(const double* nVect) const {
  ostringstream ost;
  ost << "[";
  ost.setf(std::ios::fixed);
  for (int i = 0; i < n_features_; ++i) {
    if (i != (n_features_ - 1)) {
      ost << setprecision(2) << nVect[i] << ", ";
    } else {
      ost << setprecision(2) << nVect[i] << "]" << endl;  // Terminate string by '\n'
    }
  }

  return ost.str();
}

ClientSocket* ServerGame::registerSrv(int srv_port) {
  ClientSocket* pClt_socket = NULL;

  try {
    pClt_socket = new ClientSocket("localhost", srv_port);
  } catch (SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << " PortNum: "
      << srv_port << std::endl;
    assert(0);
  }

  return pClt_socket;
}

void ServerGame::parserFromSrv(string fromSrv){
  string str = fromSrv;

  const char* c_it = str.c_str();
  int candidate_count = 0;
  int attr_index = 0;

  // check
  /*
  size_t found;
  found = str.find("S->M:");
  if(found == string::npos)
    return;
    */
  
  bool inner_set = false;
  stringstream ss;
  for(int i=0; i<str.size(); i++){
    if(c_it[i] == '['){
      inner_set = true;
    }else if(c_it[i] == ']'){
      xx_matr_[ candidate_count ][ attr_index ] = atof( ss.str().c_str() );
      inner_set = false;
      candidate_count++;
      attr_index = 0;
      ss.str("");
    }else if(c_it[i] == ' ')
      continue;
    else{
      if(inner_set){
	if(c_it[i] == ','){
	  xx_matr_[ candidate_count ][ attr_index ] = atof( ss.str().c_str() );
	  attr_index++;
	  ss.str("");
	}else{
	  ss << c_it[i];
	}
      }else{
	if(c_it[i] == ','){
	  match_score_.push_back( atof(ss.str().c_str()) );
	  ss.str("");
	}else{
	  ss << c_it[i];
	}
      }
    }
    if(i==str.size()-1)
      match_score_.push_back( atof(ss.str().c_str()) );
  }
}
