#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "tools/TextEdit.hpp"
#include "tools/CanvasEdit.hpp"
#include "tools/StrokeEdit.hpp"
#include "tools/BorderRadiusEdit.hpp"
#include "tools/EditOpacity.hpp"

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
    }
    else if (currentTool == "Fill Color" && selected_shape)
    {
        bool ok;
        QString newColor = QInputDialog::getText(this, "Fill Color", "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal, QString::fromStdString(selected_shape->fill), &ok);
        if (ok && !newColor.isEmpty())
        {
            selected_shape->fill = newColor.toStdString();
            edited = true;
        }
        currentTool = "";
    }
    else if (currentTool == "Stroke" && selected_shape)
        strokeEdit(*this, this, edited);
    else if (currentTool == "Border Radius" && selected_shape != nullptr && selected_shape->type() == "rect")
        editBorderRadius(*this, this, edited);
    else if (currentTool == "Canvas Dimensions")
        canvasEdit(*this, this, edited);
    else if (currentTool == "Fill Opacity" && selected_shape)
        editOpacity(this, edited, selected_shape->fill_opacity, "Fill Opacity");
    else if (currentTool == "Stroke Opacity" && selected_shape)
        editOpacity(this, edited, selected_shape->stroke_opacity, "Stroke Opacity");
    else if (currentTool == "Opacity" && selected_shape)
        editOpacity(this, edited, selected_shape->opacity, "Opacity");
    if (currentTool == "Text Edit" && selected_shape && selected_shape->type() == "text")
        textEdit(*this, this, edited);
    else if (currentTool == "Select")
        selected_shape = nullptr;

    update();
    if (edited)
    {
        undoStack.push_back(currentState);
        redoStack.clear();
    }
}

#endif