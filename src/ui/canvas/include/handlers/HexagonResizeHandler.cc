#include "src/ui/canvas/Canvas.h"

void Canvas::HexagonResizeHandler(std::shared_ptr<Hexagon> s, int dx, int dy,
                                  bool affects_left, bool affects_right,
                                  bool affects_top, bool affects_bottom) {
  if (s->points.empty()) return;

  double min_x = s->points[0].first, max_x = s->points[0].first;
  double min_y = s->points[0].second, max_y = s->points[0].second;
  for (const auto& [x, y] : s->points) {
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

  for (auto& [x, y] : s->points) {
    x = center_x + (x - center_x) * scale;
    y = center_y + (y - center_y) * scale;
  }

  double centerdx = 0, centerdy = 0;
  if (affects_left && !affects_right) centerdx = dx / 2.0;
  if (affects_right && !affects_left) centerdx = dx / 2.0;
  if (affects_top && !affects_bottom) centerdy = dy / 2.0;
  if (affects_bottom && !affects_top) centerdy = dy / 2.0;

  for (auto& [x, y] : s->points) {
    x += centerdx;
    y += centerdy;
  }
}