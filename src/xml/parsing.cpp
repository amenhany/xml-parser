#include "xml_editor/xml.hpp"
#include <stack>
#include <cctype>
#include <iostream>
#include <fstream>

namespace xml_editor::xml {

    void skip_whitespaces(size_t& pos, const std::string& input) {
        while (pos < input.size() && isspace(static_cast<unsigned char>(input[pos])))
            pos++;
    };

    std::string trim_string(const std::string& body) {
        size_t first_ch = 0;
        while (first_ch < body.size() && isspace(static_cast<unsigned char>(body[first_ch]))) {
            first_ch++; // remove spaces until first char
        }

        size_t last_ch = body.size();

        while (last_ch > first_ch && isspace(static_cast<unsigned char>(body[last_ch - 1]))) {
            last_ch--;  // remove spaces until last char
        }

        if (last_ch > first_ch) {
            return body.substr(first_ch, last_ch - first_ch);
        }

        return "";
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
                pos++; 

                if (input[pos] == '/') {
                    pos++; // Skip '/'
                    start = pos;

                    // Extract closing tag name
                    while (pos < input.size() && input[pos] != '>') {
                        pos++;
                    }

                    std::string closingTag = input.substr(start, pos - start);
                    closingTag = trim_string(closingTag);

                    if (!nodes.empty()) {
                        if (nodes.top()->get_tag() != closingTag) {
                            std::cerr << "Error: Mismatched tags"
                                << nodes.top()->get_tag()
                                << "> but found </" << closingTag << ">" << std::endl;
                        }
                        nodes.pop();
                    }

                    pos++;
                    skip_whitespaces(pos, input);
                }
                else {
                    start = pos;

                    while (pos < input.size() && input[pos] != '>' && !(input[pos] == '/' && pos + 1 < input.size() && input[pos + 1] == '>')) {
                        pos++;
                    }

                    std::string tagContent = input.substr(start, pos - start);
                    tagContent = trim_string(tagContent);

                    bool isSelfClosing = false;
                    if (pos < input.size() && input[pos] == '/' && pos + 1 < input.size() && input[pos + 1] == '>') {
                        isSelfClosing = true;
                        pos++;
                    }

                    if (pos < input.size() && input[pos] == '>') {
                        pos++;
                    }

                    
                    TreeNode* newNode = new TreeNode(tagContent, "");

                    if (!nodes.empty()) {
                        nodes.top()->add_child(newNode);
                        if (!isSelfClosing) {
                            nodes.push(newNode);
                        }
                    }
                    else {
                        root = newNode;
                        if (!isSelfClosing) {
                            nodes.push(newNode);
                        }
                    }

                    skip_whitespaces(pos, input);
                }
            }

            else {

                // Extract text content
                start = pos;

                while (pos < input.size() && input[pos] != '<') {
                    pos++;
                }

                std::string textContent = input.substr(start, pos - start);
                std::string trimmedContent = trim_string(textContent);

                if (!trimmedContent.empty()) {
                    if (!nodes.empty()) {
                        nodes.top()->set_value(trimmedContent);
                    }
                }

                skip_whitespaces(pos, input);
            }
        }

        if (!nodes.empty()) {
            std::cerr << "Error: Unclosed tags found" << std::endl;
            while (!nodes.empty()) {
                std::cerr << "  <" << nodes.top()->get_tag() << ">" << std::endl;
                nodes.pop();
            }

            if (root) {
                delete root;
                root = nullptr;
            }
        }

        return root ? new Tree(root) : nullptr;
    }
}
