#include "xml_editor/xml.hpp"
#include <string>
#include <iostream>

using namespace std;

namespace xml_editor::xml {
    // divide tree into subtrees and convert to JSON
    string xml2json(Tree xmlTree)
    {
        string json = "";
        TreeNode* prev_node;
        TreeNode* root = xmlTree.get_root();
        
        if (!root) return "{}";

        // Root object
        json += "{\n";
        json += "\t\"" + root->tag + "\": {\n";

        // Traverse root's children
        for (size_t i = 0; i < root->children.size(); i++) {

            TreeNode* child = root->children[i];
            if (i == 0){
                prev_node =root;
            }else{
                prev_node =root->children[i - 1];
            }

            bool is_last = (i == root->children.size() - 1);
            
            json_traversal(child, prev_node, is_last, i, json, 2);
        }

        json += "\t}\n";
        json += "}";
        return json;
    }

    

    // Recursive traversal to build JSON
    void json_traversal(TreeNode* node, TreeNode* prev_node, bool last, int order, string& json, int level = 1)
    {
        // Skip comment-like tags
        if (!node->tag.empty() && node->tag.size() >= 2 && 
            node->tag[0] == '<' && node->tag[1] == '!') {
            return;
        }

        // Check if this node's tag matches previous node's tag
        bool is_same_tag = (node->tag == prev_node->tag);

        if (is_same_tag) {
            // This is a "twin" sibling - part of an array in JSON
            if (node->children.size() > 0) {
                // Node has children
                json += tab(level) + "{\n";
                for (int i = 0; i < node->children.size(); i++) {

                    TreeNode* child = node->children[i];

                    if (i==0){
                        prev_node=node;
                    }else{
                        prev_node=node->children[i-1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);
                    
                    json_traversal(child, prev_node, is_last_child, i, json,level + 1);
                }
                json += tab(level) + "}";
            } else {
                // Leaf node
                json += tab(level) + "\"" + node->value + "\"";
            }
            json += "\n";

            // Check if we need to close the array
            if (last) {
                json += tab(level - 1) + "]";
                json += "\n";
            }
            return;
        }

        //check if this node has sibling twins
        if (has_twins(node->parent, node->tag)) {
            // This node starts an array of siblings with the same tag
            json += tab(level) + "\"" + node->tag + "\": [\n";
            
            if (node->children.size() > 0) {
                json += tab(level + 1) + "{\n";

                for (int i = 0; i < node->children.size(); i++) {

                    TreeNode* child = node->children[i];

                    if (i==0){
                        prev_node=node;
                    }else{
                        prev_node=node->children[i-1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);
                    
                    json_traversal(child, prev_node, is_last_child, i, json,level + 2);
                }
                json += tab(level + 1) + "}";
            } else {
                json += tab(level + 1) + "\"" + node->value + "\"";
            }
            if (!last) json += ",";
            json += "\n";
            return;
        }

        // No twins
        if (node->children.size() > 0) {
            // Node has children
            json += tab(level) + "\"" + node->tag + "\": {\n";

                for (int i = 0; i < node->children.size(); i++) {

                    TreeNode* child = node->children[i];

                    if (i==0){
                        prev_node=node;
                    }else{
                        prev_node=node->children[i-1];
                    }
                    bool is_last_child = (i == node->children.size() - 1);
                    
                    json_traversal(child, prev_node, is_last_child, i, json,level + 1);
                }

            json += tab(level) + "}";
        } else {
            // Leaf node
            json += tab(level) + "\"" + node->tag + "\": \"" + node->value + "\"";
        }
        if (!last) json += ",";
        json += "\n";
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

    // generate tabs
    static string tab(int level) {
        string result;
        for (int i = 0; i < level; i++) {
            result += "\t";
        }
        return result;
    }

    
    
}
