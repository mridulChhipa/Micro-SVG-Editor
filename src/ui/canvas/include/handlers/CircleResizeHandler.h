#ifndef CIRCLE_HANDLER_HPP
#define CIRCLE_HANDLER_HPP

#include <memory>
#include "src/model/Circle.h"

inline void circleResizeHandler(std::shared_ptr<Circle> s, int dx, int dy, bool affects_left, bool affects_right, bool affects_top, bool affects_bottom)
{
  double drx = 0, dry = 0;
  if (affects_left)
    drx = -dx;
  else if (affects_right)
    drx = dx;
  if (affects_top)
    dry = -dy;
  else if (affects_bottom)
    dry = dy;

  double dr = std::abs(drx) > std::abs(dry) ? drx : dry;

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