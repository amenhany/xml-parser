#ifndef XML_EDITOR_GRAPH_HPP
#define XML_EDITOR_GRAPH_HPP

#include "user.hpp"
#include <string>
#include <vector>
#include <unordered_map>


namespace xml_editor {
    class Graph {
    private:
        std::unordered_map<std::string, std::vector<std::string>> graph;
        std::unordered_map<std::string, User> id_to_user;

    public:
        Graph(Tree* tree);
        ~Graph();

        void add_vertex(const User& vertex);
        void add_edge(const std::string& v1, const std::string& v2);
        std::vector<User> get_vertices();
        std::vector<User> get_edges(const std::string& vertex_id);
    };

}

#endif
