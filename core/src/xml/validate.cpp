#include "xml_editor/xml.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

namespace xml_editor::xml {
static std::vector<XMLError> all_errors;
static std::vector<std::string> fixed_lines;

bool is_valid(const std::string &xmlText) {
  std::stringstream ss(xmlText);

  std::stack<std::string> tags;
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
      bool bracketMissing = false;
      if (endPos == std::string::npos) {
        all_errors.push_back({lineNumber, "Missing '>'"});
        newLine += ">";
        endPos = line.size();
        bracketMissing = true;
      }

      std::string tag = line.substr(
          pos + 1, bracketMissing ? std::string::npos
                                  : endPos - pos - 1); // get name inside tag

      if (!tag.size()) {
        all_errors.push_back({lineNumber, "Empty tag <> detected"});
        if (bracketMissing)
          break;
        pos = endPos + 1;
        continue;
      }

      if (tag[0] == '/') { // is a closing tag
        std::string closing = tag.substr(1);

        if (tags.empty()) { // nothing in stack,there is a closing tag
          all_errors.push_back({lineNumber, "Closing tag </" + closing +
                                                "> has no opening tag"});
          newLine.insert(pos,
                         "<" + closing + ">"); // add the missing opening tag
          pos += closing.size() + 2;
          tags.push(closing);
        }
        if (!tags.empty() &&
            tags.top() != closing) { // closing tag doesn't match top of stack

          bool found = false;
          std::stack<std::string> temp = tags;
          while (!temp.empty()) {
            if (temp.top() == closing) {
              found = true;
              break;
            }
            temp.pop();
          }

          if (found) {
            // Case 1: missing closing tag(s)
            while (!tags.empty() && tags.top() != closing) {
              all_errors.push_back(
                  {lineNumber, "Missing closing tag </" + tags.top() + ">"});
              fixed_lines.push_back(std::string((tags.size() - 1) * 4, ' ') +
                                    "</" + tags.top() + ">");
              tags.pop();
            }
            // do NOT log mismatched here
          } else {
            // Case 2: real mismatched tag (e.g. </id>)
            all_errors.push_back(
                {lineNumber, "Mismatched tag: expected </" + tags.top() +
                                 "> but found </" + closing + ">"});
            newLine.replace(pos + 2, closing.size(), tags.top());
            closing = tags.top();
          }
        }
        if (!tags.empty() && tags.top() == closing) { // correct closing tag
          tags.pop();
        }
      } else { // was an opening tag --> push in stack
        // Detect unclosed sibling
        if (!tags.empty() && tags.top() == tag) {
          all_errors.push_back({lineNumber, "Missing closing tag </" + tag +
                                                "> before sibling"});

          // insert closing tag BEFORE this opening tag
          newLine.insert(pos, "</" + tag + ">\n" +
                                  std::string((tags.size() - 1) * 4, ' '));

          tags.pop(); // close previous sibling

          // update position after insertion
          pos += tag.size() + 3;
        }

        tags.push(tag);
      }

      pos = endPos + 1;
    }

    // AUTO-CLOSE TAG IF LINE ENDS WITH TEXT
    if (!tags.empty()) {
      size_t lastOpen = newLine.rfind('>');
      size_t lastClose = newLine.rfind("</");

      // There is text after an opening tag and no closing tag
      if (lastOpen != std::string::npos &&
          (lastClose == std::string::npos || lastClose < lastOpen) &&
          lastOpen + 1 < newLine.size()) {
        std::string openTag = tags.top();
        newLine += "</" + openTag + ">";
        tags.pop();

        all_errors.push_back(
            {lineNumber, "Missing closing tag </" + openTag + ">"});
      }
    }

    fixed_lines.push_back(newLine);
  }

  int lastLine = fixed_lines.size();

  while (!tags.empty()) { // stack not empty
    all_errors.push_back(
        {lastLine, "Missing closing tag </" + tags.top() + ">"});
    fixed_lines.push_back("</" + tags.top() + ">");
    tags.pop();
  }

  return all_errors.empty();
}

bool has_errors() { return !all_errors.empty(); }

int get_error_count() { return all_errors.size(); }

const std::vector<XMLError> &get_errors() { return all_errors; }

const std::vector<std::string> &get_fixed_lines() { return fixed_lines; }

std::string get_fixed_XML() {
  std::string result = fixed_lines[0];
  for (int i = 1; i < fixed_lines.size(); i++) {
    result += '\n' + fixed_lines[i];
  }
  return result;
}

void print_errors() {
  for (auto &e : all_errors) {
    std::cout << "Line " << e.line << ": " << e.message << "\n";
  }
}

} // namespace xml_editor::xml
