#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "src/ui/canvas/include/tools/TextEdit.h"
#include "src/ui/canvas/include/tools/CanvasEdit.h"
#include "src/ui/canvas/include/tools/StrokeEdit.h"
#include "src/ui/canvas/include/tools/BorderRadiusEdit.h"
#include "src/ui/canvas/include/tools/EditOpacity.h"

inline void Canvas::setcurr_tool(const QString &tool_name)
{
  curr_tool = tool_name;
  qDebug() << "Current tool set to:" << curr_tool;

  bool edited = false;
  SVG currentState = svg.clone();

  if (curr_tool == "Delete")
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
    curr_tool = "";
  }
  else if (curr_tool == "Fill Color" && selected_shape)
  {
    bool ok;
    QString newColor = QInputDialog::getText(this, "Fill Color", "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal, QString::fromStdString(selected_shape->fill), &ok);
    if (ok && !newColor.isEmpty())
    {
      selected_shape->fill = newColor.toStdString();
      edited = true;
    }
    curr_tool = "";
  }
  else if (curr_tool == "Stroke" && selected_shape)
    strokeEdit(*this, this, edited);
  else if (curr_tool == "Border Radius" && selected_shape != nullptr && selected_shape->type() == "rect")
    editBorderRadius(*this, this, edited);
  else if (curr_tool == "Canvas Dimensions")
    canvasEdit(*this, this, edited);
  else if (curr_tool == "Fill Opacity" && selected_shape)
    editOpacity(this, edited, selected_shape->fill_opacity, "Fill Opacity");
  else if (curr_tool == "Stroke Opacity" && selected_shape)
    editOpacity(this, edited, selected_shape->stroke_opacity, "Stroke Opacity");
  else if (curr_tool == "Opacity" && selected_shape)
    editOpacity(this, edited, selected_shape->opacity, "Opacity");
  if (curr_tool == "Text Edit" && selected_shape && selected_shape->type() == "text")
    textEdit(*this, this, edited);
  else if (curr_tool == "Select")
    selected_shape = nullptr;

  update();
  if (edited)
  {
    undo_stack.push_back(currentState);
    redo_stack.clear();
  }
}

#endif