#include "../include/dict.h"

#include <string.h>

#include <utility>
using std::cerr;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::transform;
using std::unordered_set;

DictProducer::DictProducer(const string& str) {
    vector<pair<string, int>> dict = buildEnDict(str);
    ofstream dictfile("../data/dictEn.dat");
    if (!dictfile.is_open()) {
        cerr << "无法打开文件" << "dictEn.dat" << "\n";
    }
    stringstream ss;
    for (const auto& pair : dict) {
        ss << pair.first << " " << pair.second << '\n';
    }
    string result = ss.str();
    dictfile << result << "\n";
    dictfile.close();

    map<char, set<int>> dictIndex = createEnIndex(dict);
    ofstream dictIndexfile("../data/dictIndexEn.dat");
    if (!dictIndexfile.is_open()) {
        cerr << "无法打开文件" << "dictIndexEn.dat" << "\n";
    }
    stringstream ssIndex;
    for (const auto& pair : dictIndex) {
        ssIndex << pair.first << " ";
        for (auto str : pair.second) {
            ssIndex << str << " ";
        }
        ssIndex << '\n';
    }

    string resultIndex = ssIndex.str();
    dictIndexfile << resultIndex << "\n";
    dictIndexfile.close();

    std::cout << "内容已写入文件" << "\n";
}

// DictProducer::DictProducer(const string& dir, SplitTool* splittool) {

// }

unordered_set<string> loadStopwords() {
    unordered_set<string> stopwords;
    ifstream file("stop_words_eng.txt");
    string stop_word;
    if (file.is_open()) {
        while (file >> stop_word) {
            stopwords.insert(stop_word);
        }
        file.close();
    } else {
        cerr << "无法使用停用词文件\n";
    }
    return stopwords;
}

bool isStopword(const string& word, const unordered_set<string>& stopwords) {
    return stopwords.find(word) != stopwords.end();
}

unordered_map<string, int> xiabiao;

vector<pair<string, int>> DictProducer::buildEnDict(const string& str) {
    vector<pair<string, int>> dictionary;
    ifstream file(str);
    unordered_set<string> stopwords = loadStopwords();
    string line, word;

    int idx = 0;

    if (!file.is_open()) {
        cerr << "无法打开文件" << str << "\n";
    }

    while (getline(file, line)) {
        istringstream iss(line);
        while (iss >> word) {
            // 转换成小写
            transform(word.begin(), word.end(), word.begin(), tolower);

            // 把标点符号转换成空格
            transform(word.begin(), word.end(), word.begin(),
                      [](char ch) -> char {
                          if (ispunct(ch) || (ch == '\r')) {
                              return ' ';
                          }
                          return ch;
                      });

            transform(word.begin(), word.end(), word.begin(), [](char ch) {
                return isdigit(ch) ? ' ' : ch;  // 如果字符是数字，替换为空格
            });

            if (isStopword(word, stopwords)) {
                word = ' ';
            }

            // 去掉空格
            word.erase(remove_if(word.begin(), word.end(),
                                 [](char ch) { return ch == ' '; }),
                       word.end());

            // 更新词典
            auto it = find_if(dictionary.begin(), dictionary.end(),
                              [&word](const pair<string, int>& pair) {
                                  return pair.first == word;
                              });
            if (it != dictionary.end()) {
                it->second++;
            } else {
                dictionary.push_back(make_pair(word, 1));
                xiabiao[word] = idx++;
            }
        }
    }

    file.close();
    return dictionary;
}

int main() {
    DictProducer dict("english.txt");

    return 0;
}

map<string, int> DictProducer::buildCnDict() {}

map<char, set<int>> DictProducer::createEnIndex(
    vector<pair<string, int>> dictionary) {
    map<char, set<int>> Index;

    for (const auto& pair : dictionary) {
        string word = pair.first;
        set<int> danciset;
        for (char letter : word) {
            Index[letter].insert(xiabiao[word]);
        }
    }

    return Index;
}

// vector<pair<string, int>> DictProducer::createCnIndex() {
//     // 分隔中英文
//     string str = "hi你好";
//     for (size_t i = 0; i < str.size();) {
//         // 1000 0000 & ch
//         if ((str[i] & 0x80) == 0) {
//             cout << "英文：" << str.substr(i, 1) << "\n";
//             ++i;
//         } else {
//             cout << "中文：" << str.substr(i, 3) << "\n";
//             i += 3;
//         }
//     }
//     return 0;
// }

// void DictProducer::store() {}