#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::StrokeEdit(bool& edited) {
  bool w_enter;
  int new_width = QInputDialog::getInt(
      this, "Stroke Width",
      "Enter stroke width:", selected_shape_->stroke_width(), 1, 100, 1,
      &w_enter);

  if (w_enter) {
    bool col_enter;
    QString new_color = QInputDialog::getText(
        this, "Stroke Color",
        "Enter stroke color (e.g. 'black' or '#000000'):", QLineEdit::Normal,
        QString::fromStdString(selected_shape_->stroke()), &col_enter);

    if (col_enter && !new_color.isEmpty()) {
      selected_shape_->set_stroke_width(new_width);
      selected_shape_->set_stroke(new_color.toStdString());
      edited = true;
    }
  }
  curr_tool_ = "";
}

}  // namespace micro_svg_editor
