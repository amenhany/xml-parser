#include "xml_editor/sna.hpp"

namespace xml_editor::sna {

    const User* most_influencer(const Graph& xmlGraph) {
        const auto& users = xmlGraph.get_vertices();
        if (users.empty())
            return nullptr;
        const User* max_user = users[0];
        for (int i = 1; i < users.size(); i++) {
            if (users[i]->get_followers().size() >
                max_user->get_followers().size()) {
                max_user = users[i];
            }
        }
        return max_user;
    }
}
