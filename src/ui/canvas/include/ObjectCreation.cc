#include "src/ui/canvas/Canvas.h"

void Canvas::createObject(const GraphicsObjectPtr &obj, QPainterPath &path, QPen &pen)
{
  auto shape = toShapeVariant(obj);
  if (!shape)
    return;

  pen.setColor(QColor(QString::fromStdString(obj->stroke)));
  pen.setWidth(obj->stroke_width);
  pen.setStyle(Qt::SolidLine);
  pen.setJoinStyle(obj->stroke_linejoin == "round"   ? Qt::RoundJoin
                   : obj->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                     : Qt::MiterJoin);
  pen.setCapStyle(obj->stroke_linecap == "round"    ? Qt::RoundCap
                  : obj->stroke_linecap == "square" ? Qt::SquareCap
                                                    : Qt::FlatCap);
  applyDashArray(pen, obj->stroke_dasharray);

  std::visit([&](auto &&s)
             { buildShapePath(path, s); }, *shape);
}

QTransform Canvas::findTransform(const GraphicsObjectPtr &obj)
{
  QTransform transform;
  if (obj->transform.empty())
    return transform;

  QString transformStr = QString::fromStdString(obj->transform);
  // Regex to break and get transform:rotate(a, b, c)
  QRegularExpression rotateRegex(R"(rotate\(\s*([-+]?\d*\.?\d+)(?:\s*,\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+))?\s*\))");
  QRegularExpressionMatch rotateMatch = rotateRegex.match(transformStr);
  if (rotateMatch.hasMatch())
  {
    double angle = rotateMatch.captured(1).toDouble();
    if (rotateMatch.captured(2).isEmpty() || rotateMatch.captured(3).isEmpty())
    {
      transform.rotate(angle);
    }
    else
    {
      double cx = rotateMatch.captured(2).toDouble();
      double cy = rotateMatch.captured(3).toDouble();
      transform.translate(cx, cy);
      transform.rotate(angle);
      transform.translate(-cx, -cy);
    }
  }
  return transform;
}

void Canvas::applyDashArray(QPen &pen, const std::string &dasharray)
{
  if (dasharray == "none" || dasharray.empty())
    return;
  QString dashArrayStr = QString::fromStdString(dasharray);
  QStringList dashList = dashArrayStr.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
  QVector<qreal> dashes;
  for (const QString &dash : dashList)
    dashes.append(dash.toDouble() / 5.0);
  if (!dashes.isEmpty() && dashes.size() % 2 == 0)
    pen.setDashPattern(dashes);
}
