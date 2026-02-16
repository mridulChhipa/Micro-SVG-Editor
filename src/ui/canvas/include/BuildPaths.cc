#include "src/ui/canvas/Canvas.h"

template <typename PointContainer>
void Canvas::buildPointPath(QPainterPath &path, const PointContainer &points, bool close)
{
  bool first = true;
  for (const auto &[x, y] : points)
  {
    if (first)
    {
      path.moveTo(x, y);
      first = false;
    }
    else
    {
      path.lineTo(x, y);
    }
  }
  if (close)
    path.closeSubpath();
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Rect> &s)
{
  path.addRoundedRect(QRectF(s->x, s->y, s->width, s->height), s->rx, s->ry);
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Circle> &s)
{
  path.addEllipse(QPointF(s->x, s->y), s->r, s->r);
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Line> &s)
{
  path.moveTo(QPointF(s->x1, s->y1));
  path.lineTo(QPointF(s->x2, s->y2));
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Polyline> &s)
{
  buildPointPath(path, s->points, false);
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Hexagon> &s)
{
  buildPointPath(path, s->points, true);
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Text> &s)
{
  QFont font;
  font.setPointSizeF(s->font_size);
  font.setFamily(QString::fromStdString(s->font_family));
  path.addText(QPointF(s->x, s->y), font, QString::fromStdString(s->content));
}

void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Path> &s)
{
  for (const auto &[command, points] : s->commands)
  {
    // Move the pen according to desired path shape
    if (command == 'M')
      path.moveTo(QPointF(points[0].first, points[0].second));
    else if (command == 'L')
      path.lineTo(QPointF(points[0].first, points[0].second));
    else if (command == 'C')
      path.cubicTo(QPointF(points[0].first, points[0].second), QPointF(points[1].first, points[1].second), QPointF(points[2].first, points[2].second));
    else if (command == 'Q')
      path.quadTo(QPointF(points[0].first, points[0].second), QPointF(points[1].first, points[1].second));
    else if (command == 'H')
      path.lineTo(QPointF(points[0].first, path.currentPosition().y()));
    else if (command == 'V')
      path.lineTo(QPointF(path.currentPosition().x(), points[0].first));
    else if (command == 'Z')
      path.closeSubpath();
  }
}
