#include "xml_editor/tree.hpp"

namespace xml_editor {
    TreeNode::TreeNode(const std::string& tag, const std::string& value) : tag(tag), value(value), children() {}

    TreeNode::~TreeNode() {
        for (auto child : children)
            delete child;
    }

    void TreeNode::add_child(TreeNode* child) {
        children.push_back(child);
    }


    Tree::Tree(TreeNode* root) : root(root) {}

    Tree::~Tree() {
        if (root) delete root;
    }
}
