#include "src/model/resize_visitor.h"

#include <algorithm>
#include <cmath>

#include "src/model/circle.h"
#include "src/model/hexagon.h"
#include "src/model/line.h"
#include "src/model/rect.h"

namespace micro_svg_editor {

void ResizeVisitor::Visit(Rect& shape) {
  if (affects_left_) {
    shape.set_x(shape.x() + dx_);
    shape.set_width(shape.width() - dx_);
  }
  if (affects_right_) shape.set_width(shape.width() + dx_);
  if (affects_top_) {
    shape.set_y(shape.y() + dy_);
    shape.set_height(shape.height() - dy_);
  }
  if (affects_bottom_) shape.set_height(shape.height() + dy_);
}

void ResizeVisitor::Visit(Circle& shape) {
  double drx = 0, dry = 0;
  if (affects_left_)
    drx = -dx_;
  else if (affects_right_)
    drx = dx_;
  if (affects_top_)
    dry = -dy_;
  else if (affects_bottom_)
    dry = dy_;

  double dr = std::abs(drx) > std::abs(dry) ? drx : dry;

  shape.set_r(std::max(1.0, shape.r() + dr / 2.0));
  if (affects_left_) shape.set_x(shape.x() + dx_ / 2.0);
  if (affects_right_) shape.set_x(shape.x() + dx_ / 2.0);
  if (affects_top_) shape.set_y(shape.y() + dy_ / 2.0);
  if (affects_bottom_) shape.set_y(shape.y() + dy_ / 2.0);
}

void ResizeVisitor::Visit(Line& shape) {
  if (affects_left_ || affects_top_) {
    shape.set_x1(shape.x1() + dx_);
    shape.set_y1(shape.y1() + dy_);
  }
  if (affects_right_ || affects_bottom_) {
    shape.set_x2(shape.x2() + dx_);
    shape.set_y2(shape.y2() + dy_);
  }
}

void ResizeVisitor::Visit(Hexagon& shape) {
  auto& points = shape.points();
  if (points.empty()) return;

  double min_x = points[0].first, max_x = points[0].first;
  double min_y = points[0].second, max_y = points[0].second;
  for (const auto& [x, y] : points) {
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
  if (affects_left_ || affects_right_) {
    if (affects_left_) scale_change = -dx_ / (old_width / 2.0);
    if (affects_right_) scale_change = dx_ / (old_width / 2.0);
  }
  if (affects_top_ || affects_bottom_) {
    double dy_scale = 0;
    if (affects_top_) dy_scale = -dy_ / (old_height / 2.0);
    if (affects_bottom_) dy_scale = dy_ / (old_height / 2.0);

    if (std::abs(dy_scale) > std::abs(scale_change)) scale_change = dy_scale;
  }

  double scale = 1.0 + scale_change;
  if (scale < 0.1) scale = 0.1;

  for (auto& [x, y] : points) {
    x = center_x + (x - center_x) * scale;
    y = center_y + (y - center_y) * scale;
  }

  double centerdx = 0, centerdy = 0;
  if (affects_left_ && !affects_right_) centerdx = dx_ / 2.0;
  if (affects_right_ && !affects_left_) centerdx = dx_ / 2.0;
  if (affects_top_ && !affects_bottom_) centerdy = dy_ / 2.0;
  if (affects_bottom_ && !affects_top_) centerdy = dy_ / 2.0;

  for (auto& [x, y] : points) {
    x += centerdx;
    y += centerdy;
  }
}

}  // namespace micro_svg_editor
