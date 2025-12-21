#ifndef XML_EDITOR_SNA
#define XML_EDITOR_SNA

#include "graph.hpp"

namespace xml_editor::sna {

    std::string most_active(Graph graph);
    std::string most_influencer(Graph graph);
    std::string get_mutual(const std::vector<std::string>& ids);
    std::string get_suggestions(const std::string& id);
    std::string search_by_word(const std::string& word);
    std::string search_by_topic(const std::string& topic);

}

#endif
