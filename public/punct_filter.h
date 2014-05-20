// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_PUNCT_FILTER_H_
#define UTIL_SEGMENTER_PUBLIC_PUNCT_FILTER_H_

#include "base/hash_tables.h"
#include "util/dict/public/string_dict.h"
#include "util/segmenter/public/tokenid_dict.h"

namespace util {
namespace segmenter {

class PunctFilter {
 public:
  PunctFilter();
  ~PunctFilter();

 bool Load(const std::string& file_path);
 bool Filtering(const std::string& token);
 bool Filtering(const char* src, int32 length);

 private:
  bool inited_;
  TokenIDDict dict_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PunctFilter);
};

}  // namespace segmenter
}  // namespace util
#endif  // UTIL_SEGMENTER_PUBLIC_PUNCT_FILTER_H_
