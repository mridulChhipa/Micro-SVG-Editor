#include "src/model/hexagon.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <string>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Hexagon::Hexagon(
    const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  if (attributes.count("points")) {
    std::istringstream ss(attributes.at("points"));
    std::string point;
    points_.clear();
    while (std::getline(ss, point, ' ')) {
      std::istringstream point_ss(point);
      std::string x_str, y_str;
      if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str)) {
        points_.emplace_back(std::stod(x_str), std::stod(y_str));
      }
    }

    // Iterate over C(6, 2) pairs of points to find the minimum distance as the
    // side_length.
    side_length_ = std::numeric_limits<double>::max();
    for (int i = 0; i < 6; i++) {
      for (int j = i + 1; j < 6; j++) {
        double dx = points_[i].first - points_[j].first;
        double dy = points_[i].second - points_[j].second;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist < side_length_) {
          side_length_ = dist;
        }
      }

      x_ += points_[i].first;
      y_ += points_[i].second;
    }

    x_ /= 6.0;
    y_ /= 6.0;
  }
}

void Hexagon::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Hexagon::Translate(double dx, double dy) {
  for (auto& [x, y] : points_) {
    x += dx;
    y += dy;
  }
}

void Hexagon::Resize(int dx, int dy, bool affects_left, bool affects_right,
                     bool affects_top, bool affects_bottom) {
  if (points_.empty()) return;

  double min_x = points_[0].first, max_x = points_[0].first;
  double min_y = points_[0].second, max_y = points_[0].second;
  for (const auto& [x, y] : points_) {
    min_x = std::min(min_x, x);
    max_x = std::max(max_x, x);
    min_y = std::min(min_y, y);
    max_y = std::max(max_y, y);
  }

  double old_width = max_x - min_x;
  double old_height = max_y - min_y;
  double center_x = (min_x + max_x) / 2.0;
  double center_y = (min_y + max_y) / 2.0;

  double scale_change = 0;
  if (affects_left || affects_right) {
    if (affects_left) scale_change = -dx / (old_width / 2.0);
    if (affects_right) scale_change = dx / (old_width / 2.0);
  }
  if (affects_top || affects_bottom) {
    double dy_scale = 0;
    if (affects_top) dy_scale = -dy / (old_height / 2.0);
    if (affects_bottom) dy_scale = dy / (old_height / 2.0);

    if (std::abs(dy_scale) > std::abs(scale_change)) scale_change = dy_scale;
  }

  double scale = 1.0 + scale_change;
  if (scale < 0.1) scale = 0.1;

  for (auto& [x, y] : points_) {
    x = center_x + (x - center_x) * scale;
    y = center_y + (y - center_y) * scale;
  }

  double centerdx = 0, centerdy = 0;
  if (affects_left && !affects_right) centerdx = dx / 2.0;
  if (affects_right && !affects_left) centerdx = dx / 2.0;
  if (affects_top && !affects_bottom) centerdy = dy / 2.0;
  if (affects_bottom && !affects_top) centerdy = dy / 2.0;

  for (auto& [x, y] : points_) {
    x += centerdx;
    y += centerdy;
  }
}

std::string Hexagon::ToSvg() const {
  std::ostringstream ss;
  ss << "<polygon points=\"";
  for (const auto& point : points_) {
    ss << point.first << "," << point.second << " ";
  }
  ss << "\" " << PrintStyle() << "/>";
  return ss.str();
}

std::unique_ptr<GraphicsObject> Hexagon::Clone() const {
  auto copy = std::make_unique<Hexagon>();
  copy->x_ = x_;
  copy->y_ = y_;
  copy->side_length_ = side_length_;
  copy->points_ = points_;
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
