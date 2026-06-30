#include <utility>

#include "src/io/parser.h"

namespace micro_svg_editor {

void Parser::AddTagName(
    std::string tag_name,
    std::unordered_map<std::string, std::string>& attributes) {
  if (auto shape = factory_.Create(tag_name, attributes))
    svg_.Add(std::move(shape));
}

}  // namespace micro_svg_editor
