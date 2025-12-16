#include "xml_editor/xml.hpp"
#include "xml_editor/compress.hpp"
#include "xml_editor/io.hpp"
#include <fstream>
#include <iostream>

namespace xml_editor {

// ==============================
// Public Functions
// ==============================




std::pair<
    std::vector<std::string>,
    std::unordered_map<std::string, std::string>
>
Compress::compress(const std::string& xml_string) {
    int token_counter=0;
    // 1. Convert XML string into symbols (each char as string)
    std::vector<std::string> symbols;

   for (char c : xml_string) {
    symbols.push_back(std::string(1, c)); // each character becomes a string symbol
}

    // 2. Dictionary to store token -> original pair
    std::unordered_map<std::string, std::string> dictionary;

    // 3. Repeatedly apply BPE
    auto most_frequent_pair = find_most_frequent_pair(symbols);
    while(most_frequent_pair.second>1){
          std::string token = "@" + std::to_string(token_counter++);
          dictionary[token] = most_frequent_pair.first;
          replace_pair(symbols, most_frequent_pair.first, token);
          most_frequent_pair = find_most_frequent_pair(symbols);
    }
    return {symbols, dictionary};
}


std::string Compress::decompress(
    const std::vector<std::string>& compressed_symbols,
    const std::unordered_map<std::string, std::string>& dictionary
) {
    // TODO:
    // - reverse dictionary substitutions
    // - reconstruct original XML string

    std::string result;

    return result;
}

// ==============================
// Private Helper Functions
// ==============================

std::pair<std::string, int>
Compress::find_most_frequent_pair(
    const std::vector<std::string>& symbols
) {
     std::unordered_map<std::string, int> freq;
     for (size_t i = 0; i + 1 < symbols.size(); i++) {
        std::string pair = symbols[i] + symbols[i + 1];
        freq[pair]++;
    }
      std::string most_frequent;
      int max_count=0;
       for (const auto& kv : freq) {
        if (kv.second > max_count) {
            max_count = kv.second;
            most_frequent = kv.first;
        }

     return {most_frequent, max_count};
}
}


void Compress::replace_pair(
    std::vector<std::string>& symbols,
    const std::string& pair,
    const std::string& token
) {
    std::vector<std::string> new_symbols;
   size_t i = 0;
   std::string temp;
   while(i<symbols.size()){
    if(i + 1 < symbols.size() && (symbols[i] + symbols[i+1] == pair) ){
        new_symbols.push_back(token);
        i+=2;
    }
    else{
        new_symbols.push_back(symbols[i]);
        i++;
    }
   }
symbols = std::move(new_symbols);
    
}

} 

