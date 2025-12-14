#include "xml_editor/xml.hpp"

namespace xml_editor::xml {
    void recFormat(TreeNode* node, int depth, std::string& out) {
        std::string indent(depth, '\t');
        std::string end_tag = "</" + node->get_tag() + ">";
        out += indent + "<" + node->get_tag() + ">";

        for (TreeNode* child : node->get_children()) {
            out += "\n";
            recFormat(child, depth + 1, out);
        }

        if (!(node->get_value().empty())) {
            std::string value = node->get_value();
            if (value.find('\n') != std::string::npos) {
                size_t pos = 0;
                while ((pos = value.find('\n', pos)) != std::string::npos) {
                    pos++;
                    value.insert(pos, indent + "\t");
                    pos += indent.length() + 1;
                }
                out += "\n" + indent + "\t" + value + "\n" + indent + end_tag;
            }
            else
                out += value + end_tag;
        }
        else
            out += "\n" + indent + end_tag;
    }

    std::string format(Tree* tree) {
        std::string result = "";
        recFormat(tree->get_root(), 0, result);
        return result;
    }
}
