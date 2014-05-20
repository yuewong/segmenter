// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/internal/state_trans_matrix.h"

#include "base/likely.h"
#include "base/string_util.h"
#include "util/file/public/simple_file_reader.h"
#include "util/segmenter/public/bems_enum_str_map.h"

namespace util {
namespace segmenter {
StateTransMatrix::StateTransMatrix() {
  for (int i = 0; i < kStateNum; ++i) {
    for (int j = 0; j < kStateNum; ++j) {
      state_trans_matrix_[i][j] = -3.14e10;
    }
  }
}

bool StateTransMatrix::LoadTransProb(const std::string& file) {
  util::file::SimpleFileReader reader(file);
  std::string line;

  State a;
  State b;
  float freq;

  while (reader.ReadLine(&line)) {
    if (UNLIKELY(!ParserStateTrans(line, &a, &b, &freq))) {
      continue;
    }
    state_trans_matrix_[a][b] = freq;
  }
  return true;
}

bool StateTransMatrix::ParserStateTrans(
    const std::string& line,
    State* a,
    State* b,
    float* freq) {
  std::vector<std::string> v;
  SplitString(line, StateTransMatrix::kDefaultSplitSymbol, &v);

  if (v.size() != StateTransMatrix::kDefaultRowNum) {
    return false;
  }

  if (UNLIKELY(!EnumStringParser::String2Enum(v[STATE_A], a))) {
    return false;
  }

  if (UNLIKELY(!EnumStringParser::String2Enum(v[STATE_B], b))) {
    return false;
  }

  double dfreq;
  if (UNLIKELY(!StringToDouble(v[PROB], &dfreq))) {
    return false;
  }
  *freq = dfreq;

  return true;
}

float StateTransMatrix::GetTransProb(State a, State b) const {
  return state_trans_matrix_[a][b];
}
}  // segmenter
}  // util
