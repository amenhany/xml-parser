#include "xml_editor/sna.hpp"

namespace xml_editor::sna {

    std::vector<const Post*> search_by_topic(const Graph& graph, const std::string& topic) {
        std::vector<const Post*> MatchingPosts;

        const auto& id_to_user = graph.get_id_to_user();

        for (const auto& [user_id, user] : id_to_user) {

            for (const Post& post : user.get_posts()) {
                for (const std::string& post_topic : post.get_topics()) {
                    if (post_topic == topic) {
                        MatchingPosts.push_back(&post);
                        break;
                    }
                }
            }
        }
        return MatchingPosts;
    }

}
