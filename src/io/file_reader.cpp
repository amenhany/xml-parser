#include "xml_editor/io.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace xml_editor::io {

    std::string file_read(const std::string& file_name) {
        std::ifstream file("resources/" + file_name);
        if (!file.is_open()) {
            std::cerr << "Could not open the file - '" << file_name << "'" << std::endl;
            return std::string();
        }

        if (file.peek() == std::ifstream::traits_type::eof())
        {
            std::cout << "File is empty." << std::endl;
            return std::string();
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

}


