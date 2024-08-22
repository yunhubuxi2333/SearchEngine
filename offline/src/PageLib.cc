#include "../include/PageLib.h"

#include <fstream>

#include "../include/Configuration.h"
#include "../include/RssReader.h"

using std::ofstream;

PageLib::PageLib(DirScanner& scanner) : _dirScanner(scanner) {}

void PageLib::create() {
    vector<string>& files = _dirScanner.getFiles();
    RssReader reader(files);
    reader.loadFiles();
    reader.createXML();
}

#if 0
void PageLib::store() {
    ofstream ofsPage(CONFIG(RIPEPAGE_PATH));
    ofstream ofsOffset(CONFIG(OFFSET_PATH));
    int i = 0;
    for (auto& page : _pages) {
        ofstream::pos_type offset = ofsPage.tellp();
        size_t length = page.size();
        ofsPage << page;
        ofsOffset << ++i << '\t' << offset << '\t' << length << '\n';
    }
    cout << ">> store ripepage and offset success\n";
    ofsPage.close();
    ofsOffset.close();
}
#endif