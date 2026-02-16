#ifndef HEX_HANDLER_HPP
#define HEX_HANDLER_HPP

#include <memory>
#include "src/model/Hexagon.h"

inline void hexagonResizeHandler(std::shared_ptr<Hexagon> s, int dx, int dy, bool affects_left, bool affects_right, bool affects_top, bool affects_bottom)
{
  if (s->points.empty())
    return;

  double minX = s->points[0].first, maxX = s->points[0].first;
  double minY = s->points[0].second, maxY = s->points[0].second;
  for (const auto &[x, y] : s->points)
  {
    minX = std::min(minX, x);
    maxX = std::max(maxX, x);
    minY = std::min(minY, y);
    maxY = std::max(maxY, y);
  }

  double old_width = maxX - minX;
  double old_height = maxY - minY;
  double centerX = (minX + maxX) / 2.0;
  double centerY = (minY + maxY) / 2.0;

  double scale_change = 0;
  if (affects_left || affects_right)
  {
    if (affects_left)
      scale_change = -dx / (old_width / 2.0);
    if (affects_right)
      scale_change = dx / (old_width / 2.0);
  }
  if (affects_top || affects_bottom)
  {
    double dy_scale = 0;
    if (affects_top)
      dy_scale = -dy / (old_height / 2.0);
    if (affects_bottom)
      dy_scale = dy / (old_height / 2.0);

    if (std::abs(dy_scale) > std::abs(scale_change))
      scale_change = dy_scale;
  }

  double scale = 1.0 + scale_change;
  if (scale < 0.1)
    scale = 0.1;

  for (auto &[x, y] : s->points)
  {
    x = centerX + (x - centerX) * scale;
    y = centerY + (y - centerY) * scale;
  }

  double centerdx = 0, centerdy = 0;
  if (affects_left && !affects_right)
    centerdx = dx / 2.0;
  if (affects_right && !affects_left)
    centerdx = dx / 2.0;
  if (affects_top && !affects_bottom)
    centerdy = dy / 2.0;
  if (affects_bottom && !affects_top)
    centerdy = dy / 2.0;

  for (auto &[x, y] : s->points)
  {
    x += centerdx;
    y += centerdy;
  }
}

#endif