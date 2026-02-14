#ifndef CIRCLE_HANDLER_HPP
#define CIRCLE_HANDLER_HPP

#include <memory>
#include "../../../../model/Circle.hpp"

inline void circleResizeHandler(std::shared_ptr<Circle> s, int dx, int dy, bool affectsLeft, bool affectsRight, bool affectsTop, bool affectsBottom)
{
    double drX = 0, drY = 0;
    if (affectsLeft)
        drX = -dx;
    else if (affectsRight)
        drX = dx;
    if (affectsTop)
        drY = -dy;
    else if (affectsBottom)
        drY = dy;

    double dr = std::abs(drX) > std::abs(drY) ? drX : drY;

    s->r = std::max(1.0, s->r + dr / 2.0);
    if (affectsLeft)
        s->x += dx / 2.0;
    if (affectsRight)
        s->x += dx / 2.0;
    if (affectsTop)
        s->y += dy / 2.0;
    if (affectsBottom)
        s->y += dy / 2.0;
}

#endif