#include "Canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent), dragging(false)
{
    undo_stack.clear();
    redo_stack.clear();
}

QPointF Canvas::toCanvasCoordinates(QPointF point)
{
    point.setX(point.x() / zoom_factor - x_offset);
    point.setY(point.y() / zoom_factor - y_offset);
    return point;
}

std::string Canvas::currentCanvasToSVG() { return svg.toSVG(); }

std::optional<ShapeVariant> Canvas::toShapeVariant(const GraphicsObjectPtr &obj)
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

void Canvas::clearCanvas()
{
    svg.clear();
    update();
}

void Canvas::updateCanvasSize(int w, int h)
{
    svg.width = w;
    svg.height = h;
    update();
}

void Canvas::updateCanvas(const SVG &newSvg) // Used to update canvas when a file is parsed and opened
{
    svg = newSvg;
    x_offset = (width() - svg.width) / 2.0f;
    y_offset = (height() - svg.height) / 2.0f;
    zoom_factor = 1.0;

    update();
}
