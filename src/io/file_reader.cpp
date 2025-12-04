#include "xml_editor/io.hpp"
#include <fstream>
#include <string>
#include <sstream>  
#include <iostream>

using namespace std;
namespace xml_editor::io {

    std::string fileRead(const std::string &fileName){
    ifstream file(fileName);
    if (!file.is_open()) {
         cerr << "Could not open the file - '" << fileName << "'" << endl;
         return std::string();
    }

    if (file.peek() == ifstream::traits_type::eof())
    {
        cout << "File is empty." << endl;
        return std::string();
    }

    std::stringstream buffer;
    buffer << file.rdbuf();   
    return buffer.str();      
}


    }


