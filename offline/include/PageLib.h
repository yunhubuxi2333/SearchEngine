#pragma once
#include <map>
#include <vector>
#include <string>
#include "DirScanner.h"

using std::pair;
using std::map;

class PageLib {
   public:
    PageLib(DirScanner& scanner);
    void create();
    void store();

   private:
    DirScanner& _dirScanner;
    vector<string> _pages;
    map<int, pair<int, int>> _offsetLib;
};