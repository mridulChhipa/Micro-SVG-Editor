#include "src/ui/canvas/Canvas.h"

void Canvas::ApplyDrag(const QPoint& delta) {
  auto shape = ToShapeVariant(selected_shape_);
  if (!shape) return;

  std::visit(
      [&delta](auto&& s) {
        using T = std::decay_t<decltype(s)>;
        if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>) {
          s->x += delta.x();
          s->y += delta.y();
        } else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>) {
          s->x += delta.x();
          s->y += delta.y();
        } else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>) {
          s->x1 += delta.x();
          s->y1 += delta.y();
          s->x2 += delta.x();
          s->y2 += delta.y();
        } else if constexpr (std::is_same_v<T, std::shared_ptr<Path>>)
          for (auto& [_, points] : s->commands)
            for (auto& [x, y] : points) {
              x += delta.x();
              y += delta.y();
            }
        else if constexpr (std::is_same_v<T, std::shared_ptr<Polyline>> ||
                           std::is_same_v<T, std::shared_ptr<Hexagon>>)
          for (auto& [x, y] : s->points) {
            x += delta.x();
            y += delta.y();
          }
        else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>) {
          s->x += delta.x();
          s->y += delta.y();
        }
      },
      *shape);
}

void Canvas::ApplyResize(const QPoint& delta, HandleType handle) {
  auto shape = ToShapeVariant(selected_shape_);
  if (!shape) return;

  bool affects_left =
      (handle == HandleType::kTopLeft || handle == HandleType::kMiddleLeft ||
       handle == HandleType::kBottomLeft);
  bool affects_right =
      (handle == HandleType::kTopRight || handle == HandleType::kMiddleRight ||
       handle == HandleType::kBottomRight);
  bool affects_top =
      (handle == HandleType::kTopLeft || handle == HandleType::kTopCenter ||
       handle == HandleType::kTopRight);
  bool affects_bottom = (handle == HandleType::kBottomLeft ||
                         handle == HandleType::kBottomCenter ||
                         handle == HandleType::kBottomRight);

  int dx = delta.x(), dy = delta.y();

  std::visit(
      [&](auto&& s) {
        using T = std::decay_t<decltype(s)>;
        if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>) {
          if (affects_left) {
            s->x += dx;
            s->width -= dx;
          }
          if (affects_right) {
            s->width += dx;
          }
          if (affects_top) {
            s->y += dy;
            s->height -= dy;
          }
          if (affects_bottom) {
            s->height += dy;
          }
        } else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>)
          CircleResizeHandler(s, dx, dy, affects_left, affects_right,
                              affects_top, affects_bottom);
        else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>) {
          if (affects_left || affects_top) {
            s->x1 += dx;
            s->y1 += dy;
          }
          if (affects_right || affects_bottom) {
            s->x2 += dx;
            s->y2 += dy;
          }
        } else if constexpr (std::is_same_v<T, std::shared_ptr<Hexagon>>)
          HexagonResizeHandler(s, dx, dy, affects_left, affects_right,
                               affects_top, affects_bottom);
      },
      *shape);
}
