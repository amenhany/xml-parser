#include "xml_editor/xml.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

namespace xml_editor::xml {
    static std::vector < XMLError > all_errors;
    static std::vector < std::string > fixed_lines;

    bool is_valid(const std::string& xmlText) {
        std::stringstream ss(xmlText);

        std::stack < std::string > tags;
        std::string line;
        int lineNumber = 0;
        all_errors.clear();
        fixed_lines.clear();

        while (getline(ss, line)) {
            lineNumber++;
            std::string newLine = line;

            size_t pos = 0;

            while ((pos = line.find('<', pos)) != std::string::npos) {

                size_t endPos = line.find('>', pos);
                if (endPos == std::string::npos) {
                    all_errors.push_back({
                      lineNumber,
                      "Missing '>'"
                        });
                    break;
                }

                std::string tag = line.substr(pos + 1, endPos - pos - 1); //get name inside tag

                if (!tag.size()) {
                    all_errors.push_back({
                      lineNumber,
                      "Empty tag <> detected"
                        });
                    pos = endPos + 1;
                    continue;
                }

                if (tag[0] == '/') { //is a closing tag
                    std::string closing = tag.substr(1);

                    if (tags.empty()) { //nothing in stack,there is a closing tag
                        all_errors.push_back({
                          lineNumber,
                          "Closing tag </" + closing + "> has no opening tag"
                            });
                        newLine.insert(pos, "<" + closing + ">"); //add the missing opening tag
                        pos += closing.size() + 2;
                        tags.push(closing);
                    }
                    if (!tags.empty() && tags.top() != closing) { //closing tag doesn't match top of stack
                        all_errors.push_back({
                          lineNumber,
                          "Mismatched tag: expected </" + tags.top() + "> but found </" + closing + ">"
                            });
                        newLine.replace(pos + 1, closing.size(), tags.top()); //added a closing tag for top of stack
                        closing = tags.top();
                    }
                    if (!tags.empty() && tags.top() == closing) { //correct closing tag
                        tags.pop();
                    }
                }
                else { //was an opening tag --> push in stack
                    tags.push(tag);
                }

                pos = endPos + 1;
            }

            fixed_lines.push_back(newLine);
        }

        int lastLine = fixed_lines.size();

        while (!tags.empty()) { //stack not empty
            all_errors.push_back({
              lastLine,
              "Missing closing tag </" + tags.top() + "> added"
                });
            fixed_lines.push_back("</" + tags.top() + ">");
            tags.pop();
        }

        return all_errors.empty();
    }

    bool has_errors() {
        return !all_errors.empty();
    }

    int get_error_count() {
        return all_errors.size();
    }

    const std::vector < XMLError >& get_errors() {
        return all_errors;
    }

    const std::vector < std::string >& get_fixed_XML() {
        return fixed_lines;
    }

    void print_errors() {
        for (auto& e : all_errors) {
            std::cout << "Line " << e.line << ": " << e.message << "\n";
        }
    }

}
