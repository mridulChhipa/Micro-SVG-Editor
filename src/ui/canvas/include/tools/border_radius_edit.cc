#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::EditBorderRadius(bool& edited) {
  bool ok;
  int new_radius = QInputDialog::getInt(
      this, "Border Radius", "Enter border radius:", 0, 0, 100, 1, &ok);
  if (ok) {
    selected_shape_->SetBorderRadius(new_radius);
    edited = true;
  }

  curr_tool_ = "";
}

}  // namespace micro_svg_editor
