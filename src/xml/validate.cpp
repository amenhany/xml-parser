#include "xml_editor/xml.hpp"

using namespace std;

namespace xml_editor::xml {
  static vector < XMLError > allErrors;
  static vector < string > fixedLines;

  bool is_valid(const string & filename) {
    ifstream file(filename); //opens file reads it line by line
    if (!file.is_open()) {
      allErrors.push_back({
        0,
        "Cannot open file"
      });
      return false;
    }

    stack < string > tags;
    string line;
    int lineNumber = 0;
    allErrors.clear();
    fixedLines.clear();

    while (getline(file, line)) {
      lineNumber++;
      string newLine=line;

      size_t pos = 0;

      while ((pos = line.find('<', pos)) != string::npos) {

        size_t endPos = line.find('>', pos);
        if (endPos == string::npos) {
          allErrors.push_back({
            lineNumber,
            "Missing '>'"
          });
          break;
        }

        string tag = line.substr(pos + 1, endPos - pos - 1); //get name inside tag

        if (!tag.size()) {
          allErrors.push_back({
            lineNumber,
            "Empty tag <> detected"
          });
          pos = endPos + 1;
          continue;
        }

        if (tag[0] == '/') { //is a closing tag
          string closing = tag.substr(1);

          if (tags.empty()) { //nothing in stack,there is a closing tag
            allErrors.push_back({
              lineNumber,
              "Closing tag </" + closing + "> has no opening tag"
            });
            newLine.insert(pos, "<" + closing + ">"); //add the missing opening tag
            pos += closing.size() + 2; 
            tags.push(closing); 
          } 
         if (!tags.empty() && tags.top() != closing) { //closing tag doesn't match top of stack
            allErrors.push_back({
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

        fixedLines.push_back(newLine);
    }

    int lastLine = fixedLines.size();

    while (!tags.empty()) { //stack not empty
      allErrors.push_back({
        lastLine,
        "Missing closing tag </" + tags.top() + "> added"
      });
      fixedLines.push_back("</" + tags.top() + ">");
      tags.pop();
    }

    return allErrors.empty();
  }

  bool hasErrors() {
    return !allErrors.empty();
  }

  int getErrorCount() {
    return allErrors.size();
  }

  const vector < XMLError > & getErrors() {
    return allErrors;
  }

  const vector < string > & getFixedXML() {
    return fixedLines;
  }

  void printErrors() {
    for (auto & e: allErrors) {
      cout << "Line " << e.line << ": " << e.message << "\n";
    }
  }

}