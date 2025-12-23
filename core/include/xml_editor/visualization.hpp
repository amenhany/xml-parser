#ifndef XML_EDITOR_VISUALIZATION
#define XML_EDITOR_VISUALIZATION

#include <string>

namespace xml_editor::draw {
    void run_graphviz(const std::string& graph, const std::string& outputPath);
}

#endif
