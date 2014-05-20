// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_BEMS_ENUM_STR_MAP_H_
#define UTIL_SEGMENTER_PUBLIC_BEMS_ENUM_STR_MAP_H_

#include "util/enum_map/public/enum_map.h"
#include "util/segmenter/public/bems_emit_matrix.h"

BEGIN_ENUM_REGISTER(util::segmenter::State)
  ENUM_STRING_PAIR(util::segmenter::B, B);
  ENUM_STRING_PAIR(util::segmenter::E, E);
  ENUM_STRING_PAIR(util::segmenter::S, S);
  ENUM_STRING_PAIR(util::segmenter::M, M);
END_ENUM_REGISTER

#endif  // UTIL_SEGMENTER_PUBLIC_BEMS_ENUM_STR_MAP_H_
