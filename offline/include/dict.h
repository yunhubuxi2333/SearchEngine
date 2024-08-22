#pragma once
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Configuration.h"
#include "SplitTool.h"

using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

class DictProducer {
   public:
    DictProducer(const string& str, bool flag);

    vector<pair<string, int>> buildEnDict(const string& str);
    void buildCnDict(const string& str);
    map<char, set<int>> createEnIndex(vector<pair<string, int>> dictionary);
    void createCnIndex();

    void loadCnStopwords();
    void store();
    size_t getByteNum_UTF8(const char byte);
    vector<string> getFiles(const string& path);
    void pushDict(const string& word);

    // 将英文词典和索引写入文件内
    void EndictToFile(const string& str);
    // 将中文词典和索引写入文件内
    void CndictToFile(const string& str);

   private:
    vector<string> _files;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;

    unordered_set<string> _stopwords;
};
