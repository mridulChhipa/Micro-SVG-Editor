#include "src/ui/canvas/Canvas.h"

void Canvas::CreateObject(const GraphicsObjectPtr& obj, QPainterPath& path,
                          QPen& pen) {
  auto shape = ToShapeVariant(obj);
  if (!shape) return;

  pen.setColor(QColor(QString::fromStdString(obj->stroke)));
  pen.setWidth(obj->stroke_width);
  pen.setStyle(Qt::SolidLine);
  pen.setJoinStyle(obj->stroke_linejoin == "round"   ? Qt::RoundJoin
                   : obj->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                     : Qt::MiterJoin);
  pen.setCapStyle(obj->stroke_linecap == "round"    ? Qt::RoundCap
                  : obj->stroke_linecap == "square" ? Qt::SquareCap
                                                    : Qt::FlatCap);
  ApplyDashArray(pen, obj->stroke_dasharray);

  std::visit([&](auto&& s) { BuildShapePath(path, s); }, *shape);
}

QTransform Canvas::FindTransform(const GraphicsObjectPtr& obj) {
  QTransform transform;
  if (obj->transform.empty()) return transform;

  QString transform_str = QString::fromStdString(obj->transform);
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

void Canvas::ApplyDashArray(QPen& pen, const std::string& dasharray) {
  if (dasharray == "none" || dasharray.empty()) return;
  QString dash_array_str = QString::fromStdString(dasharray);
  QStringList dash_list =
      dash_array_str.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
  QVector<qreal> dashes;
  for (const QString& dash : dash_list) dashes.append(dash.toDouble() / 5.0);
  if (!dashes.isEmpty() && dashes.size() % 2 == 0) pen.setDashPattern(dashes);
}
