#ifndef DRAG_AND_RESIZE_HPP
#define DRAG_AND_RESIZE_HPP

inline void Canvas::applyDrag(const QPoint &delta)
{
    auto shape = toShapeVariant(selected_shape);
    if (!shape)
        return;

    std::visit([&delta](auto &&s)
               {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>)
            {
                s->x += delta.x();
                s->y += delta.y();
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>)
            {
                s->x += delta.x();
                s->y += delta.y();
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>)
            {
                s->x1 += delta.x();
                s->y1 += delta.y();
                s->x2 += delta.x();
                s->y2 += delta.y();
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Path>>)
            {
                for (auto &[_, points] : s->commands)
                {
                    for (auto &[x, y] : points)
                    {
                        x += delta.x();
                        y += delta.y();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Polyline>> ||
                               std::is_same_v<T, std::shared_ptr<Hexagon>>)
            {
                for (auto &[x, y] : s->points)
                {
                    x += delta.x();
                    y += delta.y();
                }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>)
            {
                s->x += delta.x();
                s->y += delta.y();
            } }, *shape);
}

inline void Canvas::applyResize(const QPoint &delta, HandleType handle)
{
    auto shape = toShapeVariant(selected_shape);
    if (!shape)
        return;

    bool affectsLeft = (handle == HandleType::TopLeft || handle == HandleType::MiddleLeft || handle == HandleType::BottomLeft);
    bool affectsRight = (handle == HandleType::TopRight || handle == HandleType::MiddleRight || handle == HandleType::BottomRight);
    bool affectsTop = (handle == HandleType::TopLeft || handle == HandleType::TopCenter || handle == HandleType::TopRight);
    bool affectsBottom = (handle == HandleType::BottomLeft || handle == HandleType::BottomCenter || handle == HandleType::BottomRight);

    int dx = delta.x();
    int dy = delta.y();

    std::visit([&](auto &&s)
               {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>)
            {
                if (affectsLeft)   { s->x += dx; s->width  -= dx; }
                if (affectsRight)  { s->width  += dx; }
                if (affectsTop)    { s->y += dy; s->height -= dy; }
                if (affectsBottom) { s->height += dy; }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>)
            {
                double drX = 0, drY = 0;
                if (affectsLeft)        drX = -dx;
                else if (affectsRight)  drX = dx;
                if (affectsTop)         drY = -dy;
                else if (affectsBottom) drY = dy;

                double dr = std::abs(drX) > std::abs(drY) ? drX : drY;

                s->r = std::max(1.0, s->r + dr / 2.0);
                if (affectsLeft)   s->x += dx / 2.0;
                if (affectsRight)  s->x += dx / 2.0;
                if (affectsTop)    s->y += dy / 2.0;
                if (affectsBottom) s->y += dy / 2.0;
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>)
            {
                if (affectsLeft || affectsTop)
                {
                    s->x1 += dx;
                    s->y1 += dy;
                }
                if (affectsRight || affectsBottom)
                {
                    s->x2 += dx;
                    s->y2 += dy;
                }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Hexagon>>)
            {
                if (s->points.empty()) return;

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
                    if (affectsLeft)  scaleChange = -dx / (oldWidth / 2.0);
                    if (affectsRight) scaleChange = dx / (oldWidth / 2.0);
                }
                if (affectsTop || affectsBottom)
                {
                    double dyScale = 0;
                    if (affectsTop)    dyScale = -dy / (oldHeight / 2.0);
                    if (affectsBottom) dyScale = dy / (oldHeight / 2.0);
                    
                    if (std::abs(dyScale) > std::abs(scaleChange))
                        scaleChange = dyScale;
                }

                double scale = 1.0 + scaleChange;
                if (scale < 0.1) scale = 0.1;

                for (auto &[x, y] : s->points)
                {
                    x = centerX + (x - centerX) * scale;
                    y = centerY + (y - centerY) * scale;
                }

                double centerDx = 0, centerDy = 0;
                if (affectsLeft && !affectsRight)   centerDx = dx / 2.0;
                if (affectsRight && !affectsLeft)   centerDx = dx / 2.0;
                if (affectsTop && !affectsBottom)   centerDy = dy / 2.0;
                if (affectsBottom && !affectsTop)   centerDy = dy / 2.0;

                for (auto &[x, y] : s->points)
                {
                    x += centerDx;
                    y += centerDy;
                }
            }else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>)
            {
                if (affectsLeft)  s->x += dx;
                if (affectsTop)   s->y += dy;
                int sizeDelta = 0;
                if (affectsRight)       sizeDelta += dx;
                else if (affectsLeft)   sizeDelta -= dx;
                if (affectsBottom)      sizeDelta += dy;
                else if (affectsTop)    sizeDelta -= dy;
                s->font_size = std::max(1.0, s->font_size + sizeDelta / 10.0);
            } }, *shape);
}

#endif