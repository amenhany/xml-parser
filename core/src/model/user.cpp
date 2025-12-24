#include "xml_editor/user.hpp"

namespace xml_editor {
    Post::Post(std::vector<std::string> topics, std::string text, std::string author_id) : topics(topics), text(text), author_id(author_id) {}
    Post::~Post() = default;

    const std::vector<std::string>& Post::get_topics() const { return topics; }
    const std::string& Post::get_text() const { return text; }
    const std::string& Post::get_author_id() const { return author_id; }

    User::User() = default;
    User::User(TreeNode* treeNode) {
        std::string tag;
        std::string value;
        for (TreeNode* child : treeNode->get_children()) {
            tag = child->get_tag();
            value = child->get_value();

            if (tag == "name")
                this->name = value;

            else if (tag == "id")
                this->id = value;

            else if (tag == "followers") {
                for (TreeNode* follower : child->get_children())
                    followers.push_back(follower->get_children()[0]->get_value());
            }

            else if (tag == "posts") {
                for (TreeNode* post : child->get_children()) {
                    std::vector<std::string> topics;
                    std::string text;
                    for (TreeNode* postChild : post->get_children()) {
                        tag = postChild->get_tag();

                        if (tag == "body")
                            text = postChild->get_value();

                        if (tag == "topics") {
                            for (TreeNode* topic : postChild->get_children())
                                topics.push_back(topic->get_value());
                        }
                    }
                    Post user_post(topics, text, id);
                    this->posts.push_back(user_post);
                }
            }
        }

        for (auto& post : this->posts)
            if (post.author_id.empty()) post.author_id = id;
    }
    User::~User() = default;

    std::string User::get_id() const { return id; }
    std::string User::get_name() const { return name; }
    const std::vector<std::string>& User::get_followers() const { return followers; }
    const std::vector<Post>& User::get_posts() const { return posts; }


    std::ostream& operator<<(std::ostream& COUT, const xml_editor::User& U) {
        COUT << "Name: " << U.get_name() << "\nID: " << U.get_id();
        return COUT;
    }

    std::ostream& operator<<(std::ostream& COUT, const xml_editor::Post& P) {
        COUT << P.get_text() << "\nAuthor ID: " << P.get_author_id() << "\nTopics: ";
        const auto& topics = P.get_topics();
        for (int i = 0; i < topics.size() - 1; i++) {
            COUT << topics[i] << ", ";
        }
        COUT << topics[topics.size() - 1];
        return COUT;
    }
}
