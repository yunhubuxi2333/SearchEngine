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
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

class SplitTool {
   public:
    virtual ~SplitTool() {}  // 虚析构函数
    virtual vector<string> cut(
        const string& sentence) = 0;  // 分词函数，纯虚函数提供接口
};

// 单例模式是一种常用的设计模式，用于确保一个类只有一个实例，并提供一个全局访问点。在C++中，实现单例模式通常涉及以下几个步骤：

// 私有构造函数：确保单例类的构造函数是私有的，这样外部就不能直接创建该类的实例。
// 静态实例：使用静态变量来存储单例的实例，这样可以通过类名直接访问这个实例。
// 公有访问器：提供一个公有方法，用于创建（如果还没有实例）或返回单例的实例。
class SplitToolCppJieba : public SplitTool {
   public:
    SplitToolCppJieba(SplitToolCppJieba&) = delete;  // 拷贝构造
    SplitToolCppJieba& operator=(SplitToolCppJieba&) = delete;
    static void destroy();
    virtual vector<string> cut(const string& sentence);
    // 公有静态方法，用于获取单例的实例
    static SplitToolCppJieba* getInstance();

   private:
    SplitToolCppJieba() {}
    virtual ~SplitToolCppJieba() {}  // 虚析构函数
    SplitTool* _cuttor;
    static SplitToolCppJieba* _pInstance;
    Configuration& _conf;
};