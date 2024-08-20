#include "Configuration.h"

#include <iostream>
#include <sstream>
using std::cout;

using std::istringstream;

Configuration* Configuration::getInstance() {
    if (_pInstance) {
        return _pInstance;
    } else {
        return nullptr;
    }
}

Configuration* Configuration::getInstance(const string& filepath) {
    if (_pInstance == nullptr) {
        ::atexit(destroy);
        _pInstance = new Configuration(filepath);
    }
    return _pInstance;
}

void Configuration::destroy() {
    if (_pInstance) delete _pInstance;
}

Configuration* Configuration(const string& filepath) : _filepath(filepath) {
    readFile(filepath);
    cout << "Configuration(const string &)\n";
}
