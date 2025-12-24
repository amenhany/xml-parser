#ifndef XML_EDITOR_USER_HPP
#define XML_EDITOR_USER_HPP

#include "tree.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace xml_editor {

    class Post {
    private:
        std::vector<std::string> topics;
        std::string text;
        std::string author_id;

    public:
        Post(std::vector<std::string> topics, std::string text, std::string author_id);
        ~Post();
        const std::vector<std::string>& get_topics() const;
        const std::string& get_text() const;
        const std::string& get_author_id() const;

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


    std::ostream& operator<<(std::ostream& COUT, const xml_editor::User& U);
    std::ostream& operator<<(std::ostream& COUT, const xml_editor::Post& P);
}

#endif
