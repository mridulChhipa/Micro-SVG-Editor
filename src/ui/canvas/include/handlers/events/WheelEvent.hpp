#ifndef WHEEL_EVENT_HPP
#define WHEEL_EVENT_HPP

inline void Canvas::wheelEvent(QWheelEvent *event)
{
  QPointF mouse_pos = event->position();

  float x_old = (mouse_pos.x() / zoom_factor) - x_offset;
  float y_old = (mouse_pos.y() / zoom_factor) - y_offset;

  if (event->angleDelta().y() > 0)
    zoom_factor *= 1.05;
  else
    zoom_factor /= 1.05;

  zoom_factor = std::min(zoom_factor, 10.0);
  zoom_factor = std::max(zoom_factor, 0.3);

  x_offset = (mouse_pos.x() / zoom_factor) - x_old;
  y_offset = (mouse_pos.y() / zoom_factor) - y_old;

  update();
}

inline void Canvas::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);

  x_offset = (width() - svg.width) / 2.0f;
  y_offset = (height() - svg.height) / 2.0f;
  update();
}

#endif