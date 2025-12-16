#include <iostream>
#include <queue>
#include "xml_editor/cli.hpp"
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

int main(int argc, char** argv) {
    if (argc > 1) {
        cli::run_cli(argc, argv);
    }
}
