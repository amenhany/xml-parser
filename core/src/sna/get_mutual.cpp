#include "xml_editor/sna.hpp"

#include <string>
#include <unordered_set>

namespace xml_editor::sna {

    std::vector<const User*> get_mutual(const Graph& xmlGraph, const std::vector<std::string>& ids) {
        std::unordered_map<std::string, User> id_to_user = xmlGraph.get_id_to_user();
        std::vector<const User*> mutual_users;

        std::unordered_set<std::string> mutual(
            id_to_user.at(ids[0]).get_followers().begin(),
            id_to_user.at(ids[0]).get_followers().end());

        for (size_t i = 1; i < ids.size(); i++) {
            std::unordered_set<std::string> current(
                id_to_user.at(ids[i]).get_followers().begin(),
                id_to_user.at(ids[i]).get_followers().end());

            for (auto it = mutual.begin(); it != mutual.end(); ) {
                if (!current.count(*it))
                    it = mutual.erase(it);
                else
                    ++it;
            }
        }
        for (const auto& id : mutual) {
            mutual_users.push_back(&id_to_user.at(id));
        }
        return mutual_users;
    }
}
