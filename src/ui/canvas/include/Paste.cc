#include "src/ui/canvas/Canvas.h"

void Canvas::Paste() {
  if (clipboard_shape_ != nullptr) {
    undo_stack_.push_back(svg_.Clone());
    redo_stack_.clear();

    // Paste object by slightly shifting them so that they don't overlap
    if (clipboard_shape_->Type() == "text") {
      auto text_shape =
          std::dynamic_pointer_cast<Text>(clipboard_shape_->Clone());
      text_shape->x += 50;
      text_shape->y += 50;
      svg_.Add(text_shape);
    } else if (clipboard_shape_->Type() == "path") {
      auto path_shape =
          std::dynamic_pointer_cast<Path>(clipboard_shape_->Clone());
      for (auto& [cmd, points] : path_shape->commands)
        for (auto& [x, y] : points) {
          x += 50;
          y += 50;
        }

      svg_.Add(path_shape);
    } else if (clipboard_shape_->Type() == "rect") {
      auto rect_shape =
          std::dynamic_pointer_cast<Rect>(clipboard_shape_->Clone());
      rect_shape->x += 50;
      rect_shape->y += 50;
      svg_.Add(rect_shape);
    } else if (clipboard_shape_->Type() == "circle") {
      auto circle_shape =
          std::dynamic_pointer_cast<Circle>(clipboard_shape_->Clone());
      circle_shape->x += 50;  // Offset the pasted shape
      circle_shape->y += 50;
      svg_.Add(circle_shape);
    } else if (clipboard_shape_->Type() == "line") {
      auto line_shape =
          std::dynamic_pointer_cast<Line>(clipboard_shape_->Clone());
      line_shape->x1 += 50;  // Offset the pasted shape
      line_shape->y1 += 50;
      line_shape->x2 += 50;
      line_shape->y2 += 50;
      svg_.Add(line_shape);
    } else if (clipboard_shape_->Type() == "polyline") {
      auto polyline_shape =
          std::dynamic_pointer_cast<Polyline>(clipboard_shape_->Clone());
      for (auto& [x, y] : polyline_shape->points) {
        x += 50;
        y += 50;
      }
      svg_.Add(polyline_shape);
    } else if (clipboard_shape_->Type() == "hexagon") {
      auto hexagon_shape =
          std::dynamic_pointer_cast<Hexagon>(clipboard_shape_->Clone());
      hexagon_shape->x += 50;
      hexagon_shape->y += 50;
      for (auto& [x, y] : hexagon_shape->points) {
        x += 50;
        y += 50;
      }
      svg_.Add(hexagon_shape);
    }

    selected_shape_ = nullptr;
    update();
  }
}
