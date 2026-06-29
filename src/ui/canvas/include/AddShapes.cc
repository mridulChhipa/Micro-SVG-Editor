#include "src/ui/canvas/Canvas.h"

void Canvas::AddShapeToCanvas(const std::string shape_type, QPointF location) {
  bool shape_added = false;
  SVG prev_state = svg_;  // Save the current state for undo
  if (shape_type == "Rectangle") {
    auto rect = std::make_shared<Rect>();
    rect->x = location.x();
    rect->y = location.y();
    rect->width = 100;
    rect->height = 100;
    svg_.objects.push_back(rect);
    shape_added = true;
  } else if (shape_type == "Circle") {
    auto circle = std::make_shared<Circle>();
    circle->x = location.x();
    circle->y = location.y();
    circle->r = 50;
    svg_.objects.push_back(circle);
    shape_added = true;
  } else if (shape_type == "Line") {
    auto line = std::make_shared<Line>();
    line->x1 = location.x();
    line->y1 = location.y();
    line->x2 = line->x1 + 100;
    line->y2 = line->y1 + 100;
    line->stroke = "blue";
    line->stroke_width = 5.0f;
    svg_.objects.push_back(line);
    shape_added = true;
  } else if (shape_type == "Polyline") {
    auto polyline = std::make_shared<Polyline>();
    polyline->points = {{350, 350}, {400, 400}, {450, 350}};
    svg_.objects.push_back(polyline);
    shape_added = true;
  } else if (shape_type == "Hexagon") {
    auto hexagon = std::make_shared<Hexagon>();
    hexagon->x = location.x();
    hexagon->y = location.y();
    hexagon->side_length = 50;

    for (int i = 0; i < 6; i++) {
      double angle = M_PI / 3.0 * i;
      double px = hexagon->x + hexagon->side_length * std::cos(angle);
      double py = hexagon->y + hexagon->side_length * std::sin(angle);
      hexagon->points.emplace_back(px, py);
    }
    svg_.objects.push_back(hexagon);
    shape_added = true;
  } else if (shape_type == "Text") {
    auto text = std::make_shared<Text>();
    text->x = location.x();
    text->y = location.y();
    text->content = "Hello, World!";
    svg_.objects.push_back(text);
    shape_added = true;
  }

  if (shape_added) {
    undo_stack_.push_back(prev_state);
    redo_stack_.clear();
  }
}
