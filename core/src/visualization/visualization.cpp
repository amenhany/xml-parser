#include "xml_editor/visualization.hpp"
#include "xml_editor/util.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <unordered_set>


namespace xml_editor::draw {

static const std::unordered_set<std::string> supported_formats = {
    "bmp",        "cgimage", "eps",  "exr",  "fig",  "gd",   "gd2",  "gif",
    "ico",        "jpg",     "jpeg", "jpe",  "jp2",  "pdf",  "pic",  "pct",
    "pict",       "png",     "ps",   "ps2",  "psd",  "sgi",  "svg",  "svgz",
    "svg_inline", "tga",     "tif",  "tiff", "wbmp", "webp", "xlib", "x11"};

void run_graphviz(const std::string &graph, const std::string &outputPath) {
  std::string extension = util::split_string(outputPath, '.').back();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (!supported_formats.count(extension))
    throw std::runtime_error("Unsupported extension: ." + extension);

  std::string command = "echo \"" + graph + "\" | dot -T" + extension +
                        " -o \"" + outputPath + "\"";
  system(command.c_str());
}
} // namespace xml_editor::draw
