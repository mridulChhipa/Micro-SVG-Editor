#ifndef TOOLS_HPP
#define TOOLS_HPP

inline void Canvas::setCurrentTool(const QString &toolName)
{
    currentTool = toolName;
    qDebug() << "Current tool set to:" << currentTool;

    if (currentTool == "Delete")
    {
        if (selected_shape)
        {
            auto it = std::find(svg.objects.begin(), svg.objects.end(), selected_shape);
            if (it != svg.objects.end())
            {
                svg.objects.erase(it);
                selected_shape = nullptr;
            }
        }
        currentTool = "";
        update();
    }
    if (currentTool == "Text Edit")
    {
        if (selected_shape && selected_shape->type() == "text")
        {
            auto text_obj = std::dynamic_pointer_cast<Text>(*std::find(svg.objects.begin(), svg.objects.end(), selected_shape));
            if (text_obj)
            {
                bool ok;
                QString newText = QInputDialog::getText(this, "Edit Text", "Enter new text:", QLineEdit::Normal, QString::fromStdString(text_obj->content), &ok);
                if (ok && !newText.isEmpty())
                {
                    text_obj->content = newText.toStdString();
                    update();
                }
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Fill Color")
    {
        if (selected_shape)
        {
            bool ok;
            QString newColor = QInputDialog::getText(this, "Fill Color", "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal, QString::fromStdString(selected_shape->fill), &ok);
            if (ok && !newColor.isEmpty())
            {
                selected_shape->fill = newColor.toStdString();
                update();
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Stroke")
    {
        if (selected_shape)
        {
            bool okWidth;
            int newWidth = QInputDialog::getInt(this, "Stroke Width",
                                                "Enter stroke width:",
                                                1, 1, 100, 1, &okWidth);

            if (okWidth)
            {
                bool okColor;
                QString newColor = QInputDialog::getText(this, "Stroke Color",
                                                         "Enter stroke color (e.g. 'black' or '#000000'):",
                                                         QLineEdit::Normal,
                                                         QString::fromStdString(selected_shape->stroke),
                                                         &okColor);

                if (okColor && !newColor.isEmpty())
                {
                    selected_shape->stroke_width = newWidth;
                    selected_shape->stroke = newColor.toStdString();

                    update();
                }
            }
        }
        currentTool = "";
    }
}

#endif