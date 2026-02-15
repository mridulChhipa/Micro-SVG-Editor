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
#include "../../model/Path.hpp"
#include "../../model/Hexagon.hpp"
#include "../../model/Text.hpp"
#include "HandleType.hpp"

using ShapeVariant = std::variant<std::shared_ptr<Rect>, std::shared_ptr<Circle>, std::shared_ptr<Line>, std::shared_ptr<Polyline>, std::shared_ptr<Path>, std::shared_ptr<Hexagon>, std::shared_ptr<Text>>;

class Canvas : public QWidget
{
    Q_OBJECT
private:
    bool dragging{false};
    bool is_resizing{false};
    QPoint last_point;
    HandleType curr_handle{HandleType::None};
    QRectF start_rect;

    QVector<SVG> undoStack;
    QVector<SVG> redoStack;
    QVector<SVG> undoStackTemp;
    bool isPerformingUndoRedo{false};
    std::string undoRedoSVG;
    bool is_drawing = false;

    QPainterPath current_path;

    GraphicsObjectPtr clipboard_shape{nullptr};

    const int handle_size = 8;
    const int adjust = 20;
    double zoom_factor = 1;

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
        if (t == "path")
            return std::dynamic_pointer_cast<Path>(obj);
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
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Path> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Hexagon> &s);
    void buildShapePath(QPainterPath &path, const std::shared_ptr<Text> &s);

    void addShapeToCanvas(const std::string, QPointF location);
    QPointF toCanvasCoordinates(QPointF point)
    {
        point.setX(point.x() / zoom_factor - x_offset);
        point.setY(point.y() / zoom_factor - y_offset);
        return point;
    }

    QRectF renderHandle(const QRectF &obj, HandleType handle_type);
    HandleType hitTestHandles(const QRectF &obj, const QPointF &point);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

public:
    GraphicsObjectPtr selected_shape{nullptr};
    QString currentTool{""};
    SVG svg;

    float x_offset{0};
    float y_offset{0};

    explicit Canvas(QWidget *parent = nullptr) : QWidget(parent), dragging(false)
    {
        undoStack.clear();
        redoStack.clear();
    }
    void updateCanvas(const SVG &newSvg)
    {
        svg = newSvg;
        std::cout << width() << " " << height() << '\n';

        x_offset = (width() - svg.width) / 2.0f;
        y_offset = (height() - svg.height) / 2.0f;
        zoom_factor = 1.0;
        
        update();
    }
    void updateCanvasSize(int w, int h)
    {
        svg.width = w;
        svg.height = h;
        update();
    }
    std::string currentCanvasToSVG() { return svg.toSVG(); }
    void clearCanvas()
    {
        svg.clear();
        update();
    }
    void setCurrentTool(const QString &toolName);
    void undo();
    void redo();

    void cut();
    void copy();
    void paste();
};

#include "./include/StackOperations.hpp"
#include "./include/AddShapes.hpp"
#include "./include/ObjectCreation.hpp"
#include "./include/Renderer.hpp"
#include "./include/DragAndResize.hpp"
#include "./include/BuildPaths.hpp"
#include "./include/RenderHandles.hpp"
#include "./include/Tools.hpp"
#include "./include/handlers/events/PaintEvent.hpp"
#include "./include/handlers/events/MousePressEvent.hpp"
#include "./include/handlers/events/MouseMoveEvent.hpp"
#include "./include/handlers/events/MouseReleaseEvent.hpp"
#include "./include/handlers/events/WheelEvent.hpp"

#endif