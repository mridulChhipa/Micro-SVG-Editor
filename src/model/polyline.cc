#include "src/model/polyline.h"

#include <sstream>
#include <string>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Polyline::Polyline(
    const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  if (attributes.count("points")) {
    std::istringstream ss(attributes.at("points"));
    std::string point;
    while (std::getline(ss, point, ' ')) {
      int comma_pos = point.find(',');
      if (comma_pos != static_cast<int>(std::string::npos)) {
        float x = std::stof(point.substr(0, comma_pos));
        float y = std::stof(point.substr(comma_pos + 1));
        points_.emplace_back(x, y);
      }
    }
  }
}

void Polyline::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Polyline::Translate(double dx, double dy) {
  for (auto& [x, y] : points_) {
    x += dx;
    y += dy;
  }
}

std::string Polyline::ToSvg() const {
  std::ostringstream ss;
  ss << "<polyline points=\"";
  for (const auto& point : points_) {
    ss << point.first << "," << point.second << " ";
  }
  ss << "\" " << PrintStyle() << "/>";
  return ss.str();
}

std::unique_ptr<GraphicsObject> Polyline::Clone() const {
  auto copy = std::make_unique<Polyline>();
  copy->points_ = points_;
  copy->set_fill(fill());
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
