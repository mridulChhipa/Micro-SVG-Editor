#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_SELECTION_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_SELECTION_H_

#include <QPointF>

#include "src/ui/canvas/handle_type.h"

namespace micro_svg_editor {

class GraphicsObject;
class SVG;

// Returns the resize handle of `shape` under `canvas_point`, or kNone.
HandleType HitHandle(GraphicsObject& shape, QPointF canvas_point);

// Returns the topmost shape in `svg` under `canvas_point`, or nullptr.
GraphicsObject* PickTopmost(SVG& svg, QPointF canvas_point);

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_SELECTION_H_
