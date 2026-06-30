#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_SHAPE_GEOMETRY_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_SHAPE_GEOMETRY_H_

#include <QPainterPath>
#include <QTransform>

namespace micro_svg_editor {

class GraphicsObject;

// Builds the QPainterPath for a shape (via the PathBuilder visitor).
QPainterPath BuildShapePath(GraphicsObject& shape);

// Parses a shape's `transform` attribute into a QTransform (rotate only).
QTransform ShapeTransform(const GraphicsObject& shape);

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_SHAPE_GEOMETRY_H_
