#include "src/ui/canvas/canvas.h"

#include <cmath>
#include <memory>
#include <unordered_map>
#include <utility>

#include <QPointF>

namespace micro_svg_editor {
namespace {

std::unique_ptr<GraphicsObject> MakeRect(QPointF location) {
  auto rect = std::make_unique<Rect>();
  rect->set_x(location.x());
  rect->set_y(location.y());
  rect->set_width(100);
  rect->set_height(100);
  return rect;
}

std::unique_ptr<GraphicsObject> MakeCircle(QPointF location) {
  auto circle = std::make_unique<Circle>();
  circle->set_x(location.x());
  circle->set_y(location.y());
  circle->set_r(50);
  return circle;
}

std::unique_ptr<GraphicsObject> MakeLine(QPointF location) {
  auto line = std::make_unique<Line>();
  line->set_x1(location.x());
  line->set_y1(location.y());
  line->set_x2(line->x1() + 100);
  line->set_y2(line->y1() + 100);
  line->set_stroke("blue");
  line->set_stroke_width(5.0f);
  return line;
}

std::unique_ptr<GraphicsObject> MakePolyline(QPointF /*location*/) {
  auto polyline = std::make_unique<Polyline>();
  polyline->points() = {{350, 350}, {400, 400}, {450, 350}};
  return polyline;
}

std::unique_ptr<GraphicsObject> MakeHexagon(QPointF location) {
  auto hexagon = std::make_unique<Hexagon>();
  hexagon->set_x(location.x());
  hexagon->set_y(location.y());
  hexagon->set_side_length(50);
  for (int i = 0; i < 6; i++) {
    double angle = M_PI / 3.0 * i;
    double px = hexagon->x() + hexagon->side_length() * std::cos(angle);
    double py = hexagon->y() + hexagon->side_length() * std::sin(angle);
    hexagon->points().emplace_back(px, py);
  }
  return hexagon;
}

std::unique_ptr<GraphicsObject> MakeText(QPointF location) {
  auto text = std::make_unique<Text>();
  text->set_x(location.x());
  text->set_y(location.y());
  text->set_content("Hello, World!");
  return text;
}

using DefaultCreator = std::unique_ptr<GraphicsObject> (*)(QPointF);

const std::unordered_map<std::string, DefaultCreator>& DefaultCreators() {
  static const std::unordered_map<std::string, DefaultCreator> creators = {
      {"Rectangle", &MakeRect},  {"Circle", &MakeCircle},
      {"Line", &MakeLine},       {"Polyline", &MakePolyline},
      {"Hexagon", &MakeHexagon}, {"Text", &MakeText}};
  return creators;
}

}  // namespace

void Canvas::AddShapeToCanvas(const std::string& shape_type, QPointF location) {
  auto it = DefaultCreators().find(shape_type);
  if (it == DefaultCreators().end()) return;

  SVG prev_state = svg_.Clone();  // Save the current state for undo.
  svg_.objects().push_back(it->second(location));
  history_.Push(std::move(prev_state));
}

}  // namespace micro_svg_editor
