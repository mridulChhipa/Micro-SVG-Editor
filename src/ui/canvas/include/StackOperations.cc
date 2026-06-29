#include "src/ui/canvas/Canvas.h"

// Undo/redo stores the whole document and deep-copies it, so that later edits
// do not mutate the snapshots already held in the stacks.
void Canvas::Undo() {
  selected_shape_ = nullptr;  // Deselect any shape when undoing
  if (!undo_stack_.isEmpty()) {
    redo_stack_.push_back(svg_.Clone());
    svg_ = undo_stack_.back();
    undo_stack_.pop_back();
    update();
  }
}

void Canvas::Redo() {
  selected_shape_ = nullptr;
  if (!redo_stack_.isEmpty()) {
    undo_stack_.push_back(svg_.Clone());
    svg_ = redo_stack_.back();
    redo_stack_.pop_back();
    update();
  }
}

void Canvas::Cut() {
  if (selected_shape_ != nullptr) {
    undo_stack_.push_back(svg_.Clone());
    redo_stack_.clear();

    Copy();
    auto it =
        std::find(svg_.objects.begin(), svg_.objects.end(), selected_shape_);
    if (it != svg_.objects.end()) {
      svg_.objects.erase(it);
      selected_shape_ = nullptr;
    }
    update();
  }
}

void Canvas::Copy() {
  if (selected_shape_ != nullptr)
    clipboard_shape_ =
        selected_shape_->Clone();  // Deep copy of the selected shape
}