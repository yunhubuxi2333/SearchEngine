#pragma once
#include <vector>

#include "WebPage.h"
using std::pair;
using std::vector;

class PageLibPreprocessor {
   public:
    PageLibPreprocessor();

    void doProcess();
    void readPageFromFile();
    void cutRedundantPages();
    void buildInvertIndex();
    void store();

   private:
    WordSegmentation _jieba;
    vector<WebPage> _pageLib;
    unordered_map<string, vector<pair<int, double>>>
        _invertIndexTable;  // 单词，文章id，权重
};
