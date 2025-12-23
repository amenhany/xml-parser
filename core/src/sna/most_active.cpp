#include "xml_editor/sna.hpp"

namespace xml_editor::sna {

    const User* most_active(const Graph& xmlGraph) {
        std::unordered_map<std::string, std::vector<std::string>> graph = xmlGraph.get_graph();
        int max = 0;
        std::string max_id;
        for (const auto& user : graph) {
            if (user.second.size() > max) {
                max = user.second.size();
                max_id = user.first;
            }
        }
        if (max == 0)
            return nullptr;
        const auto& id_to_user = xmlGraph.get_id_to_user();
        return &id_to_user.at(max_id);
    }
}
