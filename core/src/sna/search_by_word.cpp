#include "xml_editor/sna.hpp"

namespace xml_editor::sna {

    std::vector<const Post*> search_by_word(const Graph& graph, const std::string& word) {

        std::vector <const Post*> MatchingPosts;
        const auto& id_to_user = graph.get_id_to_user();

        for (const auto& [user_id, user] : id_to_user) {

            for (const Post& post : user.get_posts()) {

                if (post.get_text().find(word) != std::string::npos) {
                    MatchingPosts.push_back(&post);
                }
            }
        }
        return MatchingPosts;
    }
}
