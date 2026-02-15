#ifndef WHEEL_EVENT_HPP
#define WHEEL_EVENT_HPP

// #include <

inline void Canvas::wheelEvent(QWheelEvent *event)
{
    QPointF mousePos = event->position();

    float x_old = (mousePos.x() / zoom_factor) - x_offset;
    float y_old = (mousePos.y() / zoom_factor) - y_offset;

    if (event->angleDelta().y() > 0)
        zoom_factor *= 1.05;
    else
        zoom_factor /= 1.05;

    zoom_factor = std::min(zoom_factor, 10.0);
    zoom_factor = std::max(zoom_factor, 0.3);

    x_offset = (mousePos.x() / zoom_factor) - x_old;
    y_offset = (mousePos.y() / zoom_factor) - y_old;

    update();
}

inline void Canvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    x_offset = (width() - svg.width) / 2.0f;
    y_offset = (height() - svg.height) / 2.0f;
    update();
}

#endif