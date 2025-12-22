#include "xml_editor/cli.hpp"
#include "xml_editor/xml.hpp"
#include "xml_editor/io.hpp"
#include "xml_editor/sna.hpp"
#include "xml_editor/graph.hpp"

#include <iostream>
#include <optional>
#include <sstream>

namespace {

    using xml_editor::cli::Command;

    int find_flag(const char* flag, int argc, char** argv) {
        for (int i = 1; i + 1 < argc; ++i) {
            if (std::strcmp(argv[i], flag) == 0) return i;
        }
        return -1;
    }

    std::string get_filepath(int argc, char** argv) {
        int idx = find_flag("-i", argc, argv);
        if (idx == -1 || idx + 1 >= argc) return "";
        return argv[idx + 1];
    }

    std::string get_output_path(int argc, char** argv) {
        int idx = find_flag("-o", argc, argv);
        if (idx == -1 || idx + 1 >= argc) return "";
        return argv[idx + 1];
    }

    int get_tab_width(int argc, char** argv) {
        int idx = find_flag("-t", argc, argv);
        if (idx == -1 || idx + 1 >= argc) return 4;
        return std::max(0, std::stoi(argv[idx + 1]));
    }

    Command parse_command(const std::string& cmd) {
        if (cmd == "verify") return Command::Verify;
        if (cmd == "format") return Command::Format;
        if (cmd == "mini") return Command::Minify;
        if (cmd == "json") return Command::Json;
        if (cmd == "compress") return Command::Compress;
        if (cmd == "decompress") return Command::Decompress;

        if (cmd == "most_influencer") return Command::MostInfluencer;
        if (cmd == "most_active") return Command::MostActive;
        if (cmd == "mutual") return Command::Mutual;
        if (cmd == "suggest") return Command::Suggest;
        if (cmd == "search") return Command::Search;
        return Command::Unknown;
    }

    bool needs_graph(Command command) {
        return (command == Command::MostInfluencer ||
            command == Command::MostActive ||
            command == Command::Mutual ||
            command == Command::Suggest ||
            command == Command::Search);
    }

    std::vector<std::string> split_string(const std::string& s, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }

        return result;
    }
}


namespace xml_editor::cli {

    void run_cli(int argc, char** argv) {
        std::string filePath = get_filepath(argc, argv);
        std::string outputPath = get_output_path(argc, argv);

        Command command = parse_command(argv[1]);

        if (filePath.empty()) {
            std::cout << "Input file not specified\n";
            return;
        }

        std::string inputText = io::file_read(filePath);
        if (inputText.empty()) return;

        if (command == Command::Decompress) {
            std::string outputText = xml::decompress(inputText);
            io::file_write(outputPath, outputText);
            return;
        }

        bool isValid = xml::is_valid(inputText);

        if (command == Command::Verify || !isValid) {
            int f = find_flag("-f", argc, argv);

            if (isValid) {
                std::cout << "Valid File\n";
                return;
            }

            std::cout << xml::get_error_count() << " Errors Found\nError Log\n";
            xml::print_errors();

            if (f == -1) return;
            if (outputPath.empty()) {
                std::cout << "Output file not specified\n";
                return;
            }

            std::string fixedFile = xml::get_fixed_XML();
            io::file_write(outputPath, fixedFile);
            std::cout << "File corrected at " << outputPath << '\n';
            return;
        }

        Tree* xmlTree = xml::parse(inputText);

        if (needs_graph(command)) {
            Graph graph(xmlTree);

            switch (command) {
            case Command::MostInfluencer:
                std::cout << sna::most_influencer(graph) << '\n';
                break;
            case Command::MostActive:
                std::cout << sna::most_active(graph) << '\n';
                break;
            case Command::Mutual: {
                int idx = find_flag("-ids", argc, argv);
                std::string ids;

                if (idx <= -1) {
                    std::cout << "No IDs given\n";
                    return;
                }

                ids = argv[idx + 1];
                std::vector<std::string> separatedIds = split_string(ids, ',');
                std::cout << sna::get_mutual(graph, separatedIds) << '\n';
                break;
            }
            case Command::Suggest: {
                int idx = find_flag("-id", argc, argv);
                std::string id;

                if (idx <= -1) {
                    std::cout << "No ID given\n";
                    return;
                }

                id = argv[idx + 1];
                std::cout << sna::get_suggestions(graph, id) << '\n';
                break;
            }
            case Command::Search:
                break;
            default:
                break;
            }

            return;
        }

        if (outputPath.empty()) {
            std::cout << "Output file not specified\n";
            return;
        }

        int tabWidth = get_tab_width(argc, argv);
        std::string outputText;

        switch (command) {
        case Command::Format:
            outputText = xml::format(xmlTree);
            break;

        case Command::Json:
            outputText = xml::to_json(xmlTree);
            break;

        case Command::Minify:
            outputText = xml::minify(inputText);
            break;

        case Command::Compress:
            outputText = xml::compress(xml::minify(inputText));
            tabWidth = -1;
            break;

        default:
            std::cout << "Unknown command: " << argv[1] << std::endl;
            return;
        }

        io::file_write(outputPath, outputText, tabWidth);
    }
}
