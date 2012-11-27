#include <cassert>
#include <cstring>
#include <iostream>
#include "init_choice.h"
using std::cout;
using std::endl;

InitChoice::InitChoice(int numGambles, int numLinks, const vector<Gamble*>& gambles,
    const vector<Link*>& links) : n_gambles_(numGambles),
                                  n_links_(numLinks),
                                  gTo_cla_(new int[numGambles]),
                                  claTo_g_(new vector<int>[numGambles]),
                                  g_data_(&gambles),
                                  l_data_(&links) {
  assert(n_gambles_ == (int)(g_data_->size()));
  // Set gamble_id -> class belonging init values
  for (int i = 0; i < n_gambles_; ++i)
    gTo_cla_[i] = -1;

  // Initialize matrix graph for gamble links
  link_matr_ = new int*[n_gambles_];
  for (int i = 0; i < n_gambles_; ++i) {
    link_matr_[i] = new int[n_gambles_];
    memset(link_matr_[i], 0, sizeof(int) * n_gambles_);
  }

  exp_g_.resize(n_gambles_);
  var_g_.resize(n_gambles_);
  calExpVarsOfGambles();
  initLinksMatrGraph();
}

InitChoice::~InitChoice() {
  for (int i = 0; i < n_gambles_; ++i)
    delete [] link_matr_[i];
  delete [] link_matr_;
  delete [] gTo_cla_;
  delete [] claTo_g_;
}

void InitChoice::calExpVarsOfGambles() {
  for (int i = 0; i < n_gambles_; ++i) {
    // Add class belonging first
    gTo_cla_[g_data_->at(i)->gamble_id] = g_data_->at(i)->class_id;
    claTo_g_[g_data_->at(i)->class_id].push_back(g_data_->at(i)->gamble_id);

    int gambleId = g_data_->at(i)->gamble_id;
    exp_g_[gambleId] = g_data_->at(i)->h_ret * g_data_->at(i)->h_prob
      + g_data_->at(i)->m_ret * g_data_->at(i)->m_prob
      + g_data_->at(i)->l_ret * g_data_->at(i)->l_prob;

    var_g_[gambleId] =
      g_data_->at(i)->h_prob * (g_data_->at(i)->h_ret - exp_g_[gambleId]) *
        (g_data_->at(i)->h_ret - exp_g_[gambleId])
    + g_data_->at(i)->m_prob * (g_data_->at(i)->m_ret - exp_g_[gambleId]) *
        (g_data_->at(i)->m_ret - exp_g_[gambleId])
    + g_data_->at(i)->l_prob * (g_data_->at(i)->l_ret - exp_g_[gambleId]) *
        (g_data_->at(i)->l_ret - exp_g_[gambleId]);
  }
}

void InitChoice::printClassBelongMap() const {
  for (int i = 0; i < n_gambles_; ++i) {
    if (!claTo_g_[i].size())
      continue;
    cout << "Gambles belongs to classID: " << i << ": ";
    for (int j = 0; j < claTo_g_[i].size(); ++j) {
      cout << claTo_g_[i][j] << " ";
    }
    cout << endl;
  }
}

void InitChoice::initLinksMatrGraph() {
  assert(n_links_ == (int)(l_data_->size()));
  for (int i = 0; i < n_links_; ++i) {
    link_matr_[l_data_->at(i)->gi][l_data_->at(i)->gj] = 1;
    link_matr_[l_data_->at(i)->gj][l_data_->at(i)->gi] = 1;
  }
}
