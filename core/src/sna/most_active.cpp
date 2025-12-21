#include "xml_editor/sna.hpp"

namespace xml_editor::sna {
    std::string most_active(Graph xmlGraph) {
        std::unordered_map<std::string, std::vector<std::string>> graph = xmlGraph.get_graph();
        int max = 0;
        std::string max_id;
        std::string result = "";
        for (const auto& user : graph) {
            if (user.second.size() > max) {
                max = user.second.size();
                max_id = user.first;
            }
        }
        if (max == 0)
            return "No active users";
        std::unordered_map<std::string, User> id_to_user = xmlGraph.get_id_to_user();
        User user = id_to_user.at(max_id);
        result += "User:\nName: " + user.get_name() + "\nID: " + user.get_id();
        return result;
    }
}
