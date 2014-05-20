// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_SPLITER_PUBLIC_SPLITER_FOR_GBK_H_
#define UTIL_SEGMENTER_SPLITER_PUBLIC_SPLITER_FOR_GBK_H_

#include "base/basictypes.h"
#include "util/segmenter/base/spliter.h"

namespace util {
namespace segmenter {

class SpliterForGBK : public Spliter {
 public:
  SpliterForGBK() {}
  virtual ~SpliterForGBK() {}

 public:
  virtual void SpiltSimpleChar(
      const std::string& sentence,
      std::vector<std::pair<int, int> >* simple_char_pos,
      std::vector<int>* simple_char_code);

  virtual bool GetNextChar(
      const char* data,
      int start_pos,
      int length,
      int* end_pos,
      int* code);

  virtual bool IsPunct(const std::string& str);
  virtual bool IsPunct(const char* data, int32 length);

 private:
  int32 GetGbkCode(int32 high, int32 low) {
    return high << 8 | low;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SpliterForGBK);
};

}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_SPLITER_PUBLIC_SPLITER_FOR_GBK_H_
