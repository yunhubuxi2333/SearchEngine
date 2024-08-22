#include "../include/SplitTool.h"

SplitToolCppJieba* SplitToolCppJieba::_pInstance = nullptr;

SplitToolCppJieba* SplitToolCppJieba::getInstance() {
    if (_pInstance == nullptr) {
        ::atexit(destroy);
        _pInstance = new SplitToolCppJieba;
    }
    return _pInstance;
}

void SplitToolCppJieba::destroy() {
    if (_pInstance != nullptr) {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

vector<string> SplitToolCppJieba::cut(const string& sentence) {
    auto map = _conf.GetconfigMap();

    const char* const DICT_PATH = map[_conf.mapKey.DICT_PATH].c_str();
    const char* const HMM_PATH = map[_conf.mapKey.HMM_PATH].c_str();
    const char* const USER_DICT_PATH = map[_conf.mapKey.USER_DICT_PATH].c_str();
    const char* const IDF_PATH = map[_conf.mapKey.IDF_PATH].c_str();
    const char* const STOP_WORD_PATH = map[_conf.mapKey.STOP_WORD_PATH].c_str();
    
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER DICT_PATH, IDF PATH,
                          STOP WORD PATH);
    vector<string> words;
    jieba.Cut(sentence, words);
    // cout <limonp:Join(words.begin(),words.end(),")<endl;
    return words;
}
