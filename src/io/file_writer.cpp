#include "xml_editor/io.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace xml_editor::io {

    static std::string tabsToSpaces(const std::string& content, int tabWidth) {
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


    bool file_write(const std::string& fileName, const std::string& content, int tabWidth) {
        std::string output_content;

        if (tabWidth > 0) {
            output_content = tabsToSpaces(content, tabWidth);
        }
        else {
            output_content = content;
        }

        std::ofstream file(fileName, std::ios::trunc);

        if (!file.is_open()) {
            std::cerr << "Could not open the file for writing: '" << fileName << "'" << std::endl;
            return false;
        }

        file << output_content;

        if (file.fail()) {
            std::cerr << "An error occurred while writing to the file: '" << fileName << "'" << std::endl;
            file.close();
            return false;
        }

        file.close();
        return true;
    }

}
