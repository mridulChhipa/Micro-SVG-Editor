#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_HISTORY_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_HISTORY_H_

#include <utility>
#include <vector>

#include "src/model/svg.h"

namespace micro_svg_editor {

// Snapshot-based undo/redo for the document, with drag/resize gesture
// coalescing. Holds whole-SVG snapshots; the live document lives in Canvas.
class History {
 public:
  bool CanUndo() const { return !undo_.empty(); }
  bool CanRedo() const { return !redo_.empty(); }

  // Records a pre-mutation snapshot as an undo point; clears the redo stack.
  void Push(SVG snapshot) {
    undo_.push_back(std::move(snapshot));
    redo_.clear();
  }

  // Stashes `current` onto the redo stack and returns the previous undo state
  // for the caller to install as the live document.
  SVG Undo(const SVG& current) {
    redo_.push_back(current.Clone());
    SVG previous = std::move(undo_.back());
    undo_.pop_back();
    return previous;
  }

  SVG Redo(const SVG& current) {
    undo_.push_back(current.Clone());
    SVG next = std::move(redo_.back());
    redo_.pop_back();
    return next;
  }

  // Begins a drag/resize gesture by snapshotting the current state; the gesture
  // becomes a single undo entry when committed.
  void BeginGesture(const SVG& current) {
    temp_.push_back(current.Clone());
    gesture_active_ = true;
  }
  bool gesture_active() const { return gesture_active_; }
  void CommitGesture() {
    if (!temp_.empty()) {
      undo_.push_back(std::move(temp_.back()));
      temp_.clear();
    }
    redo_.clear();
    gesture_active_ = false;
  }

 private:
  std::vector<SVG> undo_;
  std::vector<SVG> redo_;
  std::vector<SVG> temp_;
  bool gesture_active_ = false;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_HISTORY_H_
