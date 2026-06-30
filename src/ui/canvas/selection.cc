#include "src/ui/canvas/selection.h"

#include <ranges>

#include <QPainterPath>
#include <QRectF>
#include <QTransform>

#include "src/model/graphics_object.h"
#include "src/model/svg.h"
#include "src/ui/canvas/handle_geometry.h"
#include "src/ui/canvas/shape_geometry.h"

namespace micro_svg_editor {
namespace {

// Maps a canvas-space point into the shape's local (untransformed) space.
QPointF ToLocal(const QTransform& transform, QPointF point) {
  if (transform.isIdentity()) return point;
  return transform.inverted().map(point);
}

}  // namespace

HandleType HitHandle(GraphicsObject& shape, QPointF canvas_point) {
  QPainterPath path = BuildShapePath(shape);
  QPointF local = ToLocal(ShapeTransform(shape), canvas_point);
  QRectF bounds =
      path.boundingRect().adjusted(-kAdjust, -kAdjust, kAdjust, kAdjust);
  return HitTestHandles(bounds, local);
}

GraphicsObject* PickTopmost(SVG& svg, QPointF canvas_point) {
  // Reverse order so the topmost (last drawn) shape is hit first.
  for (const GraphicsObjectPtr& obj : svg.objects() | std::views::reverse) {
    QPainterPath path = BuildShapePath(*obj);
    QPointF local = ToLocal(ShapeTransform(*obj), canvas_point);
    // Shapes that are essentially strokes get a padded hit area.
    if (obj->PreferStrokeHitTest()) {
      if (path.boundingRect()
              .adjusted(-kAdjust, -kAdjust, kAdjust, kAdjust)
              .contains(local))
        return obj.get();
    } else if (path.boundingRect().contains(local)) {
      return obj.get();
    }
  }
  return nullptr;
}

}  // namespace micro_svg_editor
