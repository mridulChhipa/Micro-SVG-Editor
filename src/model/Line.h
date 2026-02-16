#ifndef LINE_HPP
#define LINE_HPP

#include "GraphicsObject.h"
#include <sstream>

class Line : public GraphicsObject
{
public:
  float x1{0}, y1{0};
  float x2{0}, y2{0};

  Line() = default;
  Line(const std::unordered_map<std::string, std::string> &attributes)
  {
    initialiseStyle(attributes);
    x1 = attributes.count("x1") ? std::stof(attributes.at("x1")) : 0.0f;
    y1 = attributes.count("y1") ? std::stof(attributes.at("y1")) : 0.0f;
    x2 = attributes.count("x2") ? std::stof(attributes.at("x2")) : 0.0f;
    y2 = attributes.count("y2") ? std::stof(attributes.at("y2")) : 0.0f;
  }
  ~Line() = default;

  std::string type() const override
  {
    return "line";
  }

  std::string toSVG() const override
  {
    std::ostringstream ss;
    ss << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" ";
    ss << "x2=\"" << x2 << "\" y2=\"" << y2 << "\" " << printStyle() << " />";
    return ss.str();
  }

  std::shared_ptr<GraphicsObject> clone() const override
  {
    auto copy = std::make_shared<Line>();
    copy->x1 = x1;
    copy->y1 = y1;
    copy->x2 = x2;
    copy->y2 = y2;
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