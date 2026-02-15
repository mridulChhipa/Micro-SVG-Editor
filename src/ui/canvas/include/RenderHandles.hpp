#ifndef RENDERHANDLES_HPP
#define RENDERHANDLES_HPP

inline QRectF Canvas::renderHandle(const QRectF &obj, HandleType handle_type)
{
  double x, y;
  switch (handle_type)
  {
  case HandleType::TopLeft:
    x = obj.left();
    y = obj.top();
    break;
  case HandleType::TopCenter:
    x = obj.center().x();
    y = obj.top();
    break;
  case HandleType::TopRight:
    x = obj.right();
    y = obj.top();
    break;
  case HandleType::MiddleLeft:
    x = obj.left();
    y = obj.center().y();
    break;
  case HandleType::MiddleRight:
    x = obj.right();
    y = obj.center().y();
    break;
  case HandleType::BottomLeft:
    x = obj.left();
    y = obj.bottom();
    break;
  case HandleType::BottomCenter:
    x = obj.center().x();
    y = obj.bottom();
    break;
  case HandleType::BottomRight:
    x = obj.right();
    y = obj.bottom();
    break;
  default:
    x = obj.left();
    y = obj.top();
    break;
  }
  return QRectF(x - handle_size / 2, y - handle_size / 2, handle_size, handle_size);
}

inline HandleType Canvas::hitTestHandles(const QRectF &obj, const QPointF &point)
{
  for (int i = 0; i < 8; ++i)
  {
    QRectF handleRect = renderHandle(obj, static_cast<HandleType>(i));
    if (handleRect.contains(point))
      return static_cast<HandleType>(i);
  }
  return HandleType::None;
}

#endif