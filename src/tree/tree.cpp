#include "xml_editor/tree.hpp"

namespace xml_editor {
    TreeNode::TreeNode(const std::string& tag, const std::string& value) : tag(tag), value(value), children() {}

    TreeNode::~TreeNode() {
        for (auto child : children)
            delete child;
    }

    void TreeNode::add_child(TreeNode* child) {
        children.push_back(child);

        child->parent = this;
    }

    void TreeNode::set_value(const std::string& val) { this->value = val; }

    std::string TreeNode::get_value() const { return this->value; }
    std::string TreeNode::get_tag() const { return this->tag; }
    const std::vector<TreeNode*>& TreeNode::get_children() const { return children; }


    Tree::Tree(TreeNode* root) : root(root) {}

    Tree::~Tree() {
        if (root) delete root;
    }

    TreeNode* Tree::get_root() const { return this->root; }
}
