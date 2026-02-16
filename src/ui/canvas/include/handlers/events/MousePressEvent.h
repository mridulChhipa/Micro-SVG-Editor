#ifndef MOUSE_PRESS_HPP
#define MOUSE_PRESS_HPP

inline void Canvas::mousePressEvent(QMouseEvent *event)
{
  QPointF canvas_pnt = toCanvasCoordinates(event->pos());
  if (isPerformingUndoRedo)
    return;
  if (curr_tool != "" && curr_tool != "Freehand")
  {
    addShapeToCanvas(curr_tool.toStdString(), toCanvasCoordinates(event->pos()));
    update();
  }

  if (event->button() == Qt::LeftButton && curr_tool == "Freehand")
  {
    current_path = QPainterPath();
    current_path.moveTo(canvas_pnt);
    is_drawing = true;
    return;
  }

  if (selected_shape)
  {
    isPerformingUndoRedo = false;
    QPainterPath sel_path;
    QPen sel_pen;
    createObject(selected_shape, sel_path, sel_pen);
    QTransform transform = findTransform(selected_shape);

    QPointF local_point = toCanvasCoordinates(event->pos());
    // Qt does not translates by default the bounding rect so need to do that manually
    if (!transform.isIdentity())
    {
      QTransform inverted = transform.inverted();
      local_point = inverted.map(local_point);
    }

    QRectF bnd_rect = sel_path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust);
    HandleType handle = hitTestHandles(bnd_rect, local_point);

    if (handle != HandleType::None)
    {
      is_resizing = true;

      isPerformingUndoRedo = true;

      temp_stack.push_back(svg.clone()); // Save current state for potential undo

      curr_handle = handle;
      start_rect = bnd_rect;
      last_point = toCanvasCoordinates(event->pos()).toPoint();
      dragging = false;
      return;
    }
  }

  selected_shape = nullptr;
  dragging = false;
  is_resizing = false;
  curr_handle = HandleType::None;
  isPerformingUndoRedo = false;

  // Reverse order traverse so that you always check hitting from top to bottom
  for (const GraphicsObjectPtr &obj : svg.objects | std::views::reverse)
  {
    QPainterPath path;
    QPen pen;
    createObject(obj, path, pen);
    QTransform transform = findTransform(obj);

    QPointF local_point = toCanvasCoordinates(event->pos());
    if (!transform.isIdentity())
    {
      QTransform inverted = transform.inverted();
      local_point = inverted.map(local_point);
    }

    // Adjust bounding rectangle in case of line and polyline
    if (obj->type() == "line" || obj->type() == "polyline")
    {
      if (path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust).contains(local_point))
      {
        dragging = true;
        selected_shape = obj;
        last_point = toCanvasCoordinates(event->pos()).toPoint();
        temp_stack.push_back(svg.clone());
        isPerformingUndoRedo = true;
        break;
      }
    }
    else if (path.boundingRect().contains(local_point))
    {
      dragging = true;
      selected_shape = obj;
      last_point = toCanvasCoordinates(event->pos()).toPoint();
      temp_stack.push_back(svg.clone());
      isPerformingUndoRedo = true;
      break;
    }
  }
}

#endif