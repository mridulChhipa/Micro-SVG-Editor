#ifndef TEXT_EDIT_HPP
#define TEXT_EDIT_HPP

#include <memory>
#include <QWidget>

inline void textEdit(Canvas &canvas, QWidget *parent, bool &edited)
{
    auto text_obj = std::dynamic_pointer_cast<Text>(*std::find(canvas.svg.objects.begin(), canvas.svg.objects.end(), canvas.selected_shape));
    if (text_obj)
    {
        bool ok[3] = {false, false, false};
        QString newText = QInputDialog::getText(parent, "Edit Text", "Enter new text:", QLineEdit::Normal, QString::fromStdString(text_obj->content), &ok[0]);
        int newFontSize = QInputDialog::getInt(parent, "Edit Text", "Enter new Font Size:", text_obj->font_size, 1, 100, 1, &ok[1]);
        QString newFontFamily = QInputDialog::getText(parent, "Edit Text", "Enter new Font Family:", QLineEdit::Normal, QString::fromStdString(text_obj->font_family), &ok[2]);
        if (ok[0] && ok[1] && ok[2] && !newText.isEmpty())
        {
            text_obj->content = newText.toStdString();
            text_obj->font_size = newFontSize;
            text_obj->font_family = newFontFamily.toStdString();
            edited = true;
        }
    }

    canvas.currentTool = "";
}

#endif