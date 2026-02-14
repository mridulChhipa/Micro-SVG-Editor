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
    if (currentTool == "Text Edit" && selected_shape && selected_shape->type() == "text")
    {
        auto text_obj = std::dynamic_pointer_cast<Text>(*std::find(svg.objects.begin(), svg.objects.end(), selected_shape));
        if (text_obj)
        {
            bool ok[3] = {false, false, false};
            QString newText = QInputDialog::getText(this, "Edit Text", "Enter new text:", QLineEdit::Normal, QString::fromStdString(text_obj->content), &ok[0]);
            int newFontSize = QInputDialog::getInt(this, "Edit Text", "Enter new Font Size:", text_obj->font_size, 1, 100, 1, &ok[1]);
            QString newFontFamily = QInputDialog::getText(this, "Edit Text", "Enter new Font Family:", QLineEdit::Normal, QString::fromStdString(text_obj->font_family), &ok[2]);
            if (ok[0] && ok[1] && ok[2] && !newText.isEmpty())
            {
                text_obj->content = newText.toStdString();
                text_obj->font_size = newFontSize;
                text_obj->font_family = newFontFamily.toStdString();
                edited = true;
                update();
            }
        }
        currentTool = "";
    }
    else if (currentTool == "Fill Color" && selected_shape)
    {
        bool ok;
        QString newColor = QInputDialog::getText(this, "Fill Color", "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal, QString::fromStdString(selected_shape->fill), &ok);
        if (ok && !newColor.isEmpty())
        {
            selected_shape->fill = newColor.toStdString();
            edited = true;
            update();
        }
        currentTool = "";
    }
    else if (currentTool == "Stroke" && selected_shape)
    {
        bool okWidth;
        int newWidth = QInputDialog::getInt(this, "Stroke Width",
                                            "Enter stroke width:",
                                            selected_shape->stroke_width, 1, 100, 1, &okWidth);

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
        currentTool = "";
    }
    else if (currentTool == "Border Radius" && selected_shape != nullptr && selected_shape->type() == "rect")
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
    }
    else if (currentTool == "Select")
    {
        selected_shape = nullptr;
        update();
    }
    else if (currentTool == "Fill Opacity" && selected_shape)
    {
        bool ok = false;
        double newFillOpacity = QInputDialog::getDouble(this, "Fill Opacity", "Enter fill opacity (decimal fraction)", selected_shape->fill_opacity, 0, 1, 1, &ok, Qt::WindowFlags(), 0.01);

        if (ok)
        {
            selected_shape->fill_opacity = newFillOpacity;
            edited = true;
            update();
        }
    }
    else if (currentTool == "Stroke Opacity" && selected_shape)
    {
        bool ok = false;
        double newFillOpacity = QInputDialog::getDouble(this, "Stroke Opacity", "Enter Stroke opacity (decimal fraction)", selected_shape->stroke_opacity, 0, 1, 1, &ok, Qt::WindowFlags(), 0.01);

        if (ok)
        {
            selected_shape->stroke_opacity = newFillOpacity;
            edited = true;
            update();
        }
    }
    else if (currentTool == "Opacity" && selected_shape)
    {
        bool ok = false;
        double newFillOpacity = QInputDialog::getDouble(this, "Opacity", "Enter opacity (decimal fraction)", selected_shape->opacity, 0, 1, 1, &ok, Qt::WindowFlags(), 0.01);

        if (ok)
        {
            selected_shape->opacity = newFillOpacity;
            edited = true;
            update();
        }
    }
    else if (currentTool == "Transform" && selected_shape)
    {
        bool ok = false;
        QString rotation = QInputDialog::getText(this, "Transform", "Enter the transform as string of xml", QLineEdit::Normal, QString::fromStdString(selected_shape->transform), &ok);

        if (ok)
        {
            selected_shape->transform = rotation.toStdString();
            edited = true;

            update();
        }
    }

    if (edited)
    {
        undoStack.push_back(currentState);
        redoStack.clear();
    }
}

#endif