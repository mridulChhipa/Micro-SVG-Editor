#include "src/ui/canvas/Canvas.h"

void Canvas::strokeEdit(bool &edited)
{
  bool w_enter;
  int newWidth = QInputDialog::getInt(this, "Stroke Width",
                                      "Enter stroke width:",
                                      selected_shape->stroke_width, 1, 100, 1, &w_enter);

  if (w_enter)
  {
    bool col_enter;
    QString newColor = QInputDialog::getText(this, "Stroke Color",
                                             "Enter stroke color (e.g. 'black' or '#000000'):",
                                             QLineEdit::Normal,
                                             QString::fromStdString(selected_shape->stroke),
                                             &col_enter);

    if (col_enter && !newColor.isEmpty())
    {
      selected_shape->stroke_width = newWidth;
      selected_shape->stroke = newColor.toStdString();
      edited = true;
    }
  }
  curr_tool = "";
}
