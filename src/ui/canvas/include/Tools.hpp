#ifndef TOOLS_HPP
#define TOOLS_HPP

inline void Canvas::setCurrentTool(const QString &toolName)
{
    currentTool = toolName;
    qDebug() << "Current tool set to:" << currentTool;

    bool edited = false;
    SVG currentState = svg.clone();

    if (currentTool == "Delete")
    {
        if (selected_shape != nullptr)
        {
            auto it = std::find(svg.objects.begin(), svg.objects.end(), selected_shape);
            if (it != svg.objects.end())
            {
                svg.objects.erase(it);
                selected_shape = nullptr;
                edited = true;
            }
        }
        currentTool = "";
        update();
    }
    if (currentTool == "Text Edit")
    {
        if (selected_shape != nullptr && selected_shape->type() == "text")
        {
            auto text_obj = std::dynamic_pointer_cast<Text>(*std::find(svg.objects.begin(), svg.objects.end(), selected_shape));
            if (text_obj)
            {
                bool ok;
                QString newText = QInputDialog::getText(this, "Edit Text", "Enter new text:", QLineEdit::Normal, QString::fromStdString(text_obj->content), &ok);
                if (ok && !newText.isEmpty())
                {
                    text_obj->content = newText.toStdString();
                    edited = true;
                    update();
                }
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Fill Color")
    {
        if (selected_shape != nullptr)
        {
            bool ok;
            QString newColor = QInputDialog::getText(this, "Fill Color", "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal, QString::fromStdString(selected_shape->fill), &ok);
            if (ok && !newColor.isEmpty())
            {
                selected_shape->fill = newColor.toStdString();
                edited = true;
                update();
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Stroke")
    {
        if (selected_shape != nullptr)
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
                    edited = true;
                    update();
                }
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Border Radius")
    {
        if (selected_shape != nullptr && (selected_shape->type() == "rect" || selected_shape->type() == "ellipse"))
        {
            bool ok;
            int newRadius = QInputDialog::getInt(this, "Border Radius",
                                                 "Enter border radius:",
                                                 0, 0, 100, 1, &ok);
            if (ok)
            {
                if (selected_shape->type() == "rect")
                {
                    auto rect_obj = std::dynamic_pointer_cast<Rect>(*std::find(svg.objects.begin(), svg.objects.end(), selected_shape));
                    if (rect_obj)
                    {
                        rect_obj->rx = newRadius;
                        rect_obj->ry = newRadius;
                        edited = true;
                        update();
                    }
                }
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Canvas Dimensions")
    {
        bool okWidth, okHeight;
        int newWidth = QInputDialog::getInt(this, "Canvas Width",
                                            "Enter canvas width:",
                                            static_cast<int>(svg.width), 1, 5000, 1, &okWidth);
        int newHeight = QInputDialog::getInt(this, "Canvas Height",
                                             "Enter canvas height:",
                                             static_cast<int>(svg.height), 1, 5000, 1, &okHeight);
        if (okWidth && okHeight)
        {
            svg.width = newWidth;
            svg.height = newHeight;
            edited = true;
            update();
        }

        currentTool = "";
    } else if (currentTool == "Select")
    {
        // No specific action needed for select tool here
        selected_shape = nullptr; // Deselect any currently selected shape
        update();
    }

    if (edited)
    {
        undoStack.push_back(currentState);
        redoStack.clear();
    }
}

#endif