#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Configuration.h"
#include "cppjieba/Jieba.hpp"
#include "simhash/Simhasher.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

class WordSegmentation {
   public:
    WordSegmentation()
        : _jieba(CONFIG[DICT_PATH], CONFIG[HMM_PATH], CONFIG[USER_DICT_PATH],
                 CONFIG[IDF_PATH], CONFIG[STOP_WORD_PATH]),
          _simhasher(CONFIG[DICT_PATH], CONFIG[HMM_PATH], CONFIG[IDF_PATH],
                     CONFIG[STOP_WORD_PATH]) {
        cout << ">> jieba init" << endl << ">> simhasher init" << endl;
    }

    vector<string> cutWords(const string& str) {
        vector<string> words;
        _jieba.Cut(str, words, true);
        return words;
    }

    // 制作指纹
    uint64_t makeSimhash(const string& content, size_t topN) {
        using namespace simhash;

        uint64_t hash;
        _simhasher.make(content, topN, hash);

        return hash;
    }

   private:
    cppjieba::Jieba _jieba;
    simhash::Simhasher _simhasher;
};