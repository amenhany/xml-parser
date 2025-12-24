#include "xml_editor/io.hpp"
#include "xml_editor/util.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace xml_editor::io {

    bool file_write(const std::string& fileName, const std::string& content, int tabWidth) {
        std::string output_content;

        if (tabWidth > 0) {
            output_content = util::tabsToSpaces(content, tabWidth);
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
