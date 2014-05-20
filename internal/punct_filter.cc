// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: huangyue@jike.com (Yue Huang)

#include "util/segmenter/internal/punct_filter.h"

#include "index/utils/token_utils.h"
#include "base/likely.h"

namespace util {
namespace segmenter {
PunctFilter::PunctFilter() : inited_(false) {
}

PunctFilter::~PunctFilter() {
}

bool PunctFilter::Load(const std::string& file_path) {
  return inited_ = dict_.LoadDictFile(file_path);
}

bool PunctFilter::Filtering(const char* src, int32 length) {
  uint64 hash = indexing::FNV64(src, length);
  return dict_.Find(hash);
}

bool PunctFilter::Filtering(const std::string& token) {
  return Filtering(token.c_str(), token.length());
}
}  // namespace segmenter
}  // namespace util
