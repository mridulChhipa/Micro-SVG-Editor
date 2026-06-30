#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::EditOpacity(const std::string& title, float& prop, bool& edited) {
  bool ok = false;
  double new_opacity = QInputDialog::getDouble(
      this, QString::fromStdString(title),
      QString::fromStdString("Enter (decimal fraction)"), prop, 0, 1, 1, &ok,
      Qt::WindowFlags(), 0.01);

  if (ok) {
    prop = new_opacity;
    edited = true;
  }
}

}  // namespace micro_svg_editor
