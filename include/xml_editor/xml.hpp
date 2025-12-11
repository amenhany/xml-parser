#ifndef XML_EDITOR_XML
#define XML_EDITOR_XML

#include <string>
#include "tree.hpp"

namespace xml_editor::xml {

    struct XMLError {
        int line;
        std::string message;
    };

    Tree* parse(const std::string& input);

    bool is_valid(const std::string& filename);
    bool has_errors();
    int get_error_count();
    const std::vector<XMLError>& get_errors();
    const std::vector<std::string>& get_fixed_XML();
    void print_errors();

    void format();
    std::string to_json(Tree* tree);
    void minify();
    void compress();
    void decompress();
}

#endif
