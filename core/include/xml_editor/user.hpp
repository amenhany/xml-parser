#ifndef XML_EDITOR_USER_HPP
#define XML_EDITOR_USER_HPP

#include "tree.hpp"
#include <string>
#include <vector>

namespace xml_editor {
    class Post {
    private:
        std::vector<std::string> topics;
        std::string text;

    public:
        Post(std::vector<std::string> topics, std::string text);
        ~Post();
        const std::vector<std::string>& get_topics() const;
        std::string get_text() const;

        friend class User;
    };
    class User {
    private:
        std::string id;
        std::string name;
        std::vector<Post> posts;
        std::vector<std::string> followers; //followers IDs

    public:
        User(TreeNode* TreeNode);
        User();
        ~User();
        std::string get_id() const;
        std::string get_name() const;
        const std::vector<Post>& get_posts() const;
        const std::vector<std::string>& get_followers() const;
    };
}

#endif
