#include "src/ui/canvas/Canvas.h"

QRectF Canvas::RenderHandle(const QRectF& obj, HandleType handle_type) {
  double x, y;
  switch (handle_type) {
    case HandleType::kTopLeft:
      x = obj.left();
      y = obj.top();
      break;
    case HandleType::kTopCenter:
      x = obj.center().x();
      y = obj.top();
      break;
    case HandleType::kTopRight:
      x = obj.right();
      y = obj.top();
      break;
    case HandleType::kMiddleLeft:
      x = obj.left();
      y = obj.center().y();
      break;
    case HandleType::kMiddleRight:
      x = obj.right();
      y = obj.center().y();
      break;
    case HandleType::kBottomLeft:
      x = obj.left();
      y = obj.bottom();
      break;
    case HandleType::kBottomCenter:
      x = obj.center().x();
      y = obj.bottom();
      break;
    case HandleType::kBottomRight:
      x = obj.right();
      y = obj.bottom();
      break;
    default:
      x = obj.left();
      y = obj.top();
      break;
  }
  return QRectF(x - kHandleSize / 2, y - kHandleSize / 2, kHandleSize,
                kHandleSize);
}

HandleType Canvas::HitTestHandles(const QRectF& obj, const QPointF& point) {
  for (int i = 0; i < 8; ++i) {
    QRectF handle_rect = RenderHandle(obj, static_cast<HandleType>(i));
    if (handle_rect.contains(point)) return static_cast<HandleType>(i);
  }
  return HandleType::kNone;
}
