#ifndef MOUSE_MOVE_HPP
#define MOUSE_MOVE_HPP

inline void Canvas::mouseMoveEvent(QMouseEvent *event)
{
  if ((event->buttons() & Qt::LeftButton) && is_drawing && curr_tool == "Freehand")
  {
    current_path.lineTo(toCanvasCoordinates(event->pos()));
    update();
  }

  if (!(event->buttons() & Qt::LeftButton) || !selected_shape || (!dragging && !is_resizing))
    return;

  QPoint delta = toCanvasCoordinates(event->pos()).toPoint() - last_point;
  last_point = toCanvasCoordinates(event->pos()).toPoint();

  QTransform transform = findTransform(selected_shape);
  if (!transform.isIdentity())
  {
    QTransform inverted = transform.inverted();
    QPointF transformed_delta = inverted.map(QPointF(delta)) - inverted.map(QPointF(0, 0));
    delta = transformed_delta.toPoint();
  }

  if (!is_resizing)
    applyDrag(delta);
  else
    applyResize(delta, curr_handle);
  update();
}

#endif