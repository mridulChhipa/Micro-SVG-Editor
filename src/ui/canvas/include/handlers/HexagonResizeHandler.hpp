#ifndef HEX_HANDLER_HPP
#define HEX_HANDLER_HPP

#include <memory>
#include "../../../../model/Hexagon.hpp"

inline void hexagonResizeHandler(std::shared_ptr<Hexagon> s, int dx, int dy, bool affectsLeft, bool affectsRight, bool affectsTop, bool affectsBottom)
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
    if (affectsLeft || affectsRight)
    {
        if (affectsLeft)
            scaleChange = -dx / (oldWidth / 2.0);
        if (affectsRight)
            scaleChange = dx / (oldWidth / 2.0);
    }
    if (affectsTop || affectsBottom)
    {
        double dyScale = 0;
        if (affectsTop)
            dyScale = -dy / (oldHeight / 2.0);
        if (affectsBottom)
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
    if (affectsLeft && !affectsRight)
        centerDx = dx / 2.0;
    if (affectsRight && !affectsLeft)
        centerDx = dx / 2.0;
    if (affectsTop && !affectsBottom)
        centerDy = dy / 2.0;
    if (affectsBottom && !affectsTop)
        centerDy = dy / 2.0;

    for (auto &[x, y] : s->points)
    {
        x += centerDx;
        y += centerDy;
    }
}

#endif