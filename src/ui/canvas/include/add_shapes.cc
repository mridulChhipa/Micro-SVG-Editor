#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

void Canvas::AddShapeToCanvas(const std::string& shape_type, QPointF location) {
  bool shape_added = false;
  SVG prev_state = svg_.Clone();  // Save the current state for undo.
  if (shape_type == "Rectangle") {
    auto rect = std::make_unique<Rect>();
    rect->set_x(location.x());
    rect->set_y(location.y());
    rect->set_width(100);
    rect->set_height(100);
    svg_.objects().push_back(std::move(rect));
    shape_added = true;
  } else if (shape_type == "Circle") {
    auto circle = std::make_unique<Circle>();
    circle->set_x(location.x());
    circle->set_y(location.y());
    circle->set_r(50);
    svg_.objects().push_back(std::move(circle));
    shape_added = true;
  } else if (shape_type == "Line") {
    auto line = std::make_unique<Line>();
    line->set_x1(location.x());
    line->set_y1(location.y());
    line->set_x2(line->x1() + 100);
    line->set_y2(line->y1() + 100);
    line->set_stroke("blue");
    line->set_stroke_width(5.0f);
    svg_.objects().push_back(std::move(line));
    shape_added = true;
  } else if (shape_type == "Polyline") {
    auto polyline = std::make_unique<Polyline>();
    polyline->points() = {{350, 350}, {400, 400}, {450, 350}};
    svg_.objects().push_back(std::move(polyline));
    shape_added = true;
  } else if (shape_type == "Hexagon") {
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
    svg_.objects().push_back(std::move(hexagon));
    shape_added = true;
  } else if (shape_type == "Text") {
    auto text = std::make_unique<Text>();
    text->set_x(location.x());
    text->set_y(location.y());
    text->set_content("Hello, World!");
    svg_.objects().push_back(std::move(text));
    shape_added = true;
  }

  if (shape_added) history_.Push(std::move(prev_state));
}

}  // namespace micro_svg_editor
