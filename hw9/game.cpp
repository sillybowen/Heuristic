#include <cassert>
#include <fstream>
#include <sstream>
#include "game.h"
#include "gamble.h"
using namespace std;

Game::Game(const char* plyName, int srv_port, int mode)
  : ply_name_(string(plyName)), arch_clt_(registerSrv(srv_port)), mode_(mode) { }

void Game::startGame(int user) {
  string fromPly = ply_name_, fromSrv, tmpFromSrv;
  bool firstDone = false;
  fromPly.push_back('\n');

  // user : 1)Bowen 2)Tao 3)Jinil

  try {
    (*arch_clt_) << fromPly;  // Send server our player's name

    do {
      if (!firstDone) {
        string firstRoundStr;
        (*arch_clt_) >> firstRoundStr;
        // Get File Path / mode / num_rounds
        string mode, num_str;
        int num_rounds;
        stringstream ss(firstRoundStr);
        ss >> inFile_srv_ >> mode >> num_str;
        num_rounds = atoi(num_str.c_str());

        cout << "F_Path: " << inFile_srv_ << "\nMode: " << mode << "\n# of rounds: "
          << num_rounds << endl;
        if (user == 1) {  // Setup bowen's Engine class
          engine_.ParseFile(inFile_srv_);
        }else if (user == 3){  // Setup jinil's Engine class
	  engine_jinil_.ParseFile(inFile_srv_);
	  engine_jinil_.setMode(mode_);
	}
        fromSrv = firstRoundStr.substr(firstRoundStr.find("\n") + 1);
	
	// Get Gamble Data File
	readSrvOutFile();
      
	firstDone = true;
        continue;
      }

      while (1) {
        if (fromSrv.find("\n") != string::npos) {
          if (!fromSrv.compare(0, 3, "OK\n")) {  // Just "OK", remove redundency
            fromSrv = fromSrv.substr(fromSrv.find("\n") + 1);
            continue;
          } else if (!fromSrv.compare(0, 5, "ERROR")) {
            return;  // error
          } else {
            break;
          }
        }
        (*arch_clt_) >> tmpFromSrv;
        fromSrv += tmpFromSrv;
        tmpFromSrv.clear();
      }
      cout << "#FromSrv: " << fromSrv << endl;

      ////////////////////////////////////////
      // Bowen, Tao  
      // Please insert your code in here
      // You can use 'gambles_' and 'links_' vector, which we read them from server output file
      // Please return to vector<double> betting_list
      //    ; index: gamble_id, double: fraction
      //    ; if you don't want to betting in gamble_id(j), 
      //        betting_list[j] = 0;
      ///////////////////////////////////////
      vector<double>* p_betting_list;
      
      // Bowen's engine
      if (user == 1) {
        if (!firstDone) {
          p_betting_list = engine_.makeDecision();
          fromPly = convertBettingListToString(*p_betting_list);
        } else {
          // Add gamble previous returns here
          // fromPly = convertBettingListToString(vector<double>(
            //     engine_.getNumOfStocks(), (1.0 / double(engine_.getNumOfStocks()))));
          vector<int> roundinfo(engine_.getNumOfStocks());
          double totalAssets = 0.0;
          if (readSrvGamebleReturns(fromSrv, roundinfo, totalAssets) == 1) {
            engine_.giveRoundInfo(roundinfo);
          }
          p_betting_list = engine_.makeDecision();
          fromPly = convertBettingListToString(*p_betting_list);
        }
      }
      // Jinil's engine
      else if (user == 3) {
	if(!firstDone){
	  p_betting_list = engine_jinil_.makeDecision();	
	  fromPly = convertBettingListToString(*p_betting_list);
	}else{
	  vector<int> roundinfo(engine_jinil_.getNumOfStocks());
          double totalAssets = 0.0;
          if (readSrvGamebleReturns(fromSrv, roundinfo, totalAssets) == 1) {
            engine_jinil_.giveRoundInfo(roundinfo);
          }
	  p_betting_list = engine_jinil_.makeDecision();	
	  fromPly = convertBettingListToString(*p_betting_list);
	}
      }

      cout << "Player sent: " << fromPly << endl;
      (*arch_clt_) << fromPly;

      fromPly.clear();
      fromSrv.clear();
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

}

/*
0 cout<<'h';
1 cout<<'m';
2 cout<<'l';
*/
int Game::readSrvGamebleReturns(const string& fromSrv, vector<int>& roundRets,
    double& totalAssets) const {
  istringstream ist(fromSrv);
  int index;
  char retChar;

  // Remove '['
  if (ist.get() != '[')
    return -1;
  while (1) {
    ist >> index;
    ist.get();
    ist >> retChar;
    switch (retChar) {
      case 'h': roundRets[index] = 0; break;
      case 'm': roundRets[index] = 1; break;
      case 'l': roundRets[index] = 2; break;
    }

    if (ist.peek() == ',') {
      ist.get();
      skipws(ist);
    } else {  // ']'
      ist.get();
      ist >> totalAssets;
      break;
    }
  }

  return 1;
}

ClientSocket* Game::registerSrv(int srv_port) {
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

void Game::readSrvOutFile() {  // Default file is @inFile_srv_
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
}

string Game::convertBettingListToString(vector<double> betting_list_){
  ostringstream ost;
  ost << "[";
  for (int i=0; i<betting_list_.size(); i++) {
    if (betting_list_[i] != 0.0) {
      ost << i << ":" << betting_list_[i];
    } else {
      ost << i << ":0.0";
    }
    if (i != (betting_list_.size() - 1)) {
      ost << ", ";
    }
  }
  ost << "]\n";

  return ost.str();
}
