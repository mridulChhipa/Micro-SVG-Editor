#include "src/model/line.h"

#include <sstream>
#include <string>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Line::Line(const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  x1_ = attributes.count("x1") ? std::stof(attributes.at("x1")) : 0.0f;
  y1_ = attributes.count("y1") ? std::stof(attributes.at("y1")) : 0.0f;
  x2_ = attributes.count("x2") ? std::stof(attributes.at("x2")) : 0.0f;
  y2_ = attributes.count("y2") ? std::stof(attributes.at("y2")) : 0.0f;
}

void Line::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Line::Translate(double dx, double dy) {
  x1_ += dx;
  y1_ += dy;
  x2_ += dx;
  y2_ += dy;
}

std::string Line::ToSvg() const {
  std::ostringstream ss;
  ss << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" ";
  ss << "x2=\"" << x2_ << "\" y2=\"" << y2_ << "\" " << PrintStyle() << " />";
  return ss.str();
}

std::unique_ptr<GraphicsObject> Line::Clone() const {
  auto copy = std::make_unique<Line>();
  copy->x1_ = x1_;
  copy->y1_ = y1_;
  copy->x2_ = x2_;
  copy->y2_ = y2_;
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
