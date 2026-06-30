#include "src/model/rect.h"

#include <sstream>
#include <string>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Rect::Rect(const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  x_ = attributes.count("x") ? std::stof(attributes.at("x")) : 0.0f;
  y_ = attributes.count("y") ? std::stof(attributes.at("y")) : 0.0f;
  width_ = attributes.count("width") ? std::stof(attributes.at("width")) : 0.0f;
  height_ =
      attributes.count("height") ? std::stof(attributes.at("height")) : 0.0f;
  rx_ = attributes.count("rx") ? std::stof(attributes.at("rx")) : 0.0f;
  ry_ = attributes.count("ry") ? std::stof(attributes.at("ry")) : 0.0f;
}

void Rect::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Rect::Translate(double dx, double dy) {
  x_ += dx;
  y_ += dy;
}

std::string Rect::ToSvg() const {
  std::ostringstream ss;
  ss << "<rect x=\"" << x_ << "\" y=\"" << y_ << "\" width=\"" << width_
     << "\" height=\"" << height_ << "\"";
  if (rx_ > 0.0f) ss << " rx=\"" << rx_ << "\"";
  if (ry_ > 0.0f) ss << " ry=\"" << ry_ << "\"";
  ss << PrintStyle() << " />";
  return ss.str();
}

std::unique_ptr<GraphicsObject> Rect::Clone() const {
  auto copy = std::make_unique<Rect>();
  copy->x_ = x_;
  copy->y_ = y_;
  copy->width_ = width_;
  copy->height_ = height_;
  copy->rx_ = rx_;
  copy->ry_ = ry_;
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
