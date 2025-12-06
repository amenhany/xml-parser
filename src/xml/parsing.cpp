#include "xml_editor/xml.hpp"
#include <stack>
#include <cctype>

namespace xml_editor::xml {

    void skip_whitespaces(size_t& pos, const std::string& input) {
        while (pos < input.size() && isspace(input[pos]))
            pos++;
    };

    std::string trim_string(const std::string& body) {
        size_t first_ch = 0;
        while (first_ch < body.size() && isspace(body[first_ch])) {
            first_ch++; // remove spaces until first char
        }

        size_t last_ch = body.size();

        while (last_ch > first_ch && isspace(body[last_ch - 1])) {
            last_ch--;  // remove spaces until last char
        }

        if (last_ch > first_ch) {
            std::string cleaned = body.substr(first_ch, last_ch - first_ch);
            return cleaned;
        }

        return body;
    }

    Tree* parse(const std::string& input) {

        if (input.empty()) {
            return nullptr;
        }

        size_t pos = 0;
        std::stack<TreeNode*> nodes;
        TreeNode* root = nullptr;
        skip_whitespaces(pos, input);

        while (pos < input.size()) {
            size_t start;
            if (input[pos] == '<') {

                // closing tag
                if (pos + 1 < input.size() && input[pos + 1] == '/') {
                    pos += 2;
                    start = pos;

                    while (input[pos] != '>') {
                        pos++;
                    }

                    nodes.pop(); // tag name is complete
                    pos++;
                    skip_whitespaces(pos, input);
                }
                else { // open tag
                    pos++;
                    start = pos; // start of tag name
                }

                while (pos < input.size() && input[pos] != '>' && input[pos] != '/') {
                    pos++;
                }

                std::string tagName = input.substr(start, pos - start); // extract tag name
                TreeNode* newNode = new TreeNode(tagName, "");

                if (!nodes.empty()) {
                    nodes.top()->add_child(newNode); // adding child to parent node
                }
                else {
                    root = newNode; // If stack is empty, this is the root node (lowest level in stack)
                }
                nodes.push(newNode);


                while (pos < input.size() && input[pos] != '>' && input[pos] != '/') {
                    pos++;
                }
            }

            // Body text
            else {
                size_t start = pos; //Start of body text
                while (pos < input.size() && input[pos] != '<') {
                    pos++;
                }

                std::string body = input.substr(start, pos - start);

                if (!nodes.empty() && !body.empty()) {
                    std::string cleaned = trim_string(body);
                    nodes.top()->set_value(cleaned);
                }

                skip_whitespaces(pos, input);
            }
        }
        return new Tree(root);  //Return the constructed tree
    }
}
