#include "src/ui/canvas/Canvas.h"

void Canvas::StrokeEdit(bool& edited) {
  bool w_enter;
  int new_width =
      QInputDialog::getInt(this, "Stroke Width",
                           "Enter stroke width:", selected_shape_->stroke_width,
                           1, 100, 1, &w_enter);

  if (w_enter) {
    bool col_enter;
    QString new_color = QInputDialog::getText(
        this, "Stroke Color",
        "Enter stroke color (e.g. 'black' or '#000000'):", QLineEdit::Normal,
        QString::fromStdString(selected_shape_->stroke), &col_enter);

    if (col_enter && !new_color.isEmpty()) {
      selected_shape_->stroke_width = new_width;
      selected_shape_->stroke = new_color.toStdString();
      edited = true;
    }
  }
  curr_tool_ = "";
}
