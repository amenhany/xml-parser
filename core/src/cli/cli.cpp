#include "xml_editor/cli.hpp"
#include "xml_editor/graph.hpp"
#include "xml_editor/io.hpp"
#include "xml_editor/sna.hpp"
#include "xml_editor/util.hpp"
#include "xml_editor/visualization.hpp"
#include "xml_editor/xml.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

    using xml_editor::cli::Command;

    int find_flag(const char* flag, int argc, char** argv) {
        for (int i = 1; i + 1 < argc; ++i) {
            if (std::strcmp(argv[i], flag) == 0)
                return i;
        }
        return -1;
    }

    std::string get_filepath(int argc, char** argv) {
        int idx = find_flag("-i", argc, argv);
        if (idx == -1 || idx + 1 >= argc)
            return "";
        return argv[idx + 1];
    }

    std::string get_output_path(int argc, char** argv) {
        int idx = find_flag("-o", argc, argv);
        if (idx == -1 || idx + 1 >= argc)
            return "";
        return argv[idx + 1];
    }

    int get_tab_width(int argc, char** argv) {
        int idx = find_flag("-t", argc, argv);
        if (idx == -1 || idx + 1 >= argc)
            return 4;
        return std::max(0, std::stoi(argv[idx + 1]));
    }

    Command parse_command(const std::string& cmd) {
        if (cmd == "verify")
            return Command::Verify;
        if (cmd == "format")
            return Command::Format;
        if (cmd == "mini")
            return Command::Minify;
        if (cmd == "json")
            return Command::Json;
        if (cmd == "compress")
            return Command::Compress;
        if (cmd == "decompress")
            return Command::Decompress;

        if (cmd == "most_influencer")
            return Command::MostInfluencer;
        if (cmd == "most_active")
            return Command::MostActive;
        if (cmd == "mutual")
            return Command::Mutual;
        if (cmd == "suggest")
            return Command::Suggest;
        if (cmd == "search")
            return Command::Search;
        if (cmd == "draw")
            return Command::Draw;

        return Command::Unknown;
    }

    bool needs_graph(Command command) {
        return (command == Command::MostInfluencer ||
            command == Command::MostActive || command == Command::Mutual ||
            command == Command::Suggest || command == Command::Search ||
            command == Command::Draw);
    }
} // namespace

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
        if (inputText.empty())
            return;

        if (command == Command::Decompress) {
            try {
                std::string outputText = xml::decompress(inputText);
                io::file_write(outputPath, outputText);
            }
            catch (std::runtime_error& e) {
                std::cerr << e.what();
            }
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

            if (f == -1)
                return;
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
            const User* user;

            switch (command) {
            case Command::MostInfluencer:
                user = sna::most_influencer(graph);
                std::cout << "Most Influencer User:\n" << *user << '\n';
                break;
            case Command::MostActive:
                user = sna::most_active(graph);
                std::cout << "Most Active User:\n" << *user << '\n';
                break;
            case Command::Mutual: {
                int idx = find_flag("-ids", argc, argv);
                std::string ids;

                if (idx <= -1) {
                    std::cout << "No IDs given\n";
                    return;
                }

                ids = argv[idx + 1];
                std::vector<std::string> separatedIds = util::split_string(ids, ',');
                const std::vector<const User*>& mutuals =
                    sna::get_mutual(graph, separatedIds);

                if (mutuals.size() == 0) {
                    std::cout << "No mutuals between users " << ids << '\n';
                    break;
                }

                std::cout << "Mutuals:\n\n";
                for (const auto& mutual : mutuals) {
                    std::cout << *mutual << '\n';
                }
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
                const std::vector<const User*>& suggestions =
                    sna::get_suggestions(graph, id);

                if (suggestions.size() == 0) {
                    std::cout << "No suggestions for user " << id << '\n';
                    break;
                }

                std::cout << "Suggestions:\n\n";
                for (const auto& suggestion : suggestions) {
                    std::cout << *suggestion << '\n';
                }
                break;
            }
            case Command::Search: {
                int idx =
                    std::max(find_flag("-w", argc, argv), find_flag("-t", argc, argv));
                bool isTopic = std::strcmp(argv[idx], "-t") == 0;
                std::string keyword;

                if (idx <= -1) {
                    std::cout << "No keyword given\n";
                    return;
                }

                keyword = argv[idx + 1];
                const auto& results = isTopic ? sna::search_by_topic(graph, keyword)
                    : sna::search_by_word(graph, keyword);

                if (results.size() == 0) {
                    std::cout << "No results for \"" << keyword << "\"\n";
                    break;
                }

                std::cout << "Results for \"" << keyword << "\":\n\n";
                for (int i = 0; i < results.size(); i++) {
                    std::cout << "Post " << i + 1 << ":\n" << *results[i] << "\n\n";
                }
                break;
            }
            case Command::Draw: {
                if (outputPath.empty()) {
                    std::cout << "Output file not specified\n";
                    return;
                }

                try {
                    draw::run_graphviz(graph.to_dot(), outputPath);
                }
                catch (std::runtime_error& e) {
                    std::cout << e.what() << '\n';
                }
                break;
            }
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
} // namespace xml_editor::cli
