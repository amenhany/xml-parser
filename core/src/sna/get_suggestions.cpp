#include "xml_editor/sna.hpp"
#include <unordered_set>

namespace xml_editor::sna {

    std::string get_suggestions(const Graph& xmlGraph, const std::string& id) {
        const auto& id_to_user = xmlGraph.get_id_to_user();
        const User& user = id_to_user.at(id);
        const std::vector<std::string>& followers = user.get_followers();
        std::string result;

        if (followers.empty())
            return "No Suggestions";

        std::unordered_set<std::string> suggestions;

        for (const std::string& follower_id : followers) {
            for (const std::string& suggest : id_to_user.at(follower_id).get_followers()) {
                if (suggest != id)
                    suggestions.insert(suggest);
            }
        }

        for (const std::string& follower_id : followers)
            suggestions.erase(follower_id);

        int i = 1;
        for (const std::string& s_id : suggestions) {
            const User& user = id_to_user.at(s_id);
            result += "User " + std::to_string(i) + ":\n" + "Name: " + user.get_name() + "\nID: " + user.get_id() + "\n\n";
            i++;
        }
        if (result.empty())
            return "No Suggestions";
        return result;
    }
}
