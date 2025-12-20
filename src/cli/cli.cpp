#include "xml_editor/cli.hpp"
#include "xml_editor/xml.hpp"
#include "xml_editor/io.hpp"

#include <iostream>
#include <cstring>

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
        return Command::Unknown;
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

        if (outputPath.empty()) {
            std::cout << "Output file not specified\n";
            return;
        }

        int tabWidth = get_tab_width(argc, argv);

        Tree* xmlTree = xml::parse(inputText);
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
