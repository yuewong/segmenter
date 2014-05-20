# Copyright 2013 Makepolo Inc. All Rights Reserved.
# Author: huangyue@makepolo.com (Yue Huang)

cc_library(
    name = 'spliters',
    deps = [
            ':spliter_for_utf8',
            ':spliter_for_gbk',
            '//util/registrant/public:registrant',
           ],
)

cc_library(
    name = 'spliter_for_utf8',
    deps = [
           '//util/segmenter/spliter/internal:spliter_for_utf8_internal',
           ],
)

cc_library(
    name = 'spliter_for_gbk',
    deps = [
           '//util/segmenter/spliter/internal:spliter_for_gbk_internal',
           ],
)
