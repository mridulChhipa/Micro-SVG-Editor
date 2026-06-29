#include "src/ui/canvas/Canvas.h"

void Canvas::SetCurrentTool(const QString& tool_name) {
  curr_tool_ = tool_name;
  qDebug() << "Current tool set to:" << curr_tool_;

  bool edited = false;
  SVG current_state = svg_.Clone();

  if (curr_tool_ == "Delete") {
    if (selected_shape_ != nullptr) {
      auto it =
          std::find(svg_.objects.begin(), svg_.objects.end(), selected_shape_);
      if (it != svg_.objects.end()) {
        svg_.objects.erase(it);
        selected_shape_ = nullptr;
        edited = true;
      }
    }
    curr_tool_ = "";
  } else if (curr_tool_ == "Fill Color" && selected_shape_) {
    bool ok;
    QString new_color = QInputDialog::getText(
        this, "Fill Color",
        "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal,
        QString::fromStdString(selected_shape_->fill), &ok);
    if (ok && !new_color.isEmpty()) {
      selected_shape_->fill = new_color.toStdString();
      edited = true;
    }
    curr_tool_ = "";
  } else if (curr_tool_ == "Stroke" && selected_shape_)
    StrokeEdit(edited);
  else if (curr_tool_ == "Border Radius" && selected_shape_ != nullptr &&
           selected_shape_->Type() == "rect")
    EditBorderRadius(edited);
  else if (curr_tool_ == "Canvas Dimensions")
    CanvasEdit(edited);
  else if (curr_tool_ == "Fill Opacity" && selected_shape_)
    EditOpacity(edited, selected_shape_->fill_opacity, "Fill Opacity");
  else if (curr_tool_ == "Stroke Opacity" && selected_shape_)
    EditOpacity(edited, selected_shape_->stroke_opacity, "Stroke Opacity");
  else if (curr_tool_ == "Opacity" && selected_shape_)
    EditOpacity(edited, selected_shape_->opacity, "Opacity");
  if (curr_tool_ == "Text Edit" && selected_shape_ &&
      selected_shape_->Type() == "text")
    TextEdit(edited);
  else if (curr_tool_ == "Select")
    selected_shape_ = nullptr;

  update();
  if (edited) {
    undo_stack_.push_back(current_state);
    redo_stack_.clear();
  }
}