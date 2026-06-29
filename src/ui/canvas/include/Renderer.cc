#include "src/ui/canvas/Canvas.h"

void Canvas::CreateBrush(const GraphicsObjectPtr& obj, QPainter& painter) {
  painter.setOpacity(obj->opacity);

  if (obj->visibility == "hidden" || obj->visibility == "collapse") {
    painter.setBrush(Qt::NoBrush);
    QPen p = painter.pen();
    p.setStyle(Qt::NoPen);
    painter.setPen(p);
    return;
  }

  if (obj->fill != "none" && obj->fill != "polyline") {
    QColor fill_col(QString::fromStdString(obj->fill));
    fill_col.setAlphaF(obj->fill_opacity);
    painter.setBrush(QBrush(fill_col));
  } else
    painter.setBrush(Qt::NoBrush);

  QPen current_pen = painter.pen();
  QColor stroke_col = current_pen.color();
  stroke_col.setAlphaF(obj->stroke_opacity);
  current_pen.setColor(stroke_col);
  current_pen.setCapStyle(obj->stroke_linecap == "round"    ? Qt::RoundCap
                          : obj->stroke_linecap == "square" ? Qt::SquareCap
                                                            : Qt::FlatCap);
  current_pen.setJoinStyle(obj->stroke_linejoin == "round"   ? Qt::RoundJoin
                           : obj->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                             : Qt::MiterJoin);
  painter.setPen(current_pen);

  if (obj->Type() == "text")  // Special conditions to pen for text
  {
    auto text_obj = std::dynamic_pointer_cast<Text>(obj);
    if (text_obj) {
      QFont font(QString::fromStdString(text_obj->font_family));
      font.setPointSizeF(text_obj->font_size);
      painter.setFont(font);
    }
  }
}

void Canvas::DrawSvg(QPainter& painter) {
  for (const GraphicsObjectPtr& obj : svg_.objects) {
    QPainterPath path;
    QPen pen;
    CreateObject(obj, path, pen);
    painter.setPen(pen);
    CreateBrush(obj, painter);
    QTransform transform = FindTransform(obj);
    if (!transform.isIdentity()) {
      painter.setTransform(transform, true);
      painter.drawPath(path);
      painter.setTransform(QTransform());
    } else
      painter.drawPath(path);
    if (obj == selected_shape_) {
      painter.save();  // Create a check point to restore pointer later
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
        QRectF handle_rect = RenderHandle(sel_rect, static_cast<HandleType>(i));
        painter.drawRect(handle_rect);
      }
      painter.restore();  // Restore pointer neccessary because then pen
                          // painting the shapes would have been disturbed
    }
  }
}
