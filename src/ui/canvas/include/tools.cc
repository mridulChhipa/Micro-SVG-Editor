#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::DeleteSelectedShape(bool& edited) {
  if (selected_shape_ != nullptr) {
    auto it = std::find_if(svg_.objects().begin(), svg_.objects().end(),
                           [&](const GraphicsObjectPtr& obj) {
                             return obj.get() == selected_shape_;
                           });
    if (it != svg_.objects().end()) {
      svg_.objects().erase(it);
      selected_shape_ = nullptr;
      edited = true;
    }
  }
  curr_tool_ = "";
}

void Canvas::EditFillColor(bool& edited) {
  bool ok;
  QString new_color = QInputDialog::getText(
      this, "Fill Color",
      "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal,
      QString::fromStdString(selected_shape_->fill()), &ok);
  if (ok && !new_color.isEmpty()) {
    selected_shape_->set_fill(new_color.toStdString());
    edited = true;
  }
  curr_tool_ = "";
}

void Canvas::SetCurrentTool(const QString& tool_name) {
  curr_tool_ = tool_name;
  qDebug() << "Current tool set to:" << curr_tool_;

  bool edited = false;
  SVG current_state = svg_.Clone();

  if (curr_tool_ == "Delete") {
    DeleteSelectedShape(edited);
  } else if (curr_tool_ == "Fill Color" && selected_shape_) {
    EditFillColor(edited);
  } else if (curr_tool_ == "Stroke" && selected_shape_) {
    StrokeEdit(edited);
  } else if (curr_tool_ == "Border Radius" && selected_shape_ != nullptr &&
             selected_shape_->CanSetBorderRadius()) {
    EditBorderRadius(edited);
  } else if (curr_tool_ == "Canvas Dimensions") {
    CanvasEdit(edited);
  } else if (curr_tool_ == "Fill Opacity" && selected_shape_) {
    float value = selected_shape_->fill_opacity();
    EditOpacity("Fill Opacity", value, edited);
    selected_shape_->set_fill_opacity(value);
  } else if (curr_tool_ == "Stroke Opacity" && selected_shape_) {
    float value = selected_shape_->stroke_opacity();
    EditOpacity("Stroke Opacity", value, edited);
    selected_shape_->set_stroke_opacity(value);
  } else if (curr_tool_ == "Opacity" && selected_shape_) {
    float value = selected_shape_->opacity();
    EditOpacity("Opacity", value, edited);
    selected_shape_->set_opacity(value);
  }
  if (curr_tool_ == "Text Edit" && selected_shape_)
    TextEdit(edited);
  else if (curr_tool_ == "Select")
    selected_shape_ = nullptr;

  update();
  if (edited) {
    undo_stack_.push_back(std::move(current_state));
    redo_stack_.clear();
  }
}

}  // namespace micro_svg_editor
