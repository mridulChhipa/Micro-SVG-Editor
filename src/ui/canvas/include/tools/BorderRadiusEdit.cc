#include "src/ui/canvas/Canvas.h"

void Canvas::EditBorderRadius(bool& edited) {
  bool ok;
  int new_radius = QInputDialog::getInt(
      this, "Border Radius", "Enter border radius:", 0, 0, 100, 1, &ok);
  if (ok) {
    if (selected_shape_->Type() == "rect") {
      auto rect = std::dynamic_pointer_cast<Rect>(*std::find(
          svg_.objects.begin(), svg_.objects.end(), selected_shape_));
      if (rect) {
        rect->rx = new_radius;
        rect->ry = new_radius;
        edited = true;
      }
    }
  }

  curr_tool_ = "";
}
