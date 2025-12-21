#include "xml_editor/sna.hpp"

#include <string>

namespace xml_editor::sna {

    std::string get_mutual(Graph xmlGraph, const std::vector<std::string>& ids) {
        std::unordered_map<std::string, User> id_to_user = xmlGraph.get_id_to_user();
        std::string result = "";
        User compare_user = id_to_user.at(ids.at(0));
        std::vector<std::string> compare_followers = compare_user.get_followers();
        std::vector<std::string> compared_followers;
        std::vector<std::string> mutual_followers;
        for (int i = 1; i < ids.size(); i++) {
            User compared_user = id_to_user.at(ids.at(i));
            compared_followers = compared_user.get_followers();
            for (std::string follower : compared_followers) {
                for (std::string compare_follower : compare_followers) {
                    if (follower == compare_follower)
                        mutual_followers.push_back(follower);
                }
            }
            compare_followers = mutual_followers;
            mutual_followers.clear();
        }
        int i = 1;
        for (std::string mutual_id : compare_followers) {
            User user = id_to_user.at(mutual_id);
            result = "User " + std::to_string(i) + ":\n" + "Name: " + user.get_name() + "\nID: " + user.get_id() + "\n\n";
            i++;
        }
        return result;
    }
}
