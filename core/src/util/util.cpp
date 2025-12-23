#include "xml_editor/util.hpp"

#include <sstream>
#include <cctype>

namespace xml_editor::util {
    std::vector<std::string> split_string(const std::string& s, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }

        return result;
    }

    std::string tabsToSpaces(const std::string& content, int tabWidth) {
        //space_replacement(no of rep, character to be repeated)
        const std::string space_replacement(tabWidth, ' ');
        std::string result;

        result.reserve(content.size() * 2);

        for (char c : content) {
            if (c == '\t') {
                result += space_replacement;
            }
            else {
                result += c;
            }
        }
        return result;
    }
}
