#include "xml_editor/xml.hpp"
#include <string>

using std::string;

namespace xml_editor::xml {

    static string tab(int level) {
        return string(level, '\t');
    }

    static string format_value(const string& value) {
        string result = "";
        for (char c : value) {
            if (c == '\n') {
                result += "\\n";
            }
            else {
                result += c;
            }
        }
        return result;
    }

    //check if a parent has multiple children with the same tag
    static bool has_twins(TreeNode* parent, const string& tag) {
        if (!parent || parent->children.empty())
            return false;
        int count = 0;

        for (auto child : parent->children) {
            if (child->tag == tag) {
                count++;
                if (count > 1) return true;
            }
        }
        return false;
    }

    // Check if this is the last twin
    static bool is_last_twin(TreeNode* parent, TreeNode* node) {
        if (!parent || !node) return true;

        bool found = false;
        for (auto child : parent->children) {
            if (child == node) {
                found = true;
            }
            else if (found && child->tag == node->tag) {
                return false;
            }
        }
        return true;
    }

    // Check if all children have the same tag
    static bool parent_has_twin_children(TreeNode* node) {
        if (!node || node->children.empty()) return false;
        if (node->children.size() == 1) return false;

        string first_tag = node->children[0]->tag;
        for (auto child : node->children) {
            if (child->tag != first_tag) return false;
        }
        return true;
    }

    // Recursive traversal to build JSON
    void json_traversal(TreeNode* node, TreeNode* prev_node, bool last, int order, string& json, int level) {

        // Check if all siblings have same tag
        bool in_twin_array = parent_has_twin_children(node->parent);

        // Check if this node's tag matches previous node's tag (and prev is sibling, not parent)
        bool is_same_tag = (prev_node && prev_node->parent == node->parent && node->tag == prev_node->tag);

        if (is_same_tag && !in_twin_array) {
            // This is a "twin" sibling - part of an array in JSON
            if (node->children.size() > 0) {
                // Node has children
                json += ",\n";
                json += tab(level) + "{\n";
                for (int i = 0; i < node->children.size(); i++) {

                    TreeNode* child = node->children[i];

                    if (i == 0) {
                        prev_node = node;
                    }
                    else {
                        prev_node = node->children[i - 1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);

                    json_traversal(child, prev_node, is_last_child, i, json, level + 1);
                }
                json += tab(level) + "}";
            }
            else {
                // Leaf node
                json += ",\n";
                json += tab(level) + "\"" + format_value(node->value) + "\"";
            }

            // Check if we need to close the array
            if (is_last_twin(node->parent, node)) {
                json += "\n";
                json += tab(level - 1) + "]";
                if (!last) json += ",";
                json += "\n";
            }
            return;
        }

        //check if this node has sibling twins
        if (has_twins(node->parent, node->tag) && !in_twin_array) {
            // This node starts an array of siblings with the same tag
            json += tab(level - 1) + "\"" + node->tag + "\": [\n";

            if (node->children.size() > 0) {
                json += tab(level) + "{\n";

                for (int i = 0; i < node->children.size(); i++) {

                    TreeNode* child = node->children[i];

                    if (i == 0) {
                        prev_node = node;
                    }
                    else {
                        prev_node = node->children[i - 1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);

                    json_traversal(child, prev_node, is_last_child, i, json, level + 1);
                }
                json += tab(level) + "}";
            }
            else {
                json += tab(level) + "\"" + format_value(node->value) + "\"";
            }
            return;
        }

        // No twins
        if (node->children.size() > 0) {
            // Check if this node has twin children
            bool has_twin_children = parent_has_twin_children(node);

            if (has_twin_children) {
                // This node's children are twins
                if (!in_twin_array) {
                    json += tab(level) + "\"" + node->tag + "\": [\n";
                }
                else {
                    json += tab(level) + "[\n";
                }

                for (int i = 0; i < node->children.size(); i++) {
                    TreeNode* child = node->children[i];

                    if (i == 0) {
                        prev_node = node;
                    }
                    else {
                        prev_node = node->children[i - 1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);

                    json_traversal(child, prev_node, is_last_child, i, json, level + 1);
                }

                json += tab(level) + "]";
            }
            else {
                // Normal node with children
                if (!in_twin_array) {
                    json += tab(level) + "\"" + node->tag + "\": {\n";
                }
                else {
                    json += tab(level) + "{\n";
                }

                for (int i = 0; i < node->children.size(); i++) {

                    TreeNode* child = node->children[i];

                    if (i == 0) {
                        prev_node = node;
                    }
                    else {
                        prev_node = node->children[i - 1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);

                    json_traversal(child, prev_node, is_last_child, i, json, level + 1);
                }

                json += tab(level) + "}";
            }
        }
        else {
            // Leaf node
            if (!in_twin_array) {
                json += tab(level) + "\"" + node->tag + "\": \"" + format_value(node->value) + "\"";
            }
            else {
                json += tab(level) + "\"" + format_value(node->value) + "\"";
            }
        }
        if (!last) json += ",";
        json += "\n";
    }

    // divide tree into subtrees and convert to JSON
    string to_json(const Tree* xmlTree) {
        string json = "";
        TreeNode* prev_node;
        TreeNode* root = xmlTree->get_root();

        if (!root) return "{}";

        // Root object
        json += "{\n";
        json += "\t\"" + root->tag + "\": ";
        if (parent_has_twin_children(root)) {
            json += "[\n";
        }
        else {
            json += "{\n";
        }

        // Traverse root's children
        for (int i = 0; i < root->children.size(); i++) {

            TreeNode* child = root->children[i];
            if (i == 0) {
                prev_node = root;
            }
            else {
                prev_node = root->children[i - 1];
            }

            bool is_last = (i == root->children.size() - 1);

            json_traversal(child, prev_node, is_last, i, json, 2);
        }

        json += "\t";
        if (parent_has_twin_children(root)) {
            json += "]\n";
        }
        else {
            json += "}\n";
        }
        json += "}\n";
        return json;
    }
}
