// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: huangyue@jike.com (Yue Huang)

#include "util/segmenter/internal/tokenid_dict.h"

#include "base/likely.h"
#include "index/utils/token_utils.h"

namespace util {
namespace segmenter {
TokenIDDict::TokenIDDict() {
  dict_.set_empty_key(0);
}

bool TokenIDDict::Find(const uint64& hash) const {
  return dict_.find(hash) != dict_.end();
}


bool TokenIDDict::ProcessLineInternal(const std::string& key) {
  if (UNLIKELY(key.empty())) {
    return true;
  }

  uint64 hash = indexing::FNV64(key.c_str(), key.size());
  dict_.insert(hash);
  return true;
}
}  // namespace segmenter
}  // namespace util
