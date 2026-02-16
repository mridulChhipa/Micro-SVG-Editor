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

  double oldWidth = maxX - minX;
  double oldHeight = maxY - minY;
  double centerX = (minX + maxX) / 2.0;
  double centerY = (minY + maxY) / 2.0;

  double scaleChange = 0;
  if (affects_left || affects_right)
  {
    if (affects_left)
      scaleChange = -dx / (oldWidth / 2.0);
    if (affects_right)
      scaleChange = dx / (oldWidth / 2.0);
  }
  if (affects_top || affects_bottom)
  {
    double dyScale = 0;
    if (affects_top)
      dyScale = -dy / (oldHeight / 2.0);
    if (affects_bottom)
      dyScale = dy / (oldHeight / 2.0);

    if (std::abs(dyScale) > std::abs(scaleChange))
      scaleChange = dyScale;
  }

  double scale = 1.0 + scaleChange;
  if (scale < 0.1)
    scale = 0.1;

  for (auto &[x, y] : s->points)
  {
    x = centerX + (x - centerX) * scale;
    y = centerY + (y - centerY) * scale;
  }

  double centerDx = 0, centerDy = 0;
  if (affects_left && !affects_right)
    centerDx = dx / 2.0;
  if (affects_right && !affects_left)
    centerDx = dx / 2.0;
  if (affects_top && !affects_bottom)
    centerDy = dy / 2.0;
  if (affects_bottom && !affects_top)
    centerDy = dy / 2.0;

  for (auto &[x, y] : s->points)
  {
    x += centerDx;
    y += centerDy;
  }
}

#endif