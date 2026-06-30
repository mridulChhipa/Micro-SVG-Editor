#include "src/ui/canvas/handle_geometry.h"

namespace micro_svg_editor {

QRectF HandleRect(const QRectF& bounds, HandleType type) {
  double x, y;
  switch (type) {
    case HandleType::kTopLeft:
      x = bounds.left();
      y = bounds.top();
      break;
    case HandleType::kTopCenter:
      x = bounds.center().x();
      y = bounds.top();
      break;
    case HandleType::kTopRight:
      x = bounds.right();
      y = bounds.top();
      break;
    case HandleType::kMiddleLeft:
      x = bounds.left();
      y = bounds.center().y();
      break;
    case HandleType::kMiddleRight:
      x = bounds.right();
      y = bounds.center().y();
      break;
    case HandleType::kBottomLeft:
      x = bounds.left();
      y = bounds.bottom();
      break;
    case HandleType::kBottomCenter:
      x = bounds.center().x();
      y = bounds.bottom();
      break;
    case HandleType::kBottomRight:
      x = bounds.right();
      y = bounds.bottom();
      break;
    default:
      x = bounds.left();
      y = bounds.top();
      break;
  }
  return QRectF(x - kHandleSize / 2, y - kHandleSize / 2, kHandleSize,
                kHandleSize);
}

HandleType HitTestHandles(const QRectF& bounds, const QPointF& point) {
  for (int i = 0; i < 8; ++i) {
    QRectF handle_rect = HandleRect(bounds, static_cast<HandleType>(i));
    if (handle_rect.contains(point)) return static_cast<HandleType>(i);
  }
  return HandleType::kNone;
}

}  // namespace micro_svg_editor
