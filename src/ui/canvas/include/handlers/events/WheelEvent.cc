#include "src/ui/canvas/Canvas.h"

void Canvas::wheelEvent(QWheelEvent* event) {
  QPointF mouse_pos = event->position();

  float x_old = (mouse_pos.x() / zoom_factor_) - x_offset_;
  float y_old = (mouse_pos.y() / zoom_factor_) - y_offset_;

  if (event->angleDelta().y() > 0)
    zoom_factor_ *= 1.05;
  else
    zoom_factor_ /= 1.05;

  zoom_factor_ = std::min(zoom_factor_, 10.0);
  zoom_factor_ = std::max(zoom_factor_, 0.3);

  x_offset_ = (mouse_pos.x() / zoom_factor_) - x_old;
  y_offset_ = (mouse_pos.y() / zoom_factor_) - y_old;

  update();
}

void Canvas::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  x_offset_ = (width() - svg_.width * zoom_factor_) / 2.0f;
  y_offset_ = (height() - svg_.height * zoom_factor_) / 2.0f;
  update();
}

void Canvas::ZoomOut() {
  zoom_factor_ = std::max(zoom_factor_ / 1.1, 0.3);
  x_offset_ = (width() - svg_.width * zoom_factor_) / 2.0f;
  y_offset_ = (height() - svg_.height * zoom_factor_) / 2.0f;

  update();
}

void Canvas::ZoomIn() {
  zoom_factor_ = std::min(zoom_factor_ * 1.1, 10.0);
  x_offset_ = (width() - svg_.width * zoom_factor_) / 2.0f;
  y_offset_ = (height() - svg_.height * zoom_factor_) / 2.0f;

  update();
}

void Canvas::ZoomReset() {
  zoom_factor_ = 1;
  x_offset_ = (width() - svg_.width) / 2.0f;
  y_offset_ = (height() - svg_.height) / 2.0f;
  update();
}
