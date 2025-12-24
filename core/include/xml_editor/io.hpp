#ifndef XML_EDITOR_IO
#define XML_EDITOR_IO

#include <string>

namespace xml_editor::io {
    std::string file_read(const std::string& fileName);
    bool file_write(const std::string& fileName, const std::string& content, int tabWidth = 4);
}

#endif
