#include <iostream>
#include "xml_editor/io.hpp"
#include "xml_editor/xml.hpp"

using namespace xml_editor;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void tree_traverse(TreeNode* node) {
    if (!node) return;

    std::queue<TreeNode*> q;
    q.push(node);

    while (!q.empty()) {
        int size = q.size();
        while (size--) {
            TreeNode* curr = q.front();
            q.pop();

            cout << curr->get_tag() << ' ';

            for (auto child : curr->get_children())
                if (child) q.push(child);
        }
        cout << endl;
    }
}

void tree_traverse2(TreeNode* node) {
    cout << node->get_tag() << endl;
    vector<TreeNode*> children = node->get_children();
    for (auto child : children) {
        tree_traverse2(child);
    }
}

int main() {
    string input = io::file_read("input.xml");
    bool is_valid = xml::is_valid("input.xml");
    vector<xml::XMLError> errors = xml::get_errors();

    Tree* tree = xml::parse(input);
    // tree_traverse(tree->get_root());
    string json = xml::to_json(tree);
    cout << json << endl;

    for (auto error : errors) {
        cout << error.line << ' ' << error.message << endl;
    }
    return 0;
}
