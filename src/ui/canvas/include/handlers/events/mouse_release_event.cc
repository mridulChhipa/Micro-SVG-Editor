#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {

std::unique_ptr<Path> Canvas::BuildFreehandPath() {
  auto new_path = std::make_unique<Path>();
  for (int i = 0; i < current_path_.elementCount(); ++i) {
    const QPainterPath::Element& e = current_path_.elementAt(i);

    // Parse points according to movement style
    if (e.isMoveTo()) {
      std::vector<std::pair<float, float>> points;
      points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

      new_path->commands().push_back({'M', points});
    } else if (e.isLineTo()) {
      std::vector<std::pair<float, float>> points;
      points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

      new_path->commands().push_back({'L', points});
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

        new_path->commands().push_back({'C', points});

        i += 2;
      }
    }
  }
  return new_path;
}

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
  // Any kind of event is stopped when we release the mouse.
  dragging_ = false;
  is_resizing_ = false;
  curr_handle_ = HandleType::kNone;

  if (event->button() == Qt::LeftButton && is_drawing_ &&
      curr_tool_ == "Freehand") {
    // Finalise the freehand sketch into a Path object.
    SVG prev_svg = svg_.Clone();

    is_drawing_ = false;
    svg_.objects().push_back(BuildFreehandPath());
    current_path_ = QPainterPath();
    history_.Push(std::move(prev_svg));
    update();
  }

  if (history_.gesture_active()) history_.CommitGesture();
}

}  // namespace micro_svg_editor
