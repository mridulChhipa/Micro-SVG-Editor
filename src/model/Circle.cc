#include "src/model/Circle.h"

#include <sstream>
#include <string>

Circle::Circle(const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  x = std::stof(attributes.at("cx"));
  y = std::stof(attributes.at("cy"));
  r = std::stof(attributes.at("r"));
}

std::string Circle::ToSvg() const {
  std::ostringstream ss;
  ss << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << r << "\""
     << PrintStyle() << " />";
  return ss.str();
}

std::string Circle::Type() const { return "circle"; }

std::shared_ptr<GraphicsObject> Circle::Clone() const {
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
