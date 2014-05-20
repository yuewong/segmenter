// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/internal/start_prob_matrix.h"

#include "base/likely.h"
#include "base/string_util.h"
#include "util/file/public/simple_file_reader.h"
#include "util/segmenter/public/bems_enum_str_map.h"

namespace util {
namespace segmenter {
StartProbMatrix::StartProbMatrix() {
  for (int i = 0; i < kStateNum; ++i) {
    start_prob_[i] = 0.0f;
  }
}

bool StartProbMatrix::LoadStartProb(const std::string& file) {
  util::file::SimpleFileReader reader(file);
  std::string line;
  State state;
  float freq;

  while (reader.ReadLine(&line)) {
    if (UNLIKELY(!ParserStartProb(line, &state, &freq))) {
      continue;
    }
    start_prob_[state] = freq;
  }
  return true;
}

float StartProbMatrix::GetStartProb(State state) const {
  return start_prob_[state];
}

bool StartProbMatrix::ParserStartProb(
    const std::string& line,
    State* state,
    float* freq) {
  std::vector<std::string> v;
  SplitString(line, StartProbMatrix::kDefaultSplitSymbol, &v);
  if (v.size() != StartProbMatrix::kDefaultRowNum) {
    return false;
  }

  if (UNLIKELY(!EnumStringParser::String2Enum(v[STATE], state))) {
    return false;
  }

  double dfreq;
  if (UNLIKELY(!StringToDouble(v[PROB], &dfreq))) {
    return false;
  }
  *freq = dfreq;
  return true;
}
}  // segmenter
}  // util
