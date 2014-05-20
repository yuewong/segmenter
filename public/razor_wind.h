// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_RAZORWIND_H_
#define UTIL_SEGMENTER_PUBLIC_RAZORWIND_H_

#include "base/scoped_ptr.h"
#include "util/segmenter/base/segmenter.h"

namespace util {
namespace segmenter {

// segmenter base
class RazorWind : public Segmenter {
 public:
  RazorWind(const SegmenterParamter& param = SegmenterParamter());
  virtual ~RazorWind();

 public:
  void FeedInternal(
      const std::string& sentence,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens) const;

 protected:
  void Finalize(
      const std::string& sentence,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens = NULL) const;

 private:
  class Impl;
  scoped_ptr<Impl> impl_;

 private:
  DISALLOW_COPY_AND_ASSIGN(RazorWind);
};
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_PUBLIC_RAZORWIND_H_
