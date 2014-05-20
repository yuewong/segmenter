// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/internal/hidden_markov_model.h"

#include "base/logging.h"
#include "util/segmenter/base/utils.h"
#include "util/segmenter/public/bems_emit_matrix.h"
#include "util/segmenter/public/start_prob_matrix.h"
#include "util/segmenter/public/state_trans_matrix.h"

namespace util {
namespace segmenter {
HiddenMarkovModel::HiddenMarkovModel() : inited_(false) {
  emit_matrix_.reset(new BemsEmitMatrix());
  CHECK(emit_matrix_.get());
  state_trans_matrix_.reset(new StateTransMatrix());
  CHECK(state_trans_matrix_.get());
  start_prob_.reset(new StartProbMatrix());
  CHECK(start_prob_.get());
}

HiddenMarkovModel::~HiddenMarkovModel() {
}

void HiddenMarkovModel::Viterbi(
    const std::vector<int32>& simple_char_code,
    std::pair<int, int>& range,
    std::vector<int>* final_path) const {

  // BMES
  static const int32 STATE_B = 0;
  static const int32 STATE_M = 1;
  static const int32 STATE_E = 2;
  static const int32 STATE_S = 3;

  static const int32 STATE_NUM = 4;
  static const int32 TRANS_NUM = 2;

  static int prev_status[STATE_NUM][TRANS_NUM] = {
    {STATE_E, STATE_S}, // B : E, S
    {STATE_M, STATE_B}, // M : M, B
    {STATE_B, STATE_M}, // E : B, M
    {STATE_S, STATE_E}, // S : S, E
  };

  SimpleTwoDimArray<float> v(range.second - range.first + 1, STATE_NUM);
  SimpleTwoDimArray<int> best_path(range.second - range.first + 1, STATE_NUM);

  for (int i = 0; i < STATE_NUM; ++i) {
    float freq = 0.0f;
    emit_matrix_->GetEmitProb((State)i, simple_char_code[range.first], &freq);
    v.Value(0, i) = start_prob_->GetStartProb((State)i) + freq;
    best_path.Value(0, i) = i;
  }

  for (int i = range.first + 1; i <= range.second; ++i) {
    for (int j = 0; j < STATE_NUM; ++j) {
      float em_p = 0.0f;
      emit_matrix_->GetEmitProb((State)j, simple_char_code[i], &em_p);
      float max_freq = -std::numeric_limits<float>::max();
      int max_freq_state;
      for (int y = 0; y < TRANS_NUM; ++y) {
        int y0 = prev_status[j][y];
        float tran_freq = state_trans_matrix_->GetTransProb((State)y0, (State)j);
        float freq = v.Value(i - range.first - 1, y0) + em_p + tran_freq;
        if (freq > max_freq) {
          max_freq = freq;
          max_freq_state = y0;
        }
      }
      v.Value(i - range.first, j) = max_freq;
      best_path.Value(i - range.first, j) = max_freq_state;
    }
  }

  float freq = -std::numeric_limits<float>::max();
  int final_state = -1;
  for (int y = STATE_E; y < STATE_NUM; ++y) {
    if (freq < v.Value(range.second - range.first, y)) {
      freq = v.Value(range.second - range.first, y);
      final_state = y;
    }
  }

  if (final_state == -1) {
    final_path->clear();
    return;
  }
  final_path->push_back(final_state);
  for (int i = range.second - range.first; i > 0; --i) {
    final_path->push_back(best_path.Value(i, final_state));
    final_state = best_path.Value(i, final_state);
  }

  std::reverse(final_path->begin(), final_path->end());
}

bool HiddenMarkovModel::Init(
    const std::string& start_prob_file,
    const std::string& tran_prob_file,
    const std::string& emit_prob_file) {
  if (!LoadEmitMatrix(emit_prob_file)) {
    return false;
  }

  if (!InitStartProb(start_prob_file)) {
    return false;
  }

  if (!InitTranProb(tran_prob_file)) {
    return false;
  }
  DLOG(INFO) << "HiddenMarkovModel init success";
  return true;
}

bool HiddenMarkovModel::InitStartProb(const std::string& file_path) {
  return start_prob_->LoadStartProb(file_path);
}

bool HiddenMarkovModel::InitTranProb(const std::string& file_path) {
  return state_trans_matrix_->LoadTransProb(file_path);
}

bool HiddenMarkovModel::LoadEmitMatrix(const std::string& file_path) {
  return emit_matrix_->LoadModel(file_path);
}
}  // segmenter
}  // util
