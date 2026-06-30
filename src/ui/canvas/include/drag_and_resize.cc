#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::ApplyDrag(const QPoint& delta) {
  if (selected_shape_) selected_shape_->Translate(delta.x(), delta.y());
}

void Canvas::ApplyResize(const QPoint& delta, HandleType handle) {
  if (!selected_shape_) return;

  bool affects_left =
      (handle == HandleType::kTopLeft || handle == HandleType::kMiddleLeft ||
       handle == HandleType::kBottomLeft);
  bool affects_right =
      (handle == HandleType::kTopRight || handle == HandleType::kMiddleRight ||
       handle == HandleType::kBottomRight);
  bool affects_top =
      (handle == HandleType::kTopLeft || handle == HandleType::kTopCenter ||
       handle == HandleType::kTopRight);
  bool affects_bottom = (handle == HandleType::kBottomLeft ||
                         handle == HandleType::kBottomCenter ||
                         handle == HandleType::kBottomRight);

  selected_shape_->Resize(delta.x(), delta.y(), affects_left, affects_right,
                          affects_top, affects_bottom);
}

}  // namespace micro_svg_editor
