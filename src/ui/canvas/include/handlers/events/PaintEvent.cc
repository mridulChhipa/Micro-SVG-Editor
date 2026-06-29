#include "src/ui/canvas/Canvas.h"

void Canvas::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);  // Makes lines smooth
  painter.fillRect(event->rect(),
                   Qt::darkGray);  // Canvas edge empty space fill (Lighter Gray
                                   // but darkGray)

  /*
  Zoom Application and translation of canvas according to offset (Zoom using
  scroll wheel makes current point fixed so offsets are applied)
  */
  painter.scale(zoom_factor_, zoom_factor_);
  painter.translate(x_offset_, y_offset_);

  QRectF drawing_area(0, 0, svg_.width, svg_.height);
  painter.setBrush(QColor(50, 50, 50));
  painter.setPen(Qt::NoPen);
  painter.drawRect(drawing_area);

  // Sets painter area which requies the translation we did
  painter.setClipRect(drawing_area);

  DrawSvg(painter);  // Draws the parsed document

  if (!current_path_.isEmpty()) {
    QPen pen(Qt::black, 2);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    painter.drawPath(current_path_);
  }
}
