#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

Canvas::Canvas(QWidget* parent) : QWidget(parent) {}

QPointF Canvas::ToCanvasCoordinates(QPointF point) {
  return viewport_.ToCanvas(point);
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
  viewport_.Reset();
  viewport_.Center(width(), height(), svg_.width(), svg_.height());

  update();
}

}  // namespace micro_svg_editor
