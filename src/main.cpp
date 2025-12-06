
#include <iostream>
#include <xml_editor/io.hpp>

int main() {
    std::cout << xml_editor::io::file_read("input.xml") << std::endl;
    return 0;
}
