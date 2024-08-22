#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class DirScanner {
   public:
    DirScanner();

    void operator()();
    vector<string>& getFiles();
    // 遍历
    void traverse(const string& dirPath);

   private:
    vector<string> _files;  // 存放每个xml的绝对路径
};