#ifndef DRAG_AND_RESIZE_HPP
#define DRAG_AND_RESIZE_HPP

#include "handlers/HexagonResizeHandler.hpp"
#include "handlers/CircleResizeHandler.hpp"

inline void Canvas::applyDrag(const QPoint &delta)
{
    auto shape = toShapeVariant(selected_shape);
    if (!shape)
        return;

    std::visit([&delta](auto &&s)
               {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>) { s->x += delta.x(); s->y += delta.y(); }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>) { s->x += delta.x(); s->y += delta.y();}
            else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>) { s->x1 += delta.x(); s->y1 += delta.y(); s->x2 += delta.x(); s->y2 += delta.y(); }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Path>>)
                for (auto &[_, points] : s->commands)
                    for (auto &[x, y] : points) { x += delta.x(); y += delta.y(); }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Polyline>> || std::is_same_v<T, std::shared_ptr<Hexagon>>)
                for (auto &[x, y] : s->points) { x += delta.x(); y += delta.y(); }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>) { s->x += delta.x(); s->y += delta.y();} }, *shape);
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

    int dx = delta.x(), dy = delta.y();

    std::visit([&](auto &&s)
               {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>) {
                if (affectsLeft)   { s->x += dx; s->width  -= dx; }
                if (affectsRight)  { s->width  += dx; }
                if (affectsTop)    { s->y += dy; s->height -= dy; }
                if (affectsBottom) { s->height += dy; }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>)
               circleResizeHandler(s, dx, dy, affectsLeft, affectsRight, affectsTop, affectsBottom);
            else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>)
            {
                if (affectsLeft || affectsTop) { s->x1 += dx; s->y1 += dy; }
                if (affectsRight || affectsBottom) { s->x2 += dx; s->y2 += dy; }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Hexagon>>)
                hexagonResizeHandler(s, dx, dy, affectsLeft, affectsRight, affectsTop, affectsBottom); }, *shape);
}

#endif