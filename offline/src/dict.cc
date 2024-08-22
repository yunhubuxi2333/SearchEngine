#include "../include/dict.h"

#include <dirent.h>
#include <string.h>

#include <fstream>
#include <ios>
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

DictProducer::DictProducer(const string& str, bool flag) {
    if (flag == 1) {
        EndictToFile(str);
    } else if (flag == 0) {
        CndictToFile(str);
    }
}

void DictProducer::EndictToFile(const string& str) {
    // 把英文词典写入文件中
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

    // 把英文词典索引写入到文件中
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

void DictProducer::CndictToFile(const string& str) {
    buildCnDict(str);
    ofstream dictfile("../data/dictCn.dat");
    if (!dictfile.is_open()) {
        cerr << "无法打开文件" << "dictCn.dat" << "\n";
    }

    stringstream ss;
    for (const auto& pair : _dict) {
        ss << pair.first << " " << pair.second << '\n';
    }

    string result = ss.str();
    dictfile << result << "\n";
    dictfile.close();

    createCnIndex();
    ofstream dictIndexfile("../data/dictIndexCn.dat");
    if (!dictIndexfile.is_open()) {
        cerr << "无法打开文件" << "dictIndexCn.dat" << "\n";
    }

    stringstream ssIndex;
    for (const auto& pair : _index) {
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

void DictProducer::loadCnStopwords() {
    ifstream file("stop_words_zh.txt");
    string stop_word;
    if (file.is_open()) {
        while (file >> stop_word) {
            _stopwords.insert(stop_word);
        }
        file.close();
    } else {
        cerr << "无法使用停用词文件\n";
    }
}

void DictProducer::buildCnDict(const string& str) {
    getFiles(str);
    for (auto& file : _files) {
        ifstream ifs(file, std::ios::ate);  // 打开文件，并将ptr指向文件末尾
        if (!ifs.good()) {
            cerr << "openCnDict file fail\n";
            return;
        }
        size_t length = ifs.tellg();
        ifs.seekg(std::ios_base::beg);
        char* buff = new char[length + 1];
        ifs.read(buff, length + 1);
        string txt(buff);

        vector<string> tmp = SplitToolCppJieba::getInstance()->cut(txt);
        for (auto& i : tmp) {
            // 不是停用词且UTF-8字节数为3
            if (!_stopwords.count(i) && getByteNum_UTF8(i[0] == 3)) {
                pushDict(i);
            }
        }
        ifs.close();
    }
}

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

void DictProducer::createCnIndex() {
    int i = 0;  // 记录下标
    // vector<pair<string, int>> _dict;
    for (auto elem : _dict) {
        string word = elem.first;
        size_t charNums = word.size() / getByteNum_UTF8(word[0]);
        for (size_t idx = 0, n = 0; n != charNums; ++idx, ++n) {  //  得到字符数
            // 按照字符个数切割
            size_t charLen = getByteNum_UTF8(word[idx]);
            string subWord =
                word.substr(idx, charLen);  // 按照编码格式，进行拆分
            _index[subWord].insert(i);
            idx += (charLen - 1);
        }
        ++i;
    }
}

size_t DictProducer::getByteNum_UTF8(const char byte) {
    int byteNum = 0;
    for (size_t i = 0; i < 6; ++i) {
        if (byte & (1 << (7 - i)))
            ++byteNum;
        else
            break;
    }

    return byteNum == 0 ? 1 : byteNum;
}

void DictProducer::store() {}

int main() {
    DictProducer dict1("english.txt", 1);  // 1表示英文文件
    DictProducer dict2("art", 0);          // 0表示中文文件
}

vector<string> DictProducer::getFiles(const string& path) {
    DIR* pDir = opendir(path.c_str());
    if (!pDir) {
        perror("opendir");
        cout << path << "\n";
        return vector<string>{};
    }
    vector<string> ans;
    for (dirent* p = readdir(pDir); p != nullptr; p = readdir(pDir)) {
        if (p->d_type == DT_REG) {
            string pathname = path + "/" + p->d_name;
            ans.push_back(pathname);
        }
    }
    closedir(pDir);

    return ans;
}

void DictProducer::pushDict(const string& word) {
    auto it = std::find_if(
        _dict.begin(), _dict.end(),
        [&word](const pair<string, int>& pair) { return pair.first == word; });

    if (it != _dict.end()) {
        it->second++;
    } else {
        _dict.push_back(make_pair(word, 1));
    }
}