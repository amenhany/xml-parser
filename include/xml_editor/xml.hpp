#ifndef XML_EDITOR_XML
#define XML_EDITOR_XML

#include <string>
#include "tree.hpp"

namespace xml_editor::xml {
    Tree* parse(const std::string& input);
    bool is_valid();

    void format();
    void to_json();
    void minify();
    void compress();
    void decompress();
}

#endif
