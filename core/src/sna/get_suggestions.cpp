#include "xml_editor/sna.hpp"
#include <unordered_set>

namespace xml_editor::sna {

    std::vector<const User*> get_suggestions(const Graph& xmlGraph, const std::string& id) {
        std::unordered_map<std::string, User> id_to_user = xmlGraph.get_id_to_user();
        std::vector<const User*> result;
        const User& user = id_to_user.at(id);
        std::vector<std::string> followers = user.get_followers();
        std::unordered_set<std::string> suggestions;

        for (const std::string& follower_id : followers) {
            for (const std::string& suggested_id : id_to_user.at(follower_id).get_followers()) {
                if (suggested_id != id)
                    suggestions.insert(suggested_id);
            }
        }
        for (const std::string& follower_id : followers)
            suggestions.erase(follower_id);
        for (const std::string& s_id : suggestions)
            result.push_back(&id_to_user.at(s_id));
        return result;
    }
}
