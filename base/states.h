// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_BASE_STATES_H_
#define UTIL_SEGMENTER_BASE_STATES_H_

#include "base/basictypes.h"

namespace util {
namespace segmenter {

typedef enum {
  B = 0,
  M = 1,
  E = 2,
  S = 3,
} State;

}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_BASE_STATES_H_
