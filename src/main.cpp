
#include <iostream>
#include "xml_editor/io.hpp"
int main() {
    std::cout <<xml_editor::io::fileRead("example.xml")<< std::endl;
    return 0;
}
