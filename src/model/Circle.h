#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "GraphicsObject.h"
#include <sstream>

class Circle : public GraphicsObject
{
public:
  float x{0.0f};
  float y{0.0f};
  float r{0.0f};

  Circle() = default;
  Circle(const std::unordered_map<std::string, std::string> &attributes)
  {
    initialiseStyle(attributes);
    x = std::stof(attributes.at("cx"));
    y = std::stof(attributes.at("cy"));
    r = std::stof(attributes.at("r"));
  }

  ~Circle() = default;

  std::string toSVG() const override
  {
    std::ostringstream ss;
    ss << "<circle cx=\"" << x << "\" cy=\"" << y
       << "\" r=\"" << r << "\"" << printStyle() << " />";
    return ss.str();
  }

  std::string type() const override { return "circle"; }
  std::shared_ptr<GraphicsObject> clone() const override
  {
    auto copy = std::make_shared<Circle>();
    copy->x = x;
    copy->y = y;
    copy->r = r;
    copy->id = id;
    copy->class_name = class_name;
    copy->style = style;
    copy->fill = fill;
    copy->fill_opacity = fill_opacity;
    copy->stroke = stroke;
    copy->stroke_width = stroke_width;
    copy->stroke_opacity = stroke_opacity;
    copy->stroke_linecap = stroke_linecap;
    copy->stroke_linejoin = stroke_linejoin;
    copy->stroke_dasharray = stroke_dasharray;
    copy->transform = transform;
    copy->opacity = opacity;
    copy->visibility = visibility;
    return copy;
  }
};
#endif