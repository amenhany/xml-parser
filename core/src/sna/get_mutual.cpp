#include "xml_editor/sna.hpp"

#include <string>
#include <unordered_set>

namespace xml_editor::sna {

    std::string get_mutual(Graph xmlGraph, const std::vector<std::string>& ids) {

        std::unordered_map<std::string, User> id_to_user = xmlGraph.get_id_to_user();
        std::string result;
        std::unordered_set<std::string> mutual(
            id_to_user.at(ids[0]).get_followers().begin(),
            id_to_user.at(ids[0]).get_followers().end()
        );

        for (int i = 1; i < ids.size(); i++) {
            std::unordered_set<std::string> current(
                id_to_user.at(ids[i]).get_followers().begin(),
                id_to_user.at(ids[i]).get_followers().end()
            );
            for (auto it = mutual.begin(); it != mutual.end(); ) {
                if (!current.count(*it))
                    it = mutual.erase(it);
                else
                    ++it;
            }
        }
        int i = 1;
        for (std::string mutual_id : mutual) {
            User user = id_to_user.at(mutual_id);
            result += "User " + std::to_string(i) + ":\n" + "Name: " + user.get_name() + "\n" + "ID: " + user.get_id() + "\n\n";
            i++;
        }
        return result;
    }
}
