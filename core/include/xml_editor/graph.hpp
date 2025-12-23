#ifndef XML_EDITOR_GRAPH_HPP
#define XML_EDITOR_GRAPH_HPP

#include "tree.hpp"
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

        std::vector<const User*> get_vertices() const;
        std::vector<const User*> get_edges(const std::string& vertex_id) const;
        const std::unordered_map<std::string, std::vector<std::string>>& get_graph() const;
        const std::unordered_map<std::string, User>& get_id_to_user() const;
    };

}

#endif
