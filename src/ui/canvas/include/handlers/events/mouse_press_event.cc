#include "src/ui/canvas/canvas.h"
#include "src/ui/canvas/selection.h"

namespace micro_svg_editor {

void Canvas::mousePressEvent(QMouseEvent* event) {
  QPointF canvas_pnt = ToCanvasCoordinates(event->pos());
  if (history_.gesture_active()) return;

  if (curr_tool_ != "" && curr_tool_ != "Freehand") {
    AddShapeToCanvas(curr_tool_.toStdString(), canvas_pnt);
    update();
  }

  if (event->button() == Qt::LeftButton && curr_tool_ == "Freehand") {
    current_path_ = QPainterPath();
    current_path_.moveTo(canvas_pnt);
    is_drawing_ = true;
    return;
  }

  // A click on a resize handle of the current selection starts a resize.
  if (selected_shape_) {
    HandleType handle = HitHandle(*selected_shape_, canvas_pnt);
    if (handle != HandleType::kNone) {
      is_resizing_ = true;
      history_.BeginGesture(svg_);  // Save current state for a potential undo.
      curr_handle_ = handle;
      last_point_ = canvas_pnt.toPoint();
      dragging_ = false;
      return;
    }
  }

  // Otherwise pick the topmost shape under the cursor and start a drag.
  selected_shape_ = nullptr;
  dragging_ = false;
  is_resizing_ = false;
  curr_handle_ = HandleType::kNone;

  GraphicsObject* hit = PickTopmost(svg_, canvas_pnt);
  if (hit) {
    dragging_ = true;
    selected_shape_ = hit;
    last_point_ = canvas_pnt.toPoint();
    history_.BeginGesture(svg_);
  }
}

}  // namespace micro_svg_editor
