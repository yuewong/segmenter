// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: huangyue@jike.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_TOKENID_DICT_H_
#define UTIL_SEGMENTER_PUBLIC_TOKENID_DICT_H_

#include "base/basictypes.h"
#include "util/dict/public/dict_base.h"
#include "util/sparsehash/public/dense_hash_set.h"

namespace util {
namespace segmenter {

class TokenIDDict : public util::dict::NoPayloadDict<uint64> {
 public:
  TokenIDDict();
  virtual ~TokenIDDict() {}

 public:
  bool Find(const uint64& hash) const;

 protected:
  bool ProcessLineInternal(const std::string& key);

 private:
  ::util::google::dense_hash_set<uint64> dict_;
};

}  // namespace segmenter
}  // namespace util
#endif  // UTIL_SEGMENTER_PUBLIC_TOKENID_DICT_H_
