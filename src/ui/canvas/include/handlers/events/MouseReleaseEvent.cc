#include "src/ui/canvas/Canvas.h"

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
  Q_UNUSED(event);  // Prevent warnings because position is not being used but
                    // is required as a parameter
  // Any kind of event is stopped when we release mouse
  dragging_ = false;
  is_resizing_ = false;
  curr_handle_ = HandleType::kNone;

  if (event->button() == Qt::LeftButton && is_drawing_ &&
      curr_tool_ == "Freehand") {
    // Function to draw a freehand sketch
    SVG prev_svg = svg_.Clone();

    is_drawing_ = false;
    std::shared_ptr<Path> new_path = std::make_shared<Path>();
    for (int i = 0; i < current_path_.elementCount(); ++i) {
      const QPainterPath::Element& e = current_path_.elementAt(i);

      // Parse points according to movement style
      if (e.isMoveTo()) {
        std::vector<std::pair<float, float>> points;
        points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

        new_path->commands.push_back({'M', points});
      } else if (e.isLineTo()) {
        std::vector<std::pair<float, float>> points;
        points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

        new_path->commands.push_back({'L', points});
      } else if (e.isCurveTo()) {
        if (i + 2 < current_path_.elementCount()) {
          std::vector<std::pair<float, float>> points;

          points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

          const QPainterPath::Element& cp2 = current_path_.elementAt(i + 1);
          points.push_back(
              {static_cast<float>(cp2.x), static_cast<float>(cp2.y)});

          const QPainterPath::Element& end = current_path_.elementAt(i + 2);
          points.push_back(
              {static_cast<float>(end.x), static_cast<float>(end.y)});

          new_path->commands.push_back({'C', points});

          i += 2;
        }
      }
    }

    svg_.objects.push_back(new_path);
    current_path_ = QPainterPath();
    undo_stack_.push_back(prev_svg);
    redo_stack_.clear();
    update();
  }

  if (is_performing_undo_redo_) {
    undo_stack_.push_back(temp_stack_.back().Clone());
    temp_stack_.clear();
    redo_stack_.clear();
    is_performing_undo_redo_ = false;
  }
}