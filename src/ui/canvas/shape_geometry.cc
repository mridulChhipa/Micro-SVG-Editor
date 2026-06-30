#include "src/ui/canvas/shape_geometry.h"

#include <QRegularExpression>
#include <QString>

#include "src/model/graphics_object.h"
#include "src/ui/canvas/include/path_builder.h"

namespace micro_svg_editor {

QPainterPath BuildShapePath(GraphicsObject& shape) {
  QPainterPath path;
  PathBuilder builder(path);
  shape.Accept(builder);
  return path;
}

QTransform ShapeTransform(const GraphicsObject& shape) {
  QTransform transform;
  if (shape.transform().empty()) return transform;

  QString transform_str = QString::fromStdString(shape.transform());
  // Regex to break and get transform:rotate(a, b, c)
  QRegularExpression rotate_regex(
      R"(rotate\(\s*([-+]?\d*\.?\d+)(?:\s*,\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+))?\s*\))");
  QRegularExpressionMatch rotate_match = rotate_regex.match(transform_str);
  if (rotate_match.hasMatch()) {
    double angle = rotate_match.captured(1).toDouble();
    if (rotate_match.captured(2).isEmpty() ||
        rotate_match.captured(3).isEmpty()) {
      transform.rotate(angle);
    } else {
      double cx = rotate_match.captured(2).toDouble();
      double cy = rotate_match.captured(3).toDouble();
      transform.translate(cx, cy);
      transform.rotate(angle);
      transform.translate(-cx, -cy);
    }
  }
  return transform;
}

}  // namespace micro_svg_editor
