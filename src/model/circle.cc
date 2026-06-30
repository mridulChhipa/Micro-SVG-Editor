#include "src/model/circle.h"

#include <sstream>
#include <string>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Circle::Circle(const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  x_ = std::stof(attributes.at("cx"));
  y_ = std::stof(attributes.at("cy"));
  r_ = std::stof(attributes.at("r"));
}

std::string Circle::ToSvg() const {
  std::ostringstream ss;
  ss << "<circle cx=\"" << x_ << "\" cy=\"" << y_ << "\" r=\"" << r_ << "\""
     << PrintStyle() << " />";
  return ss.str();
}

void Circle::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Circle::Translate(double dx, double dy) {
  x_ += dx;
  y_ += dy;
}

std::unique_ptr<GraphicsObject> Circle::Clone() const {
  auto copy = std::make_unique<Circle>();
  copy->x_ = x_;
  copy->y_ = y_;
  copy->r_ = r_;
  copy->set_id(id());
  copy->set_class_name(class_name());
  copy->set_style(style());
  copy->set_fill(fill());
  copy->set_fill_opacity(fill_opacity());
  copy->set_stroke(stroke());
  copy->set_stroke_width(stroke_width());
  copy->set_stroke_opacity(stroke_opacity());
  copy->set_stroke_linecap(stroke_linecap());
  copy->set_stroke_linejoin(stroke_linejoin());
  copy->set_stroke_dasharray(stroke_dasharray());
  copy->set_transform(transform());
  copy->set_opacity(opacity());
  copy->set_visibility(visibility());
  return copy;
}

}  // namespace micro_svg_editor
