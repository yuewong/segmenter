// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_BASE_SEGMENTER_H_
#define UTIL_SEGMENTER_BASE_SEGMENTER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"

namespace util {
namespace segmenter {

struct SegmenterToken {
 SegmenterToken() : pos_(0), length_(0) {}
 SegmenterToken(int32 pos, int32 length) : pos_(pos), length_(length) {}
 int32 pos_;
 int32 length_;
};

namespace SupportEncoding {
enum Encoding {
  UTF8 = 1,
  GBK = 2,
  INVALID = 3
};
}

struct SegmenterParamter {
  SegmenterParamter() : enc_(SupportEncoding::GBK) {}
  SupportEncoding::Encoding enc_;
};

// segmenter base
class Segmenter {
 public:
  Segmenter(const SegmenterParamter& param = SegmenterParamter()) :
    paramter_(param) {}
  virtual ~Segmenter() {}

 public:
  void Feed(
      const std::string& sentence,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens = NULL) const {
    Preproccess(sentence);
    FeedInternal(sentence, words, tokens);
    Finalize(sentence, words, tokens);
  }

 protected:
  virtual void FeedInternal(
      const std::string& sentence,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens) const = 0;

  virtual void Preproccess(const std::string& sentence) const {
    // do nothing
  }

  virtual void Finalize(
      const std::string& sentence,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens = NULL) const {
    // do nothing
  }

 protected:
  SegmenterParamter paramter_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Segmenter);
};
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_BASE_SEGMENTER_H_
