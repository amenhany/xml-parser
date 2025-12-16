#ifndef COMPRESS_HPP
#define COMPRESS_HPP


#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace xml_editor {
    class Compress{
public:
//function takes the input and output file names from the CLI/GUI interface
std::pair<std::vector<std::string>, std::unordered_map<std::string, std::string>> compress(const std::string& xml_string);
std::string decompress(const std::vector<std::string>& compressed_symbols,const std::unordered_map<std::string, std::string>& dictionary);


private:
std::pair<std::string, int> find_most_frequent_pair(const std::vector<std::string>& symbols);
void replace_pair(std::vector<std::string>& symbols, const std::string& pair, const std::string& token);
        
    };
    
}




#endif