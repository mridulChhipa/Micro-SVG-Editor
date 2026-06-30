#include "src/ui/canvas/render/shape_renderer.h"

#include <string>

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QPainterPath>
#include <QPen>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QTransform>
#include <QVector>

#include "src/model/graphics_object.h"
#include "src/model/shape_visitor.h"
#include "src/model/svg.h"
#include "src/model/text.h"
#include "src/ui/canvas/handle_geometry.h"
#include "src/ui/canvas/shape_geometry.h"

namespace micro_svg_editor {
namespace {

// Applies a text shape's font to the painter; a no-op for other shapes.
class FontApplier : public DefaultShapeVisitor {
 public:
  explicit FontApplier(QPainter& painter) : painter_(painter) {}
  void Visit(Text& text) override {
    QFont font(QString::fromStdString(text.font_family()));
    font.setPointSizeF(text.font_size());
    painter_.setFont(font);
  }

 private:
  QPainter& painter_;
};

void ApplyDashArray(QPen& pen, const std::string& dasharray) {
  if (dasharray == "none" || dasharray.empty()) return;
  QString dash_array_str = QString::fromStdString(dasharray);
  QStringList dash_list =
      dash_array_str.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
  QVector<qreal> dashes;
  for (const QString& dash : dash_list) dashes.append(dash.toDouble() / 5.0);
  if (!dashes.isEmpty() && dashes.size() % 2 == 0) pen.setDashPattern(dashes);
}

QPen BuildPen(const GraphicsObject& shape) {
  QPen pen;
  pen.setColor(QColor(QString::fromStdString(shape.stroke())));
  pen.setWidth(shape.stroke_width());
  pen.setStyle(Qt::SolidLine);
  pen.setJoinStyle(shape.stroke_linejoin() == "round"   ? Qt::RoundJoin
                   : shape.stroke_linejoin() == "bevel" ? Qt::BevelJoin
                                                        : Qt::MiterJoin);
  pen.setCapStyle(shape.stroke_linecap() == "round"    ? Qt::RoundCap
                  : shape.stroke_linecap() == "square" ? Qt::SquareCap
                                                       : Qt::FlatCap);
  ApplyDashArray(pen, shape.stroke_dasharray());
  return pen;
}

void ConfigurePainter(GraphicsObject& shape, QPainter& painter) {
  painter.setOpacity(shape.opacity());

  if (shape.visibility() == "hidden" || shape.visibility() == "collapse") {
    painter.setBrush(Qt::NoBrush);
    QPen p = painter.pen();
    p.setStyle(Qt::NoPen);
    painter.setPen(p);
    return;
  }

  if (shape.fill() != "none" && shape.fill() != "polyline") {
    QColor fill_col(QString::fromStdString(shape.fill()));
    fill_col.setAlphaF(shape.fill_opacity());
    painter.setBrush(QBrush(fill_col));
  } else {
    painter.setBrush(Qt::NoBrush);
  }

  QPen current_pen = painter.pen();
  QColor stroke_col = current_pen.color();
  stroke_col.setAlphaF(shape.stroke_opacity());
  current_pen.setColor(stroke_col);
  current_pen.setCapStyle(shape.stroke_linecap() == "round"    ? Qt::RoundCap
                          : shape.stroke_linecap() == "square" ? Qt::SquareCap
                                                               : Qt::FlatCap);
  current_pen.setJoinStyle(shape.stroke_linejoin() == "round" ? Qt::RoundJoin
                           : shape.stroke_linejoin() == "bevel"
                               ? Qt::BevelJoin
                               : Qt::MiterJoin);
  painter.setPen(current_pen);

  FontApplier font_applier(painter);  // Sets the font for text shapes only.
  shape.Accept(font_applier);
}

void DrawSelection(QPainter& painter, const QPainterPath& path,
                   const QTransform& transform) {
  painter.save();  // Checkpoint so the shape pen/brush are restored afterwards.
  if (!transform.isIdentity()) painter.setTransform(transform, true);
  QPen handle_pen(Qt::blue, 1, Qt::DashLine);
  painter.setPen(handle_pen);
  painter.setBrush(Qt::NoBrush);
  QRectF sel_rect =
      path.boundingRect().adjusted(-kAdjust, -kAdjust, kAdjust, kAdjust);
  painter.drawRect(sel_rect);
  painter.setPen(Qt::blue);
  painter.setBrush(Qt::white);
  for (int i = 0; i < 8; ++i) {
    painter.drawRect(HandleRect(sel_rect, static_cast<HandleType>(i)));
  }
  painter.restore();
}

}  // namespace

void ShapeRenderer::Draw(QPainter& painter, const SVG& svg,
                         const GraphicsObject* selected) {
  for (const GraphicsObjectPtr& obj : svg.objects()) {
    QPen pen = BuildPen(*obj);
    painter.setPen(pen);
    ConfigurePainter(*obj, painter);

    QPainterPath path = BuildShapePath(*obj);
    QTransform transform = ShapeTransform(*obj);
    if (!transform.isIdentity()) {
      painter.setTransform(transform, true);
      painter.drawPath(path);
      painter.setTransform(QTransform());
    } else {
      painter.drawPath(path);
    }

    if (obj.get() == selected) DrawSelection(painter, path, transform);
  }
}

}  // namespace micro_svg_editor
