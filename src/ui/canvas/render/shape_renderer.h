#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_RENDER_SHAPE_RENDERER_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_RENDER_SHAPE_RENDERER_H_

#include <QPainter>

namespace micro_svg_editor {

class SVG;
class GraphicsObject;

// Renders an SVG document onto a painter, drawing selection chrome around
// `selected` (may be null).
class ShapeRenderer {
 public:
  void Draw(QPainter& painter, const SVG& svg, const GraphicsObject* selected);
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_RENDER_SHAPE_RENDERER_H_
