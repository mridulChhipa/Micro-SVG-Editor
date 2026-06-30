#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::Undo() {
  selected_shape_ = nullptr;  // Deselect any shape when undoing.
  if (history_.CanUndo()) {
    svg_ = history_.Undo(svg_);
    update();
  }
}

void Canvas::Redo() {
  selected_shape_ = nullptr;
  if (history_.CanRedo()) {
    svg_ = history_.Redo(svg_);
    update();
  }
}

void Canvas::Cut() {
  if (selected_shape_ != nullptr) {
    history_.Push(svg_.Clone());

    Copy();
    auto it = std::find_if(svg_.objects().begin(), svg_.objects().end(),
                           [&](const GraphicsObjectPtr& obj) {
                             return obj.get() == selected_shape_;
                           });
    if (it != svg_.objects().end()) {
      svg_.objects().erase(it);
      selected_shape_ = nullptr;
    }
    update();
  }
}

void Canvas::Copy() {
  if (selected_shape_ != nullptr)
    clipboard_.Set(selected_shape_->Clone());  // Deep copy of the selection.
}

}  // namespace micro_svg_editor
