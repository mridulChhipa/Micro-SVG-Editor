#include "src/io/parser.h"

#include <memory>

#include "src/model/circle.h"
#include "src/model/hexagon.h"
#include "src/model/line.h"
#include "src/model/path.h"
#include "src/model/polyline.h"
#include "src/model/rect.h"
#include "src/model/text.h"

namespace micro_svg_editor {

void Parser::AddTagName(
    std::string tag_name,
    std::unordered_map<std::string, std::string>& attributes) {
  if (tag_name == "rect")
    svg_.Add(std::make_unique<Rect>(attributes));
  else if (tag_name == "line")
    svg_.Add(std::make_unique<Line>(attributes));
  else if (tag_name == "circle")
    svg_.Add(std::make_unique<Circle>(attributes));
  else if (tag_name == "polyline")
    svg_.Add(std::make_unique<Polyline>(attributes));
  else if (tag_name == "polygon")
    svg_.Add(std::make_unique<Hexagon>(attributes));
  else if (tag_name == "path")
    svg_.Add(std::make_unique<Path>(attributes));
  else if (tag_name == "text")
    svg_.Add(std::make_unique<Text>(attributes));
}

}  // namespace micro_svg_editor
