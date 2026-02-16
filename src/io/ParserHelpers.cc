#include "src/io/Parser.h"

void Parser::addTagName(std::string tag_name, std::unordered_map<std::string, std::string> &attributes)
{
    if (tag_name == "rect")
        svg.add(std::make_shared<Rect>(attributes));
    else if (tag_name == "line")
        svg.add(std::make_shared<Line>(attributes));
    else if (tag_name == "circle")
        svg.add(std::make_shared<Circle>(attributes));
    else if (tag_name == "polyline")
        svg.add(std::make_shared<Polyline>(attributes));
    else if (tag_name == "polygon")
        svg.add(std::make_shared<Hexagon>(attributes));
    else if (tag_name == "path")
        svg.add(std::make_shared<Path>(attributes));
    else if (tag_name == "text")
        svg.add(std::make_shared<Text>(attributes));
}
