#include "PageLibPreprocessor.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>

using std::cout;
using std::make_pair;
using std::ofstream;

PageLibPreprocessor::PageLibPreprocessor() { _pageLib.reserve(300); }

void PageLibPreprocessor::doProcess() {
    readPageFromFile();

    // 计算每个步骤需要花的时间
    auto start = std::chrono::system_clock::now();  // 获取当前时间点
    cutRedundantPages();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cout << ">> cut redundant pages success, use time: "
         << elapsed_seconds.count() << " sec" << "\n";

    buildInvertIndex();
    auto end2 = std::chrono::system_clock::now();
    elapsed_seconds = end2 - end;
    cout << ">> build invert index success, use time: "
         << elapsed_seconds.count() << " sec" << "\n";

    elapsed_seconds = end2 - start;
    cout << ">> total time cost: " << elapsed_seconds.count() << " sec" << "\n";
    store();
}

// 从文件中读取数据存入_pageLib
void PageLibPreprocessor::readPageFromFile() {
    XMLDocument doc;
    doc.LoadFile(CONFIG[RIPEPAGE_PATH].c_str());
    XMLElement* page = doc.FirstChildElement("doc");

    do {
        string docid = page->FirstChildElement("docid")->GetText();
        string title = page->FirstChildElement("title")->GetText();
        string link = page->FirstChildElement("link")->GetText();
        string content = page->FirstChildElement("content")->GetText();

        _pageLib.emplace_back(std::stoi(docid), title, link, content);
    } while (page = page->NextSiblingElement());
}

// 去重
void PageLibPreprocessor::cutRedundantPages() {
    simhash::Simhasher simhasher(CONFIG[DICT_PATH], CONFIG[HMM_PATH],
                                 CONFIG[IDF_PATH], CONFIG[STOP_WORD_PATH]);

    cout << ">> before cut: " << _pageLib.size() << endl;

    for (auto& page : _pageLib) {
        page.generateSimhash(_jieba);
    }
    // 先排序，因为unique只能把相邻的重复页面移到后面，并返回新的末尾迭代器
    std::sort(_pageLib.begin(), _pageLib.end());

    auto it = std::unique(_pageLib.begin(), _pageLib.end());
    // 配合erase删除这些重复的页面
    _pageLib.erase(it, _pageLib.end());
    cout << ">> after cut: " << _pageLib.size() << endl;
}

void PageLibPreprocessor::buildInvertIndex() {
    for (auto& page : _pageLib) {
        page.buildWordsMap(_jieba);
    }

    for (auto& page : _pageLib) {
        unordered_map<string, int> wordsMap = page.getWordsMap();
        for (auto& wordFreq : wordsMap) {
            _invertIndexTable[wordFreq.first].push_back(
                make_pair(page.getDocId(), wordFreq.second));  // 单词
        }
    }

    // 保存每篇文档中所有词的权重的平方和, int为docid，之后去做归一化处理
    unordered_map<int, double> weightSum;

    int totalPageNum = _pageLib.size();
    for (auto& elem : _invertIndexTable) {
        int df = elem.second.size();  // 关键词在所有文章中出现的次数
        double idf = log2(static_cast<double>(totalPageNum /
                                              (df + 1)));  // 关键词的逆文档频率

        for (auto& item : elem.second) {
            double weight = item.second * idf;
            item.second = weight;
            // 计算每篇文档中词语的权重
            weightSum[item.first] += pow(weight, 2);
        }
    }

    for (auto& elem : _invertIndexTable) {
        for (auto& item : elem.second) {
            // 归一化处理
            item.second = item.second / sqrt(weightSum[item.first]);
        }
    }
}

void PageLibPreprocessor::store() {
    string declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    XMLDocument xmlpages;
    // 反序列化，将json字符串转换为数据结构
    xmlpages.Parse(declaration.c_str());

    for (auto& page : _pageLib) {
        page.insertDoc(xmlpages);
    }

    xmlpages.SaveFile(CONFIG[NEW_RIPEPAGE_PATH].c_str());
    cout << ">> store new ripepage success" << endl;

    ofstream ofsIndex(CONFIG[INDEX_PATH]);

    for (auto& elem : _invertIndexTable) {
        ofsIndex << elem.first << '\t';
        for (auto& item : elem.second) {
            ofsIndex << item.first << '\t' << item.second << '\t';
        }
        ofsIndex << '\n';
    }

    ofsIndex.close();
    cout << ">> store invert index success" << endl;
}