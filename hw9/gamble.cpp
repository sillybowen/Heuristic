#include "gamble.h"
using namespace std;

Gamble::Gamble(int gamble_id_, int class_id_, float h_ret_, float h_prob_,
	 float m_ret_, float m_prob_, float l_ret_, float l_prob_)
    :gamble_id(gamble_id_), class_id(class_id_), h_ret(h_ret_), h_prob(h_prob_),
    m_ret(m_ret_), m_prob(m_prob_), l_ret(l_ret_), l_prob(l_prob_) 
{ 
  expect_ret = 0;
  previous_ret = 0;
  calculateExpectRet();
}

void Gamble::calculateExpectRet(){
  if(h_prob >= m_prob && h_prob >= l_prob)
    expect_ret = h_ret;
  else if(m_prob >= h_prob && m_prob >= l_prob)
    expect_ret = m_ret;
  else
    expect_ret = l_ret;
  if(previous_ret != 0)
    expect_ret = (expect_ret + previous_ret) * 2;
  
}

void Gamble::increaseHighProb(){
  l_prob /= 2;
  h_prob += l_prob;
}

void Gamble::increaseLowProb(){
  h_prob /= 2;
  l_prob += h_prob;
}

void Gamble::updatePreviousRet(int type){
  if(type == 1)
    previous_ret = h_ret;
  else if(type == 0)
    previous_ret = m_ret;
  else
    previous_ret = l_ret;
}
