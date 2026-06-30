#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::Paste() {
  if (!clipboard_.HasContent()) return;

  history_.Push(svg_.Clone());

  // Paste a deep copy, slightly shifted so it does not overlap the original.
  std::unique_ptr<GraphicsObject> copy = clipboard_.Clone();
  copy->Translate(50, 50);
  svg_.Add(std::move(copy));

  selected_shape_ = nullptr;
  update();
}

}  // namespace micro_svg_editor
