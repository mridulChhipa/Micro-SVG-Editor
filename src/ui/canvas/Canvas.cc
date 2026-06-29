#include "Canvas.h"

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

std::optional<ShapeVariant> Canvas::ToShapeVariant(
    const GraphicsObjectPtr& obj) {
  const std::string t = obj->Type();
  if (t == "rect") return std::dynamic_pointer_cast<Rect>(obj);
  if (t == "circle") return std::dynamic_pointer_cast<Circle>(obj);
  if (t == "line") return std::dynamic_pointer_cast<Line>(obj);
  if (t == "polyline") return std::dynamic_pointer_cast<Polyline>(obj);
  if (t == "hexagon") return std::dynamic_pointer_cast<Hexagon>(obj);
  if (t == "path") return std::dynamic_pointer_cast<Path>(obj);
  if (t == "text") return std::dynamic_pointer_cast<Text>(obj);
  return std::nullopt;
}

void Canvas::ClearCanvas() {
  svg_.Clear();
  update();
}

void Canvas::UpdateCanvasSize(int w, int h) {
  svg_.width = w;
  svg_.height = h;
  update();
}

void Canvas::UpdateCanvas(const SVG& new_svg)  // Used to update canvas when a
                                               // file is parsed and opened
{
  svg_ = new_svg;
  x_offset_ = (width() - svg_.width) / 2.0f;
  y_offset_ = (height() - svg_.height) / 2.0f;
  zoom_factor_ = 1.0;

  update();
}
