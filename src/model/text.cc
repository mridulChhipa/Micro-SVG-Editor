#include "src/model/text.h"

#include <sstream>
#include <string>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Text::Text(const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  x_ = attributes.count("x") ? std::stof(attributes.at("x")) : 0.0f;
  y_ = attributes.count("y") ? std::stof(attributes.at("y")) : 0.0f;
  dx_ = attributes.count("dx") ? std::stof(attributes.at("dx")) : 0.0f;
  dy_ = attributes.count("dy") ? std::stof(attributes.at("dy")) : 0.0f;
  font_family_ =
      attributes.count("font-family") ? attributes.at("font-family") : "Arial";
  font_size_ = attributes.count("font-size")
                   ? std::stof(attributes.at("font-size"))
                   : 12.0f;
  font_weight_ =
      attributes.count("font-weight") ? attributes.at("font-weight") : "normal";
  text_anchor_ =
      attributes.count("text-anchor") ? attributes.at("text-anchor") : "start";
  content_ =
      attributes.count("content") ? attributes.at("content") : "Sample Text";
}

std::string Text::ToSvg() const {
  std::ostringstream ss;
  ss << "<text x=\"" << x_ << "\" y=\"" << y_ << "\" dx=\"" << dx_ << "\" dy=\""
     << dy_ << "\" font-family=\"" << font_family_ << "\" font-size=\""
     << font_size_ << "\" font-weight=\"" << font_weight_ << "\" text-anchor=\""
     << text_anchor_ << "\"" << PrintStyle() << ">" << content_ << "</text>";
  return ss.str();
}

void Text::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Text::Translate(double dx, double dy) {
  x_ += dx;
  y_ += dy;
}

std::unique_ptr<GraphicsObject> Text::Clone() const {
  auto copy = std::make_unique<Text>();
  copy->x_ = x_;
  copy->y_ = y_;
  copy->dx_ = dx_;
  copy->dy_ = dy_;
  copy->font_family_ = font_family_;
  copy->font_size_ = font_size_;
  copy->font_weight_ = font_weight_;
  copy->text_anchor_ = text_anchor_;
  copy->content_ = content_;

  // Clone style attributes.
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
