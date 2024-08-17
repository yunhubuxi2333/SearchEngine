#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

class SplitTool {
    vector<string> cut() {}
};

class DictProducer {
   public:
    DictProducer(const string& str) {}
    DictProducer(const string& dir, SplitTool* splittool) {}
    void buildEnDict() {}
    void buildCnDict() {}
    void createIndex() {}
    void store() {}

   private:
    vector<string> _files;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;
    SplitTool* _cuttor;
};

class DictIndexProducer {};

class Configuration {};

class SplitToolCppJieba : public SplitTool {
    SplitTool* cut();
};