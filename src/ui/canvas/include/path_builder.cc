#include "src/ui/canvas/include/path_builder.h"

#include <QFont>
#include <QPointF>
#include <QRectF>
#include <QString>

#include "src/model/circle.h"
#include "src/model/hexagon.h"
#include "src/model/line.h"
#include "src/model/path.h"
#include "src/model/polyline.h"
#include "src/model/rect.h"
#include "src/model/text.h"

namespace micro_svg_editor {
namespace {

template <typename PointContainer>
void BuildPointPath(const PointContainer& points, bool close,
                    QPainterPath& path) {
  bool first = true;
  for (const auto& [x, y] : points) {
    if (first) {
      path.moveTo(x, y);
      first = false;
    } else {
      path.lineTo(x, y);
    }
  }
  if (close) path.closeSubpath();
}

}  // namespace

void PathBuilder::Visit(Rect& shape) {
  path_.addRoundedRect(
      QRectF(shape.x(), shape.y(), shape.width(), shape.height()), shape.rx(),
      shape.ry());
}

void PathBuilder::Visit(Circle& shape) {
  path_.addEllipse(QPointF(shape.x(), shape.y()), shape.r(), shape.r());
}

void PathBuilder::Visit(Line& shape) {
  path_.moveTo(QPointF(shape.x1(), shape.y1()));
  path_.lineTo(QPointF(shape.x2(), shape.y2()));
}

void PathBuilder::Visit(Polyline& shape) {
  BuildPointPath(shape.points(), false, path_);
}

void PathBuilder::Visit(Hexagon& shape) {
  BuildPointPath(shape.points(), true, path_);
}

void PathBuilder::Visit(Text& shape) {
  QFont font;
  font.setPointSizeF(shape.font_size());
  font.setFamily(QString::fromStdString(shape.font_family()));
  path_.addText(QPointF(shape.x(), shape.y()), font,
                QString::fromStdString(shape.content()));
}

void PathBuilder::Visit(Path& shape) {
  for (const auto& [command, points] : shape.commands()) {
    // Move the pen according to desired path shape
    if (command == 'M')
      path_.moveTo(QPointF(points[0].first, points[0].second));
    else if (command == 'L')
      path_.lineTo(QPointF(points[0].first, points[0].second));
    else if (command == 'C')
      path_.cubicTo(QPointF(points[0].first, points[0].second),
                    QPointF(points[1].first, points[1].second),
                    QPointF(points[2].first, points[2].second));
    else if (command == 'Q')
      path_.quadTo(QPointF(points[0].first, points[0].second),
                   QPointF(points[1].first, points[1].second));
    else if (command == 'H')
      path_.lineTo(QPointF(points[0].first, path_.currentPosition().y()));
    else if (command == 'V')
      path_.lineTo(QPointF(path_.currentPosition().x(), points[0].first));
    else if (command == 'Z')
      path_.closeSubpath();
  }
}

}  // namespace micro_svg_editor
