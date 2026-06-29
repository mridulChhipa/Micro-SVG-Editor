#include "src/ui/canvas/Canvas.h"

void Canvas::EditOpacity(bool& edited, float& prop, std::string title) {
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
