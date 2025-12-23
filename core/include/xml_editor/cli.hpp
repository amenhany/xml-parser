#ifndef XML_EDITOR_CLI
#define XML_EDITOR_CLI

namespace xml_editor::cli {
    enum class Command {
        Verify,
        Format,
        Json,
        Minify,
        Compress,
        Decompress,

        MostInfluencer,
        MostActive,
        Mutual,
        Suggest,
        Search,
        Draw,

        Unknown
    };

    void run_cli(int argc, char** argv);
}

#endif
