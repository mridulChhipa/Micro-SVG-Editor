#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::mouseMoveEvent(QMouseEvent* event) {
  if ((event->buttons() & Qt::LeftButton) && is_drawing_ &&
      curr_tool_ == "Freehand") {
    current_path_.lineTo(ToCanvasCoordinates(event->pos()));
    update();
  }

  if (!(event->buttons() & Qt::LeftButton) || !selected_shape_ ||
      (!dragging_ && !is_resizing_))
    return;

  QPoint delta = ToCanvasCoordinates(event->pos()).toPoint() - last_point_;
  last_point_ = ToCanvasCoordinates(event->pos()).toPoint();

  QTransform transform = FindTransform(selected_shape_);
  if (!transform.isIdentity()) {
    QTransform inverted = transform.inverted();
    QPointF transformed_delta =
        inverted.map(QPointF(delta)) - inverted.map(QPointF(0, 0));
    delta = transformed_delta.toPoint();
  }

  if (!is_resizing_)
    ApplyDrag(delta);
  else
    ApplyResize(delta, curr_handle_);
  update();
}

}  // namespace micro_svg_editor
