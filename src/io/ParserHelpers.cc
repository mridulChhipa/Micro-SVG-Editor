#include "src/io/Parser.h"

#include <memory>

#include "src/model/Circle.h"
#include "src/model/Hexagon.h"
#include "src/model/Line.h"
#include "src/model/Path.h"
#include "src/model/Polyline.h"
#include "src/model/Rect.h"
#include "src/model/Text.h"

void Parser::AddTagName(
    std::string tag_name,
    std::unordered_map<std::string, std::string>& attributes) {
  if (tag_name == "rect")
    svg_.Add(std::make_shared<Rect>(attributes));
  else if (tag_name == "line")
    svg_.Add(std::make_shared<Line>(attributes));
  else if (tag_name == "circle")
    svg_.Add(std::make_shared<Circle>(attributes));
  else if (tag_name == "polyline")
    svg_.Add(std::make_shared<Polyline>(attributes));
  else if (tag_name == "polygon")
    svg_.Add(std::make_shared<Hexagon>(attributes));
  else if (tag_name == "path")
    svg_.Add(std::make_shared<Path>(attributes));
  else if (tag_name == "text")
    svg_.Add(std::make_shared<Text>(attributes));
}
