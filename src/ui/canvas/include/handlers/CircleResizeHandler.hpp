#ifndef CIRCLE_HANDLER_HPP
#define CIRCLE_HANDLER_HPP

#include <memory>
#include "../../../../model/Circle.hpp"

inline void circleResizeHandler(std::shared_ptr<Circle> s, int dx, int dy, bool affects_left, bool affects_right, bool affects_top, bool affects_bottom)
{
  double drX = 0, drY = 0;
  if (affects_left)
    drX = -dx;
  else if (affects_right)
    drX = dx;
  if (affects_top)
    drY = -dy;
  else if (affects_bottom)
    drY = dy;

  double dr = std::abs(drX) > std::abs(drY) ? drX : drY;

  s->r = std::max(1.0, s->r + dr / 2.0);
  if (affects_left)
    s->x += dx / 2.0;
  if (affects_right)
    s->x += dx / 2.0;
  if (affects_top)
    s->y += dy / 2.0;
  if (affects_bottom)
    s->y += dy / 2.0;
}

#endif