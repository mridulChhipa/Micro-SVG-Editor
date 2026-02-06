#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <ranges>
#include <algorithm>
#include <variant>
#include <optional>

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QImage>
#include <QResizeEvent>
#include <QRegularExpression>
#include <QInputDialog>

#include "../../model/SVG.hpp"
#include "../../model/Rect.hpp"
#include "../../model/Circle.hpp"
#include "../../model/Line.hpp"
#include "../../model/Polyline.hpp"
#include "../../model/Hexagon.hpp"
#include "../../model/Text.hpp"
#include "HandleType.hpp"

using ShapeVariant = std::variant<std::shared_ptr<Rect>, std::shared_ptr<Circle>, std::shared_ptr<Line>, std::shared_ptr<Polyline>, std::shared_ptr<Hexagon>, std::shared_ptr<Text>>;

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr) : QWidget(parent), dragging(false) {}

    void updateCanvas(const SVG &newSvg)
    {
        svg = newSvg;
        update();
    }

    std::string currentCanvasToSVG()
    {
        return svg.toSVG();
    }

    void clearCanvas()
    {
        svg.clear();
        update();
    }

    void setCurrentTool(const QString &toolName);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.fillRect(event->rect(), QColor("#414141"));
        drawSVG(painter);
    }

private:
    bool dragging{false};
    bool is_resizing{false};
    QPoint last_point;
    GraphicsObjectPtr selected_shape{nullptr};
    HandleType curr_handle{HandleType::None};
    QRectF start_rect;

    QString currentTool;

    const int handle_size = 8;
    const int adjust = 20;

    SVG svg;

    std::optional<ShapeVariant> toShapeVariant(const GraphicsObjectPtr &obj)
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
        if (t == "text")
            return std::dynamic_pointer_cast<Text>(obj);
        return std::nullopt;
    }

    QTransform findTransform(const GraphicsObjectPtr &obj);
    void createObject(const GraphicsObjectPtr &obj, QPainterPath &path, QPen &pen);
    void applyDashArray(QPen &pen, const std::string &dasharray);

    void createBrush(const GraphicsObjectPtr &obj, QPainter &painter);
    void drawSVG(QPainter &painter);

    void applyDrag(const QPoint &delta);
    void applyResize(const QPoint &delta, HandleType handle);

    template <typename PointContainer>
    void buildPointPath(QPainterPath &path, const PointContainer &points, bool close);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Rect> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Circle> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Line> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Polyline> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Hexagon> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Text> &s);

    void addShapeToCanvas(const std::string);

    QRectF renderHandle(const QRectF &obj, HandleType handle_type);
    HandleType hitTestHandles(const QRectF &obj, const QPointF &point);
};

#include "./include/MouseEvents.hpp"
#include "./include/AddShapes.hpp"
#include "./include/ObjectCreation.hpp"
#include "./include/Renderer.hpp"
#include "./include/DragAndResize.hpp"
#include "./include/BuildPaths.hpp"
#include "./include/RenderHandles.hpp"
#include "./include/Tools.hpp"

#endif