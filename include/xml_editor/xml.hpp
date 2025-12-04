#ifndef XML_EDITOR_XML
#define XML_EDITOR_XML

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include "tree.hpp"

struct XMLError {
        int line;
        std::string message;
    };


namespace xml_editor::xml {

    Tree* parse(const std::string& input);
    bool is_valid(const std::string& filename);
    bool hasErrors();
    int getErrorCount();
    const std::vector<XMLError>& getErrors();
    const std::vector<std::string>& getFixedXML();
    void printErrors();
    void format();
    void to_json();
    void minify();
    void compress();
    void decompress();
}

#endif
