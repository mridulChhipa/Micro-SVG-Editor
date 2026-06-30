#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::Paste() {
  if (clipboard_shape_ == nullptr) return;

  undo_stack_.push_back(svg_.Clone());
  redo_stack_.clear();

  // Paste a deep copy, slightly shifted so it does not overlap the original.
  std::unique_ptr<GraphicsObject> copy = clipboard_shape_->Clone();
  copy->Translate(50, 50);
  svg_.Add(std::move(copy));

  selected_shape_ = nullptr;
  update();
}

}  // namespace micro_svg_editor
