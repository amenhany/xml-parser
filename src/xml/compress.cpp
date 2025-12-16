#include "xml_editor/xml.hpp"

#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::string;
using std::unordered_map;

using SymbolList = vector<string>;
using Dictionary = unordered_map<string, string>;
using CompressedData = pair<SymbolList, Dictionary>;

namespace {

    pair<string, int> find_most_frequent_pair(const vector<string>& symbols) {

        unordered_map<string, int> freq;
        for (size_t i = 0; i + 1 < symbols.size(); i++) {
            string pair = symbols[i] + symbols[i + 1];
            freq[pair]++;
        }

        string most_frequent;
        int max_count = 0;
        for (const auto& kv : freq) {
            if (kv.second > max_count) {
                max_count = kv.second;
                most_frequent = kv.first;
            }
        }

        return { most_frequent, max_count };
    }

    void replace_pair(
        vector<string>& symbols,
        const string& pair,
        const string& token
    ) {
        vector<string> new_symbols;
        size_t i = 0;
        string temp;
        while (i < symbols.size()) {
            if (i + 1 < symbols.size() && (symbols[i] + symbols[i + 1] == pair)) {
                new_symbols.push_back(token);
                i += 2;
            }
            else {
                new_symbols.push_back(symbols[i]);
                i++;
            }
        }
        symbols = std::move(new_symbols);
    }


    // BPE Algorithm

    CompressedData bpe(const string& xml_string) {
        int token_counter = 0;
        // 1. Convert XML string into symbols (each char as string)
        vector<string> symbols;

        for (char c : xml_string) {
            symbols.push_back(string(1, c)); // each character becomes a string symbol
        }

        // 2. Dictionary to store token -> original pair
        unordered_map<string, string> dictionary;

        // 3. Repeatedly apply BPE
        auto most_frequent_pair = find_most_frequent_pair(symbols);
        while (most_frequent_pair.second > 1) {
            string token = "@" + std::to_string(token_counter++) + "#";
            dictionary[token] = most_frequent_pair.first;
            replace_pair(symbols, most_frequent_pair.first, token);
            most_frequent_pair = find_most_frequent_pair(symbols);
        }
        return { symbols, dictionary };
    }

    // Decompression algorithm

    CompressedData to_map(const string& comp_string) {
        SymbolList symbols;
        Dictionary dictionary;

        std::istringstream input(comp_string);
        string line;

        // 1. Read and validate version
        if (!std::getline(input, line)) {
            throw std::runtime_error("Empty compressed input");
        }

        if (line != "XML_EDITOR_COMP_V1") {
            throw std::runtime_error("Unsupported compression format");
        }

        // 2. Read until DICT
        while (std::getline(input, line)) {
            if (line == "DICT") break;
        }

        // 3. Read dictionary
        while (std::getline(input, line)) {
            if (line == "ENDDICT") break;

            size_t eq_pos = line.find('=');
            if (eq_pos == string::npos) continue;

            string token = line.substr(0, eq_pos);
            string value = line.substr(eq_pos + 1);
            dictionary[token] = value;
        }

        // 4. Read until DATA
        while (std::getline(input, line)) {
            if (line == "DATA") break;
        }

        // 5. Read compressed data line
        if (!std::getline(input, line)) {
            throw std::runtime_error("Missing DATA section");
        }

        string data_line = line;

        // 6. Parse DATA into symbols
        for (size_t i = 0; i < data_line.size();) {
            if (data_line[i] == '@') {
                size_t j = i + 1;
                while (j < data_line.size() && data_line[j] != '#') j++;
                j++; // include the #
                symbols.push_back(data_line.substr(i, j - i));
                i = j;
            }
            else {
                symbols.push_back(string(1, data_line[i]));
                i++;
            }

        }

        return { symbols, dictionary };
    }

    string decompression_algorithm(
        const SymbolList& compressed_symbols,
        const Dictionary& dictionary
    ) {
        string result = "";
        for (const auto& symbol : compressed_symbols) {
            string expanded = symbol;

            bool replaced;
            do {
                replaced = false;
                size_t pos = 0;

                while (pos < expanded.size()) {
                    if (expanded[pos] == '@') {
                        size_t end = expanded.find('#', pos);
                        if (end == string::npos) break; // malformed
                        string token = expanded.substr(pos, end - pos + 1);

                        if (dictionary.count(token)) {
                            expanded.replace(pos, end - pos + 1, dictionary.at(token));
                            replaced = true;
                            // continue scanning from same pos, because new text may contain tokens
                        }
                        else {
                            pos = end + 1;
                        }
                    }
                    else {
                        pos++;
                    }
                }
            } while (replaced);
            result += expanded;
        }

        return result;
    }
}

namespace xml_editor::xml {

    string compress(const string& xml_string) {
        CompressedData data = bpe(xml_string);
        // convert data to string
        string compressed_string = "XML_EDITOR_COMP_V1\nDICT\n";
        for (const auto& entry : data.second) {
            compressed_string += entry.first + "=" + entry.second + "\n";
        }
        compressed_string += "ENDDICT\nDATA\n";
        for (const auto& symbol : data.first) {
            compressed_string += symbol;
        }
        compressed_string += "\nENDDATA\n";



        return compressed_string;

    }

    string decompress(const string& comp_string) {
        CompressedData data = to_map(comp_string);
        return decompression_algorithm(data.first, data.second);
    }
}
