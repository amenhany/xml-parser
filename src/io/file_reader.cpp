#include "xml_editor/io.hpp"
#include <fstream>
#include <string>
#include <sstream>  
#include <iostream>

using namespace std;
namespace xml_editor::io {

    static std::string fileRead(const std::string &fileName){
    ifstream file(fileName);
    if (!file.is_open()) {
         cerr << "Could not open the file - '" << fileName << "'" << endl;
         return nullptr ;
    }

    if (file.peek() == ifstream::traits_type::eof())
    {
        cout << "File is empty.";
        return nullptr ;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();   
    return buffer.str();      
}


    }


