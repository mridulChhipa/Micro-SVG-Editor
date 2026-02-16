#include "src/ui/canvas/Canvas.h"

void Canvas::textEdit(bool &edited)
{
  auto text_obj = std::dynamic_pointer_cast<Text>(*std::find(svg.objects.begin(), svg.objects.end(), selected_shape));
  if (text_obj)
  {
    bool ok[3] = {false, false, false};
    QString new_text = QInputDialog::getText(this, "Edit Text", "Enter new text:", QLineEdit::Normal, QString::fromStdString(text_obj->content), &ok[0]);
    int new_font_size = QInputDialog::getInt(this, "Edit Text", "Enter new Font Size:", text_obj->font_size, 1, 100, 1, &ok[1]);
    QString new_font_fam = QInputDialog::getText(this, "Edit Text", "Enter new Font Family:", QLineEdit::Normal, QString::fromStdString(text_obj->font_family), &ok[2]);
    if (ok[0] && ok[1] && ok[2] && !new_text.isEmpty())
    {
      text_obj->content = new_text.toStdString();
      text_obj->font_size = new_font_size;
      text_obj->font_family = new_font_fam.toStdString();
      edited = true;
    }
  }

  curr_tool = "";
}
