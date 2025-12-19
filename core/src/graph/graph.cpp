#include "xml_editor/graph.hpp"
#include "xml_editor/user.hpp"
#include "xml_editor/tree.hpp"
#include <map>
#include <string>
#include <vector>


namespace xml_editor {
    class Graph {
    private:
        std::unordered_map<std::string, std::vector<std::string>> graph;
        std::unordered_map<std::string, User> id_to_user;

    public:
        Graph::Graph(Tree* tree) {
            TreeNode* root = tree->get_root();
            for (TreeNode* user : root->get_children()) {
                User u(user);
                add_vertex(u);
            }
            for (const auto& v1 : id_to_user) {
                User user = v1.second;
                for (const std::string& followerID : user.get_followers()) {
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

        void Graph::add_edge(const std::string& v1, const std::string& v2) {
            graph[v1].push_back(v2);
        }

        std::vector<User> Graph::get_vertices() {
            std::vector<User> vertices;
            for (const auto& v : id_to_user) {
                vertices.push_back(v.second);
            }
            return vertices;
        }

        std::vector<User> Graph::get_edges(const std::string& vertex_id) {
            std::vector<User> result;

            if (graph.find(vertex_id) == graph.end())
                return {};
            for (const std::string& id : graph[vertex_id]) {
                result.push_back(id_to_user[id]);
            }
            return result;
        }
    };
}

