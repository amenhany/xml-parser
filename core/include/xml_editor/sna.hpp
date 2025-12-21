#ifndef XML_EDITOR_SNA
#define XML_EDITOR_SNA

#include "graph.hpp"

namespace xml_editor::sna {

    std::string most_active(Graph xmlGraph);
    std::string most_influencer(Graph xmlGraph);
    std::string get_mutual(Graph xmlGraph, const std::vector<std::string>& ids);
    std::string get_suggestions(Graph xmlGraph, const std::string& id);
    std::string search_by_word(Graph xmlGraph, const std::string& word);
    std::string search_by_topic(Graph xmlGraph, const std::string& topic);

}

#endif
