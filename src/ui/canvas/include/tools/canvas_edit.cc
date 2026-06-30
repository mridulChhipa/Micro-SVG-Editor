#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::CanvasEdit(bool& edited) {
  bool w_enter, ok_height;
  int new_width = QInputDialog::getInt(
      this, "Canvas Width",
      "Enter canvas width:", static_cast<int>(svg_.width()), 1, 5000, 1,
      &w_enter);
  int new_height = QInputDialog::getInt(
      this, "Canvas Height",
      "Enter canvas height:", static_cast<int>(svg_.height()), 1, 5000, 1,
      &ok_height);
  if (w_enter && ok_height) {
    svg_.set_width(new_width);
    svg_.set_height(new_height);
    edited = true;
  }

  curr_tool_ = "";
}

}  // namespace micro_svg_editor
