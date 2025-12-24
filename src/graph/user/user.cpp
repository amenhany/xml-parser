#include "xml_editor/user.hpp"

#include <string>
#include <vector>

namespace xml_editor {
    Post::Post(std::vector<std::string> topics, std::string text) : topics(topics), text(text) {}
    Post::~Post() = default;

    const std::vector<std::string>& Post::get_topics() const { return topics; }
    std::string Post::get_text() const { return text; }

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
                    Post user_post(topics, text);
                    this->posts.push_back(user_post);
                }
            }
        }
    }
    User::~User() = default;

    std::string User::get_id() const { return id; }
    std::string User::get_name() const { return name; }
    const std::vector<std::string>& User::get_followers() const { return followers; }
    const std::vector<Post>& User::get_posts() const { return posts; }
}
