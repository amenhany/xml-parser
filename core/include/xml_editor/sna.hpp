#ifndef XML_EDITOR_SNA
#define XML_EDITOR_SNA

#include "graph.hpp"

namespace xml_editor::sna {

    const User* most_active(const Graph& xmlGraph);
    const User* most_influencer(const Graph& xmlGraph);
    std::vector<const User*> get_mutual(const Graph& xmlGraph, const std::vector<std::string>& ids);
    std::vector<const User*> get_suggestions(const Graph& xmlGraph, const std::string& id);
    std::vector<Post> search_by_word(const Graph& xmlGraph, const std::string& word);
    std::vector<Post> search_by_topic(const Graph& xmlGraph, const std::string& topic);

}

#endif
