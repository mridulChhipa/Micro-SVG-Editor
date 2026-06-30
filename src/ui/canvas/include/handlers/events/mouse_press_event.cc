#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::mousePressEvent(QMouseEvent* event) {
  QPointF canvas_pnt = ToCanvasCoordinates(event->pos());
  if (is_performing_undo_redo_) return;
  if (curr_tool_ != "" && curr_tool_ != "Freehand") {
    AddShapeToCanvas(curr_tool_.toStdString(),
                     ToCanvasCoordinates(event->pos()));
    update();
  }

  if (event->button() == Qt::LeftButton && curr_tool_ == "Freehand") {
    current_path_ = QPainterPath();
    current_path_.moveTo(canvas_pnt);
    is_drawing_ = true;
    return;
  }

  if (selected_shape_) {
    is_performing_undo_redo_ = false;
    QPainterPath sel_path;
    QPen sel_pen;
    CreateObject(selected_shape_, sel_path, sel_pen);
    QTransform transform = FindTransform(selected_shape_);

    QPointF local_point = ToCanvasCoordinates(event->pos());
    // Qt does not translates by default the bounding rect so need to do that
    // manually
    if (!transform.isIdentity()) {
      QTransform inverted = transform.inverted();
      local_point = inverted.map(local_point);
    }

    QRectF bnd_rect =
        sel_path.boundingRect().adjusted(-kAdjust, -kAdjust, kAdjust, kAdjust);
    HandleType handle = HitTestHandles(bnd_rect, local_point);

    if (handle != HandleType::kNone) {
      is_resizing_ = true;

      is_performing_undo_redo_ = true;

      temp_stack_.push_back(
          svg_.Clone());  // Save the current state for a potential undo

      curr_handle_ = handle;
      start_rect_ = bnd_rect;
      last_point_ = ToCanvasCoordinates(event->pos()).toPoint();
      dragging_ = false;
      return;
    }
  }

  selected_shape_ = nullptr;
  dragging_ = false;
  is_resizing_ = false;
  curr_handle_ = HandleType::kNone;
  is_performing_undo_redo_ = false;

  // Reverse order traverse so that you always check hitting from top to bottom
  for (const GraphicsObjectPtr& obj : svg_.objects() | std::views::reverse) {
    QPainterPath path;
    QPen pen;
    CreateObject(obj.get(), path, pen);
    QTransform transform = FindTransform(obj.get());

    QPointF local_point = ToCanvasCoordinates(event->pos());
    if (!transform.isIdentity()) {
      QTransform inverted = transform.inverted();
      local_point = inverted.map(local_point);
    }

    // Adjust bounding rectangle for shapes that are essentially strokes.
    if (obj->PreferStrokeHitTest()) {
      if (path.boundingRect()
              .adjusted(-kAdjust, -kAdjust, kAdjust, kAdjust)
              .contains(local_point)) {
        dragging_ = true;
        selected_shape_ = obj.get();
        last_point_ = ToCanvasCoordinates(event->pos()).toPoint();
        temp_stack_.push_back(svg_.Clone());
        is_performing_undo_redo_ = true;
        break;
      }
    } else if (path.boundingRect().contains(local_point)) {
      dragging_ = true;
      selected_shape_ = obj.get();
      last_point_ = ToCanvasCoordinates(event->pos()).toPoint();
      temp_stack_.push_back(svg_.Clone());
      is_performing_undo_redo_ = true;
      break;
    }
  }
}

}  // namespace micro_svg_editor
