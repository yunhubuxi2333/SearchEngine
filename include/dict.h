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

using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

class SplitTool {
    vector<string> cut() {}
};

class DictProducer {
   public:
    DictProducer(const string& str);
    DictProducer(const string& dir, SplitTool* splittool);
    vector<pair<string, int>> buildEnDict(const string& str);
    map<string, int> buildCnDict();
    map<char, set<int>> createEnIndex(vector<pair<string, int>> dictionary);
    map<string, set<string>> createCnIndex();
    void store();

   private:
    vector<string> _files;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;
    SplitTool* _cuttor;
};

class Configuration {};

class SplitToolCppJieba : public SplitTool {
    SplitTool* cut();
};