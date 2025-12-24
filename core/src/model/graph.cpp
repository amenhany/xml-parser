#include "xml_editor/graph.hpp"
#include "xml_editor/tree.hpp"

#include <unordered_map>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace xml_editor {
    Graph::Graph(Tree* tree) {
        TreeNode* root = tree->get_root();
        for (TreeNode* user : root->get_children()) {
            User u(user);
            add_vertex(u);
        }
        for (const auto& v1 : id_to_user) {
            const User& user = v1.second;
            for (const string& followerID : user.get_followers()) {
                if (id_to_user.find(followerID) != id_to_user.end())
                    add_edge(followerID, user.get_id());
            }
        }
    }
    Graph::~Graph() {}

    void Graph::add_vertex(const User& vertex) {
        graph[vertex.get_id()] = {};
        id_to_user[vertex.get_id()] = vertex;
    }

    void Graph::add_edge(const string& v1, const string& v2) {
        graph[v1].push_back(v2);
    }

    vector<const User*> Graph::get_vertices() const {
        vector<const User*> vertices;
        for (const auto& v : id_to_user) {
            vertices.push_back(&v.second);
        }
        return vertices;
    }

    vector<const User*> Graph::get_edges(const string& vertex_id) const {
        vector<const User*> result;

        if (graph.find(vertex_id) == graph.end())
            return {};
        for (const string& id : graph.at(vertex_id)) {
            result.push_back(&id_to_user.at(id));
        }
        return result;
    }

    const std::unordered_map<string, vector<string>>& Graph::get_graph() const { return graph; }
    const std::unordered_map<string, User>& Graph::get_id_to_user() const { return id_to_user; }

    std::string Graph::to_dot() const {
        std::string result = "digraph { ";
        std::vector<const User*> users = get_vertices();

        // nodes (for isolated users)
        for (const User* user : users) {
            result += "\"" + user->get_id() + "\"; ";
        }

        // edges
        for (const User* user : users) {
            for (const auto& follower : user->get_followers()) {
                result += "\"" + user->get_id() + "\" -> \"" + follower + "\"; ";
            }
        }

        result += "}";
        return result;
    }
}

