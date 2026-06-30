#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_HANDLE_GEOMETRY_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_HANDLE_GEOMETRY_H_

#include <QPointF>
#include <QRectF>

#include "src/ui/canvas/handle_type.h"

namespace micro_svg_editor {

inline constexpr int kHandleSize = 8;
// Padding added around a shape's bounds to form its selection box.
inline constexpr int kAdjust = 20;

// Returns the small square for the given resize handle on `bounds`.
QRectF HandleRect(const QRectF& bounds, HandleType type);

// Returns which handle (if any) of `bounds` contains `point`.
HandleType HitTestHandles(const QRectF& bounds, const QPointF& point);

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_HANDLE_GEOMETRY_H_
