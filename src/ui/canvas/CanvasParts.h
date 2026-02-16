#ifndef CPAR_HPP
#define CPAR_HPP

#include "src/ui/canvas/include/StackOperations.h"
#include "src/ui/canvas/include/AddShapes.h"
#include "src/ui/canvas/include/ObjectCreation.h"
#include "src/ui/canvas/include/Renderer.h"
#include "src/ui/canvas/include/DragAndResize.h"
#include "src/ui/canvas/include/BuildPaths.h"
#include "src/ui/canvas/include/RenderHandles.h"
#include "src/ui/canvas/include/Tools.h"
#include "src/ui/canvas/include/handlers/events/PaintEvent.h"
#include "src/ui/canvas/include/handlers/events/MousePressEvent.h"
#include "src/ui/canvas/include/handlers/events/MouseMoveEvent.h"
#include "src/ui/canvas/include/handlers/events/MouseReleaseEvent.h"
#include "src/ui/canvas/include/handlers/events/WheelEvent.h"

inline std::optional<ShapeVariant> Canvas::toShapeVariant(const GraphicsObjectPtr &obj)
{
  const std::string t = obj->type();
  if (t == "rect")
    return std::dynamic_pointer_cast<Rect>(obj);
  if (t == "circle")
    return std::dynamic_pointer_cast<Circle>(obj);
  if (t == "line")
    return std::dynamic_pointer_cast<Line>(obj);
  if (t == "polyline")
    return std::dynamic_pointer_cast<Polyline>(obj);
  if (t == "hexagon")
    return std::dynamic_pointer_cast<Hexagon>(obj);
  if (t == "path")
    return std::dynamic_pointer_cast<Path>(obj);
  if (t == "text")
    return std::dynamic_pointer_cast<Text>(obj);
  return std::nullopt;
}

inline void Canvas::clearCanvas()
{
  svg.clear();
  update();
}

inline void Canvas::updateCanvasSize(int w, int h)
{
  svg.width = w;
  svg.height = h;
  update();
}

inline void Canvas::updateCanvas(const SVG &newSvg) // Used to update canvas when a file is parsed and opened
{
  svg = newSvg;
  x_offset = (width() - svg.width) / 2.0f;
  y_offset = (height() - svg.height) / 2.0f;
  zoom_factor = 1.0;

  update();
}

#endif