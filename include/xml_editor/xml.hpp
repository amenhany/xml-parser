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

    bool is_valid(const std::string& xmlText);
    bool has_errors();
    int get_error_count();
    const std::vector<XMLError>& get_errors();
    std::string get_fixed_XML();
    void print_errors();

    std::string format(const Tree* tree);
    std::string to_json(const Tree* tree);
    std::string minify(const std::string& xmlText);

    std::string compress(const std::string& xml_string);
    std::string decompress(const std::string& comp_string);
}

#endif
