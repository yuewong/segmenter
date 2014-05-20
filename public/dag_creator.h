// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_DAG_CREATOR_H_
#define UTIL_SEGMENTER_PUBLIC_DAG_CREATOR_H_

#include <vector>

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "util/segmenter/base/spliter.h"
#include "util/segmenter/public/dag.h"
#include "util/segmenter/base/segmenter.h"

namespace util {
namespace segmenter {

class DagCreator {
 public:
  DagCreator(const SegmenterParamter* param);
  ~DagCreator();

 public:
  bool LoadDict();

  void Feed(
      const std::string& sentent,
      std::vector<std::string>* words,
      Dag* dag,
      Spliter* spliter) const;

 private:
  bool BuildTrie();

 private:
  class Impl;
  scoped_ptr<Impl> impl_;

 private:
   DISALLOW_COPY_AND_ASSIGN(DagCreator);
};

}  // namespace file
}  // namespace util
#endif  // UTIL_FILE_PUBLIC_LINUX_POSIX_FILE_H_
