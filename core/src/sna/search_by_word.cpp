#include "xml_editor/sna.hpp"
using namespace std;

namespace xml_editor::sna {

    vector<Post> search_by_word(const Graph& graph, const string& word) {

        vector <Post> MatchingPosts;
        auto id_to_user = graph.get_id_to_user();

        for (const auto& [user_id, user] : id_to_user) {

            for (const Post& post : user.get_posts()) {
                
                if (post.get_text().find(word) != string::npos) {
                    MatchingPosts.push_back(post);
                }
            }
        }
        return MatchingPosts;
    }
}
