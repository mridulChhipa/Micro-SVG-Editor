#ifndef PAINT_EVENT_HPP
#define PAINT_EVENT_HPP

inline void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Makes lines smooth
    painter.fillRect(event->rect(), Qt::darkGray); // Canvas edge empty space fill

    float x_offset = (width() - svg.width) / 2.0f;
    float y_offset = (height() - svg.height) / 2.0f;

    painter.translate(x_offset, y_offset);
    QRectF drawing_area(0, 0, svg.width, svg.height);
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::NoPen);
    painter.drawRect(drawing_area);

    painter.setClipRect(drawing_area);

    drawSVG(painter);

    if (!current_path.isEmpty())
    {
        QPen pen(Qt::black, 2);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);

        painter.drawPath(current_path);
    }
}

#endif