// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/internal/bems_emit_matrix.h"

#include <limits>

#include "base/likely.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "util/file/public/simple_file_reader.h"
#include "util/segmenter/base/utils.h"
#include "util/segmenter/public/bems_enum_str_map.h"

namespace util {
namespace segmenter {

BemsEmitMatrix::BemsEmitMatrix() : is_loaded_(false), emit_num_(0) {
  for (int i = 0; i < kStateNum; ++i) {
    emit_matrix_[i] = new EmitMatrix(kDefaultEmitMatrixSize);
    CHECK(emit_matrix_[i]) << "EmitMatrix alloc mem failed";
    emit_matrix_[i]->set_empty_key(0);
  }
}

BemsEmitMatrix::~BemsEmitMatrix() {
  for (int i = 0; i < kStateNum; ++i) {
    delete emit_matrix_[i];
  }
}

bool BemsEmitMatrix::ParserStateEmit(
    const std::string& line,
    State* state,
    int32* code,
    float* freq) {
  std::vector<std::string> v;
  SplitString(line, BemsEmitMatrix::kDefaultSplitSymbol, &v);
  if (v.size() != kDefaultMatrixNum) {
    return false;
  }

  if (UNLIKELY(!EnumStringParser::String2Enum(v[STATE_NAME], state))) {
    return false;
  }

  if (UNLIKELY(!ConvertToUtf8Code(v[CHINESE_CHAR], code))) {
    return false;
  }

  double dfreq;
  if (UNLIKELY(!StringToDouble(v[PROB], &dfreq))) {
    return false;
  }
  *freq = dfreq;

  return true;
}

bool BemsEmitMatrix::LoadModel(const std::string& file_path) {
  util::file::SimpleFileReader reader(file_path);

  std::string line;
  State state;
  float freq;
  int32 code;

  while (reader.ReadLine(&line)) {
    if (UNLIKELY(!ParserStateEmit(line, &state, &code, &freq))) {
      continue;
    }
    (*emit_matrix_[state])[code] = freq;
    emit_num_++;
  }
  return emit_num_ != 0;
}

bool BemsEmitMatrix::GetEmitProb(
    State state,
    const std::string& str,
    float *prob) const {
  int32 code;
  if (UNLIKELY(!ConvertToUtf8Code(str, &code))) {
    return false;
  }
  return GetEmitProb(state, code, prob);
}

bool BemsEmitMatrix::GetEmitProb(State state, int32 code, float *prob) const {
  if (UNLIKELY(state >= kStateNum)) {
    return false;
  }

  EmitMatrix::const_iterator c_iter = emit_matrix_[state]->find(code);
  if (c_iter == emit_matrix_[state]->end()) {
    *prob = -std::numeric_limits<float>::max();
  } else {
    *prob = c_iter->second;
  }

  return true;
}
}  // segmenter
}  // util
