#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

Canvas::Canvas(QWidget* parent) : QWidget(parent), dragging_(false) {
  undo_stack_.clear();
  redo_stack_.clear();
}

QPointF Canvas::ToCanvasCoordinates(QPointF point) {
  point.setX(point.x() / zoom_factor_ - x_offset_);
  point.setY(point.y() / zoom_factor_ - y_offset_);
  return point;
}

std::string Canvas::CurrentCanvasToSvg() { return svg_.ToSvg(); }

void Canvas::ClearCanvas() {
  svg_.Clear();
  update();
}

void Canvas::UpdateCanvasSize(int w, int h) {
  svg_.set_width(w);
  svg_.set_height(h);
  update();
}

// Used to update the canvas when a file is parsed and opened. Takes the
// document by value because SVG is move-only.
void Canvas::UpdateCanvas(SVG new_svg) {
  svg_ = std::move(new_svg);
  x_offset_ = (width() - svg_.width()) / 2.0f;
  y_offset_ = (height() - svg_.height()) / 2.0f;
  zoom_factor_ = 1.0;

  update();
}

}  // namespace micro_svg_editor
