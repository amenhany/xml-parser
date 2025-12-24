#include "xml_editor/sna.hpp"

namespace xml_editor::sna {

    const User* most_active(const Graph& xmlGraph) {
        const auto& graph = xmlGraph.get_graph();
        const auto& id_to_user = xmlGraph.get_id_to_user();
        int max = 0;
        std::string max_id;

        for (const auto& user : graph) {
            if (user.second.size() + (id_to_user.at(user.first)).get_followers().size() > max) {
                max = user.second.size() + (id_to_user.at(user.first)).get_followers().size();
                max_id = user.first;
            }
        }
        if (max == 0)
            return nullptr;
        return &id_to_user.at(max_id);
    }
}
