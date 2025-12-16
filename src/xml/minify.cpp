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

        for (size_t i = 0; i < xml.size(); i++) {
            char c = xml[i];

            if (c == '<') {
                insideTag = true;
                insideText = false;

                while (!result.empty() && isspace((result.back()))) {
                    result.pop_back();
                }

                result.push_back(c);
                continue;
            }

            if (c == '>') {
                insideTag = false;
                insideText = true;
                result.push_back(c);

                while (i + 1 < xml.size() && isspace((xml[i + 1]))) {
                    i++;
                }
                continue;
            }

            if (insideTag || insideText) {
                result.push_back(c);
            }
        }

        return result;
    }

}
