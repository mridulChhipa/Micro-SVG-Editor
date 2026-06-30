#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::wheelEvent(QWheelEvent* event) {
  viewport_.ZoomAt(event->position(), event->angleDelta().y() > 0);
  update();
}

void Canvas::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  viewport_.Center(width(), height(), svg_.width(), svg_.height());
  update();
}

void Canvas::ZoomOut() {
  viewport_.ZoomOutStep();
  viewport_.Center(width(), height(), svg_.width(), svg_.height());
  update();
}

void Canvas::ZoomIn() {
  viewport_.ZoomInStep();
  viewport_.Center(width(), height(), svg_.width(), svg_.height());
  update();
}

void Canvas::ZoomReset() {
  viewport_.Reset();
  viewport_.Center(width(), height(), svg_.width(), svg_.height());
  update();
}

}  // namespace micro_svg_editor
