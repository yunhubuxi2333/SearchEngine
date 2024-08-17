#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

class Configuration {
   public:
    Configuration* getInstance() = delete;
    map<string, string>& getConfigMap();

   private:
    string _configFilePath;
    map<string, string> _configs;
};

class DirScanner {
   public:
    vector<string>& getFile();
    void traverse(string);  // todo dir:string
};

class PageLib {
   public:
    PageLib();
    void create();
    void store();

   private:
    DirScanner _dirScanner;
    vector<string> _pages;
    map<int, pair<int, int>> _offsetLib;
};
class WebPage {};
class SplitTool {};
class PageLibPreProcessor {
   public:
    void cutRedundantPage();
    void buildInvertIndexMap();
    void storeOnDisk();

   private:
    vector<WebPage> _pageList;
    unordered_map _offsetLib;
    unordered_map _invertIndexLib;
    SplitTool* wordCutter;
};