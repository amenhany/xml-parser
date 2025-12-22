#include "xml_editor/sna.hpp"

namespace xml_editor::sna {
    std::string most_influencer(const Graph& xmlGraph) {
        const auto& users = xmlGraph.get_vertices();
        std::string result;
        const User* max_user = &users.at(0);
        for (int i = 1; i < users.size(); i++) {
            const User& user = users.at(i);
            if (user.get_followers().size() > max_user->get_followers().size())
                max_user = &user;
        }
        if (max_user->get_followers().empty())
            return "No followers";
        result += "User:\nName: " + max_user->get_name() + "\nID: " + max_user->get_id();
        return result;
    }
}
