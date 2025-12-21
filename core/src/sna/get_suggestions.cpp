#include "xml_editor/sna.hpp"

namespace xml_editor::sna {
    std::string get_suggestions(Graph xmlGraph, const std::string& id) {
        std::unordered_map<std::string, User> id_to_user = xmlGraph.get_id_to_user();
        User user = id_to_user.at(id);
        std::string result = "";
        std::vector<std::string> followers = user.get_followers();
        if (followers.empty())
            return "No Suggestions";
        std::vector<std::string> suggestions = id_to_user.at(followers.at(0)).get_followers();
        for (int i = 1; i < followers.size(); i++) {
            std::vector<std::string> user_followers = id_to_user.at(followers.at(i)).get_followers();
            for (std::string follower_id : user_followers) {
                bool suggested = true;
                for (std::string suggested_follower : suggestions) {
                    if (follower_id == suggested_follower || follower_id == id) {
                        suggested = false;
                        break;
                    }
                }
                if (suggested)
                    suggestions.push_back(follower_id);
            }
        }
        int i = 1;
        for (std::string suggest : suggestions) {
            User user = id_to_user.at(suggest);
            result = "User " + std::to_string(i) + ":\n" + "Name: " + user.get_name() + "\nID: " + user.get_id() + "\n\n";
            i++;
        }
        return result;
    }
}
