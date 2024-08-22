#pragma once
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cout;
using std::map;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

#define XML_PATH "xmlDir"
#define RIPEPAGE_PATH "ripepageLib"
#define OFFSET_PATH "offsetLib"
#define NEW_RIPEPAGE_PATH "newRipepageLib"
#define NEW_OFFSET_PATH "newOffsetLib"
#define INDEX_PATH "invertIndex"
#define DICT_PATH "dict"
#define HMM_PATH "hmm_model"
#define IDF_PATH "idf"
#define STOP_WORD_PATH "stop_words"
#define USER_DICT_PATH "user_dict"

#define CONFIG Configuration::getInstance()->getConfigMap()

class Configuration {
   public:
    static Configuration* getInstance();
    static Configuration* getInstance(const string& filepath);
    static void destroy();

    unordered_map<string, string>& getConfigMap() { return _configmap; }

   private:
    Configuration(const string& filepath);
    ~Configuration() { cout << "~Configuration()\n"; }
    void readFile(const string& filename);

   private:
    string _filepath;
    static Configuration* _pInstance;
    map<string, string> _configs;
    unordered_map<string, string>& _configmap;
};

// class PageLib {
//    public:
//     PageLib();
//     void create();
//     void store();

//    private:
//     DirScanner _dirScanner;
//     vector<string> _pages;
//     map<int, pair<int, int>> _offsetLib;
// };

class WebPage {};

// class PageLibPreProcessor {
//    public:
//     void cutRedundantPage();
//     void buildInvertIndexMap();
//     void storeOnDisk();

//    private:
//     vector<WebPage> _pageList;
//     SplitTool* wordCutter;
// };