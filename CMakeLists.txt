CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
project(ngrams)

add_executable(ngrams ByteNgrams.h ByteNgrams.cpp CharNgrams.h CharNgrams.cpp INgrams.h INgrams.cpp ngrams.cpp ngrams.h ternarySearchTree.h text2wfreq.h text2wfreq.cpp WordNgrams.h WordNgrams.cpp config.h vector.h mystring.h string.cpp)