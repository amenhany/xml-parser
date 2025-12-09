#ifndef XML_EDITOR_TREE_HPP
#define XML_EDITOR_TREE_HPP

#include <string>
#include <vector>

namespace xml_editor {
    class TreeNode {
    public:
        std::string tag;
        std::string value;
        std::vector<TreeNode*> children;
        TreeNode* parent;

    public:
        TreeNode(const std::string& tag, const std::string& value);
        ~TreeNode();

        void add_child(TreeNode* child);
        void set_value(const std::string& val);
        std::string get_value();
        std::string get_tag();
        std::vector<TreeNode*> get_children();

        // Delete copy, assignment operators to avoid double deletion
        TreeNode(const TreeNode&) = delete;
        TreeNode& operator=(const TreeNode&) = delete;

        friend class Tree;
    };

    class Tree {
    private:
        TreeNode* root;

    public:
        Tree(TreeNode* root);
        ~Tree();

        // Delete copy, assignment operators to avoid double deletion
        Tree(const Tree&) = delete;
        Tree& operator=(const Tree&) = delete;

        TreeNode* get_root() const;
    };
}

#endif
