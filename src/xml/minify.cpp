#include "xml_editor/xml.hpp"
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
namespace xml_editor::xml {


string minifyXML(const string& xml) {
    string result;
    result.reserve(xml.size());

    bool insideTag = false;    
    bool insideText = false;   
    bool lastWasSpace = false; 

    for (size_t i = 0; i < xml.size(); i++) {
        char c = xml[i];

        if (c == '<') {
            insideTag = true;
            insideText = false;
            result.push_back(c);
            lastWasSpace = false;
            continue;
        }

        if (c == '>') {
            insideTag = false;
            insideText = true;
            result.push_back(c);
            lastWasSpace = false;
            continue;
        }

        if (insideTag) {
            if (!isspace(c)) {
                result.push_back(c);
            }
            continue;
        }

        if (insideText) {
            if (isspace(c)) {
                if (!lastWasSpace) {
                    result.push_back(' ');
                    lastWasSpace = true;
                }
            } else {
                result.push_back(c);
                lastWasSpace = false;
            }
            continue;
        }
    }

    if (!result.empty() && result[0] == ' ')
        result.erase(result.begin());

    if (!result.empty() && result.back() == ' ')
        result.pop_back();

    return result;
}

}
