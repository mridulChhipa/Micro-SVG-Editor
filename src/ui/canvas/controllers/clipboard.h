#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_CLIPBOARD_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_CLIPBOARD_H_

#include <memory>
#include <utility>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

// Holds a single deep-copied shape for cut/copy/paste.
class Clipboard {
 public:
  bool HasContent() const { return shape_ != nullptr; }

  void Set(std::unique_ptr<GraphicsObject> shape) { shape_ = std::move(shape); }

  // Returns a fresh deep copy of the held shape (nullptr if empty).
  std::unique_ptr<GraphicsObject> Clone() const {
    return shape_ ? shape_->Clone() : nullptr;
  }

 private:
  std::unique_ptr<GraphicsObject> shape_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_CLIPBOARD_H_
