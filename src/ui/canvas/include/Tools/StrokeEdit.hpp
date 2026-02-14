#ifndef STROKE_EDIT_HPP
#define STROKE_EDIT_HPP

#include <memory>
#include <QWidget>

inline void strokeEdit(Canvas &canvas, QWidget *parent, bool &edited)
{
    bool okWidth;
    int newWidth = QInputDialog::getInt(parent, "Stroke Width",
                                        "Enter stroke width:",
                                        canvas.selected_shape->stroke_width, 1, 100, 1, &okWidth);

    if (okWidth)
    {
        bool okColor;
        QString newColor = QInputDialog::getText(parent, "Stroke Color",
                                                 "Enter stroke color (e.g. 'black' or '#000000'):",
                                                 QLineEdit::Normal,
                                                 QString::fromStdString(canvas.selected_shape->stroke),
                                                 &okColor);

        if (okColor && !newColor.isEmpty())
        {
            canvas.selected_shape->stroke_width = newWidth;
            canvas.selected_shape->stroke = newColor.toStdString();
            edited = true;
        }
    }
    canvas.currentTool = "";
}

#endif