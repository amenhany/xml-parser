#include "xml_editor/cli.hpp"
#include "gui.h"
#include <cstring>

using namespace xml_editor;

int main(int argc, char* argv[]) {
    if (argc <= 1 || std::strcmp(argv[1], "--gui") == 0) {
        return gui::run_gui(argc, argv);
    }

    cli::run_cli(argc, argv);
    return 0;
}
