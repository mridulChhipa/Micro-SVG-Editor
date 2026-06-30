#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_VIEWPORT_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_VIEWPORT_H_

#include <algorithm>

#include <QPainter>
#include <QPointF>

namespace micro_svg_editor {

// Owns the pan/zoom transform between widget space and document space.
class Viewport {
 public:
  double zoom() const { return zoom_; }
  float x_offset() const { return x_offset_; }
  float y_offset() const { return y_offset_; }

  // Maps a widget-space point to document space.
  QPointF ToCanvas(QPointF point) const {
    point.setX(point.x() / zoom_ - x_offset_);
    point.setY(point.y() / zoom_ - y_offset_);
    return point;
  }

  void Apply(QPainter& painter) const {
    painter.scale(zoom_, zoom_);
    painter.translate(x_offset_, y_offset_);
  }

  // Centres a document of size (doc_w, doc_h) within the widget at the current
  // zoom.
  void Center(int widget_w, int widget_h, float doc_w, float doc_h) {
    x_offset_ = (widget_w - doc_w * zoom_) / 2.0f;
    y_offset_ = (widget_h - doc_h * zoom_) / 2.0f;
  }

  void Reset() { zoom_ = 1.0; }

  // Zooms toward a fixed widget-space point (mouse-wheel behaviour).
  void ZoomAt(QPointF widget_point, bool zoom_in) {
    float x_old = (widget_point.x() / zoom_) - x_offset_;
    float y_old = (widget_point.y() / zoom_) - y_offset_;
    if (zoom_in)
      zoom_ *= 1.05;
    else
      zoom_ /= 1.05;
    zoom_ = std::min(zoom_, 10.0);
    zoom_ = std::max(zoom_, 0.3);
    x_offset_ = (widget_point.x() / zoom_) - x_old;
    y_offset_ = (widget_point.y() / zoom_) - y_old;
  }

  void ZoomInStep() { zoom_ = std::min(zoom_ * 1.1, 10.0); }
  void ZoomOutStep() { zoom_ = std::max(zoom_ / 1.1, 0.3); }

 private:
  double zoom_ = 1.0;
  float x_offset_ = 0.0f;
  float y_offset_ = 0.0f;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CONTROLLERS_VIEWPORT_H_
