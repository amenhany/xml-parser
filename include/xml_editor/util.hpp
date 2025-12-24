#ifndef XML_EDITOR_UTIL
#define XML_EDITOR_UTIL

#include <string>
#include <vector>

namespace xml_editor::util {
    std::vector<std::string> split_string(const std::string& s, char delimiter);
    std::string tabsToSpaces(const std::string& content, int tabWidth);
}

#endif
