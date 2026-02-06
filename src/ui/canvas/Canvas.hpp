#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <ranges> // to reverse iterate through objects for selection

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QImage>
#include <QResizeEvent>
#include <QRegularExpression>

#include "../../model/SVG.hpp"
#include "../../model/Rect.hpp"
#include "../../model/Circle.hpp"
#include "../../model/Line.hpp"
#include "../../model/Polyline.hpp"
#include "../../model/Polygon.hpp"
#include "../../model/Text.hpp"
#include "HandleType.hpp"

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

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        // Iterate in reverse order to select topmost object first
        for (const GraphicsObjectPtr &obj : svg.objects | std::views::reverse)
        {
            QPainterPath path;
            QPen pen;
            createObject(obj, path, pen);
            if (obj->type() == "line" || obj->type() == "polyline")
            {
                if (path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust).contains(event->pos()))
                {
                    dragging = true;
                    selected_shape = obj;
                    last_point = event->pos();
                    break;
                }
            }
            else if (path.boundingRect().contains(event->pos()))
            {
                dragging = true;
                selected_shape = obj;
                last_point = event->pos();
                break;
            }
        }

        if (selected_shape)
        {
            std::cout << "Selected shape type: " << selected_shape->type() << std::endl;
            QRectF boundingRect;
            QPainterPath path;
            if (selected_shape->type() == "rect")
            {
                std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                path.addRoundedRect(QRectF(rect->x, rect->y, rect->width, rect->height), rect->rx, rect->ry);
            }
            else if (selected_shape->type() == "circle")
            {
                std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                path.addEllipse(QPointF(circle->x, circle->y), circle->r, circle->r);
            }
            else if (selected_shape->type() == "line")
            {
                std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                path.moveTo(line->x1, line->y1);
                path.lineTo(line->x2, line->y2);
            }
            else if (selected_shape->type() == "polyline")
            {
                std::shared_ptr<Polyline> polyline = std::dynamic_pointer_cast<Polyline>(selected_shape);
                for (const auto &[x, y] : polyline->points)
                {
                    if (path.isEmpty())
                        path.moveTo(x, y);
                    else
                        path.lineTo(x, y);
                }
            }
            else if (selected_shape->type() == "polygon")
            {
                std::shared_ptr<Polygon> polygon = std::dynamic_pointer_cast<Polygon>(selected_shape);
                for (const auto &[x, y] : polygon->points)
                {
                    if (path.isEmpty())
                        path.moveTo(x, y);
                    else
                        path.lineTo(x, y);
                }
                path.closeSubpath();
            }
            else if (selected_shape->type() == "text")
            {
                std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                path.addText(QPointF(text->x, text->y), QFont(QString::fromStdString(text->font_family), text->font_size), QString::fromStdString(text->content));
            }

            boundingRect = path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust);

            HandleType handle = hitTestHandles(boundingRect, event->pos());
            std::cout << "Hit test handle: " << static_cast<int>(handle) << std::endl;

            if (handle != HandleType::None)
            {
                std::cout << "Resizing from handle: " << static_cast<int>(handle) << std::endl;
                is_resizing = true;
                curr_handle = handle;
                start_rect = boundingRect;
                last_point = event->pos();
                dragging = false;
            }
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            QPoint delta = event->pos() - last_point;
            last_point = event->pos();

            if (selected_shape && !is_resizing)
            {
                if (selected_shape->type() == "rect")
                {
                    std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                    rect->x += delta.x();
                    rect->y += delta.y();
                }
                else if (selected_shape->type() == "circle")
                {
                    std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                    circle->x += delta.x();
                    circle->y += delta.y();
                }
                else if (selected_shape->type() == "line")
                {
                    std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                    line->x1 += delta.x();
                    line->y1 += delta.y();
                    line->x2 += delta.x();
                    line->y2 += delta.y();
                }
                else if (selected_shape->type() == "polyline")
                {
                    std::shared_ptr<Polyline> polyline = std::dynamic_pointer_cast<Polyline>(selected_shape);
                    for (auto &[x, y] : polyline->points)
                    {
                        x += delta.x();
                        y += delta.y();
                    }
                }
                else if (selected_shape->type() == "polygon")
                {
                    std::shared_ptr<Polygon> polygon = std::dynamic_pointer_cast<Polygon>(selected_shape);
                    for (auto &[x, y] : polygon->points)
                    {
                        x += delta.x();
                        y += delta.y();
                    }
                }
                else if (selected_shape->type() == "text")
                {
                    std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                    text->x += delta.x();
                    text->y += delta.y();
                }
                update();
            }

            if (is_resizing && selected_shape)
            {
                if (curr_handle == HandleType::TopLeft)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x() / 2.0;
                        circle->y += delta.y() / 2.0;
                        circle->r -= std::max(delta.x(), delta.y());
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::TopRight)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::BottomLeft)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::BottomRight)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::TopCenter)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::MiddleLeft)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::MiddleRight)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                else if (curr_handle == HandleType::BottomCenter)
                {
                    if (selected_shape->type() == "rect")
                    {
                        std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(selected_shape);
                        rect->x += delta.x();
                        rect->y += delta.y();
                        rect->width -= delta.x();
                        rect->height -= delta.y();
                    }
                    else if (selected_shape->type() == "circle")
                    {
                        std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(selected_shape);
                        circle->x += delta.x();
                        circle->y += delta.y();
                        circle->r -= std::max(delta.x(), delta.y()) / 2;
                    }
                    else if (selected_shape->type() == "line")
                    {
                        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(selected_shape);
                        line->x1 += delta.x();
                        line->y1 += delta.y();
                    }
                    else if (selected_shape->type() == "polyline")
                    {
                        // Resizing polyline from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "polygon")
                    {
                        // Resizing polygon from top-left handle is complex; skipping for simplicity
                    }
                    else if (selected_shape->type() == "text")
                    {
                        std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(selected_shape);
                        text->x += delta.x();
                        text->y += delta.y();
                        text->font_size -= std::max(delta.x(), delta.y()) / 10;
                    }
                }
                update();
            }
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        dragging = false;
        // selected_shape = nullptr;
        is_resizing = false;
        curr_handle = HandleType::None;
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.fillRect(event->rect(), QColor("#414141")); // Clear the canvas with gray background
        drawSVG(painter);
    }

    void drawRotationIcon(QPainter *p, QPointF center, double size)
    {
        p->setBrush(Qt::white);
        p->setPen(Qt::black);
        // Draw a small circle or a specific rotation pixmap
        p->drawEllipse(center, size / 2, size / 2);
    }

private:
    bool dragging{false};
    bool is_resizing{false};
    QPoint last_point;
    GraphicsObjectPtr selected_shape{nullptr};
    HandleType curr_handle{HandleType::None};
    QRectF start_rect;

    const int handle_size = 8;
    const int adjust = 20;

    SVG svg;

    void createObject(const GraphicsObjectPtr &obj, QPainterPath &path, QPen &pen)
    {
        if (obj->type() == "rect")
        {
            std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(obj);

            pen.setColor(QColor(QString::fromStdString(rect->stroke)));
            pen.setWidth(rect->stroke_width);
            pen.setStyle(Qt::SolidLine);
            pen.setJoinStyle(Qt::RoundJoin); // Makes the outer corners look smoother

            path.addRoundedRect(QRectF(rect->x, rect->y, rect->width, rect->height), rect->rx, rect->ry);
        }
        else if (obj->type() == "circle")
        {
            std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(obj);

            pen.setColor(QColor(QString::fromStdString(circle->stroke)));
            pen.setWidth(circle->stroke_width);
            pen.setStyle(Qt::SolidLine);
            pen.setJoinStyle(Qt::RoundJoin); // Makes the outer corners look smoother

            path.addEllipse(QPointF(circle->x, circle->y), circle->r, circle->r);
        }
        else if (obj->type() == "line")
        {
            std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(obj);

            pen.setColor(QColor(QString::fromStdString(line->stroke)));
            pen.setWidth(line->stroke_width);
            pen.setStyle(Qt::SolidLine);
            pen.setJoinStyle(Qt::PenJoinStyle(line->stroke_linecap == "round" ? Qt::RoundJoin : line->stroke_linecap == "bevel" ? Qt::BevelJoin
                                                                                                                                : Qt::MiterJoin));
            pen.setCapStyle(Qt::PenCapStyle(line->stroke_linecap == "round" ? Qt::RoundCap : line->stroke_linecap == "square" ? Qt::SquareCap
                                                                                                                              : Qt::FlatCap));
            if (line->stroke_dasharray != "none")
            {
                QString dashArrayStr = QString::fromStdString(line->stroke_dasharray);
                QStringList dashList = dashArrayStr.split(",");
                QVector<qreal> dashes;
                for (const QString &dash : dashList)
                {
                    dashes.append(dash.toDouble() / 5.0);
                }
                if (!dashes.isEmpty() && dashes.size() % 2 == 0)
                    pen.setDashPattern(dashes);
            };

            path.moveTo(QPointF(line->x1, line->y1));
            path.lineTo(QPointF(line->x2, line->y2));
        }
        else if (obj->type() == "polyline")
        {
            std::shared_ptr<Polyline> polyline = std::dynamic_pointer_cast<Polyline>(obj);
            pen.setColor(QColor(QString::fromStdString(polyline->stroke)));
            pen.setWidth(polyline->stroke_width);
            pen.setStyle(Qt::SolidLine);
            pen.setJoinStyle(Qt::PenJoinStyle(polyline->stroke_linejoin == "round" ? Qt::RoundJoin : polyline->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                                                                                                          : Qt::MiterJoin));
            if (polyline->stroke_dasharray != "none" && !polyline->stroke_dasharray.empty())
            {
                QString dashArrayStr = QString::fromStdString(polyline->stroke_dasharray);
                QStringList dashList = dashArrayStr.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
                QVector<qreal> dashes;
                for (const QString &dash : dashList)
                {
                    dashes.append(dash.toDouble() / 5.0);
                }
                if (!dashes.isEmpty() && dashes.size() % 2 == 0)
                    pen.setDashPattern(dashes);
            }

            bool first = true;
            for (const auto &[x, y] : polyline->points)
            {
                if (first)
                {
                    path.moveTo(x, y); // Start the path here
                    first = false;
                }
                else
                {
                    path.lineTo(x, y); // Draw to subsequent points
                }
            }
        }
        else if (obj->type() == "polygon")
        {
            std::shared_ptr<Polygon> polygon = std::dynamic_pointer_cast<Polygon>(obj);
            pen.setColor(QColor(QString::fromStdString(polygon->stroke)));
            pen.setWidth(polygon->stroke_width);
            pen.setStyle(Qt::SolidLine);
            pen.setJoinStyle(Qt::PenJoinStyle(polygon->stroke_linejoin == "round" ? Qt::RoundJoin : polygon->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                                                                                                        : Qt::MiterJoin));
            if (polygon->stroke_dasharray != "none" && !polygon->stroke_dasharray.empty())
            {
                QString dashArrayStr = QString::fromStdString(polygon->stroke_dasharray);
                QStringList dashList = dashArrayStr.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);

                QVector<qreal> dashes;
                for (const QString &dash : dashList)
                {
                    dashes.append(dash.toDouble());
                }
                if (!dashes.isEmpty() && dashes.size() % 2 == 0)
                    pen.setDashPattern(dashes);
            }

            for (auto &[x, y] : polygon->points)
            {
                if (&x == &polygon->points.front().first && &y == &polygon->points.front().second)
                {
                    path.moveTo(x, y); // Start the path here
                }
                else
                {
                    path.lineTo(x, y); // Draw to subsequent points
                }
            }

            auto &[x1, y1] = polygon->points.front();
            path.lineTo(x1, y1); // Close the polyline path
        }
        else if (obj->type() == "text")
        {
            std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(obj);

            pen.setColor(QColor(QString::fromStdString(text->stroke)));
            QFont font;
            font.setPointSizeF(text->font_size);
            font.setFamily(QString::fromStdString(text->font_family));
            path.addText(QPointF(text->x, text->y), font, QString::fromStdString(text->content));
        }
    }

    void createBrush(const GraphicsObjectPtr &obj, QPainter &painter)
    {
        painter.setOpacity(obj->opacity);

        if (obj->visibility == "hidden" || obj->visibility == "collapse")
        {
            painter.setBrush(Qt::NoBrush);
            QPen p = painter.pen();
            p.setStyle(Qt::NoPen);
            painter.setPen(p);
            return;
        }

        if (obj->fill != "none" && obj->fill != "polyline")
        {
            QColor fillCol(QString::fromStdString(obj->fill));
            fillCol.setAlphaF(obj->fill_opacity);
            painter.setBrush(QBrush(fillCol));
        }
        else
        {
            painter.setBrush(Qt::NoBrush);
        }

        QPen currentPen = painter.pen();
        QColor strokeCol = currentPen.color();
        strokeCol.setAlphaF(obj->stroke_opacity);
        currentPen.setColor(strokeCol);

        currentPen.setCapStyle(obj->stroke_linecap == "round" ? Qt::RoundCap : obj->stroke_linecap == "square" ? Qt::SquareCap
                                                                                                               : Qt::FlatCap);
        currentPen.setJoinStyle(obj->stroke_linejoin == "round" ? Qt::RoundJoin : obj->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                                                                                  : Qt::MiterJoin);

        painter.setPen(currentPen);

        if (obj->type() == "text")
        {
            auto textObj = std::dynamic_pointer_cast<Text>(obj);
            if (textObj)
            {
                QFont font(QString::fromStdString(textObj->font_family));
                font.setPointSizeF(textObj->font_size);
                painter.setFont(font);
            }
        }
    }
    void drawSVG(QPainter &painter)
    {
        painter.setRenderHint(QPainter::Antialiasing);

        for (const GraphicsObjectPtr &obj : svg.objects)
        {
            QPainterPath path;
            QPen pen;
            createObject(obj, path, pen);
            painter.setPen(pen);
            createBrush(obj, painter);
            QTransform transform;
            if (!obj->transform.empty())
            {
                QString transformStr = QString::fromStdString(obj->transform);
                QRegularExpression rotateRegex(R"(rotate\(\s*([-+]?\d*\.?\d+)(?:\s*,\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+))?\s*\))");
                QRegularExpressionMatch rotateMatch = rotateRegex.match(transformStr);
                if (rotateMatch.hasMatch())
                {
                    double angle = rotateMatch.captured(1).toDouble();
                    if (rotateMatch.captured(2).isEmpty() || rotateMatch.captured(3).isEmpty())
                    {
                        transform.rotate(angle);
                    }
                    else
                    {
                        double cx = rotateMatch.captured(2).toDouble();
                        double cy = rotateMatch.captured(3).toDouble();
                        transform.translate(cx, cy);
                        transform.rotate(angle);
                        transform.translate(-cx, -cy);
                    }
                }
                painter.setTransform(transform, true);
                painter.drawPath(path);
                painter.setTransform(QTransform()); // Reset transform
            }

            if (obj == selected_shape)
            {
                painter.save();
                QPen handlePen(Qt::blue, 1, Qt::DashLine);
                painter.setPen(handlePen);
                painter.setBrush(Qt::NoBrush);
                painter.drawRect(path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust)); // Draw bounding box with some padding

                painter.setPen(Qt::blue);
                painter.setBrush(Qt::white);
                for (int i = 0; i < 8; ++i)
                {
                    QRectF handleRect = renderHandle(path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust), static_cast<HandleType>(i));
                    painter.drawRect(handleRect);
                }
                painter.restore();
            }
        }
    }

    QRectF renderHandle(const QRectF &obj, HandleType handle_type)
    {
        double x, y;
        switch (handle_type)
        {
        case HandleType::TopLeft:
            x = obj.left();
            y = obj.top();
            break;
        case HandleType::TopCenter:
            x = obj.center().x();
            y = obj.top();
            break;
        case HandleType::TopRight:
            x = obj.right();
            y = obj.top();
            break;
        case HandleType::MiddleLeft:
            x = obj.left();
            y = obj.center().y();
            break;
        case HandleType::MiddleRight:
            x = obj.right();
            y = obj.center().y();
            break;
        case HandleType::BottomLeft:
            x = obj.left();
            y = obj.bottom();
            break;
        case HandleType::BottomCenter:
            x = obj.center().x();
            y = obj.bottom();
            break;
        case HandleType::BottomRight:
            x = obj.right();
            y = obj.bottom();
            break;
        default:
            x = obj.left();
            y = obj.top();
            break;
        }

        // Update QRectF to handle the adjusted bounding box
        return QRectF(x - handle_size / 2, y - handle_size / 2, handle_size, handle_size);
    }

    HandleType hitTestHandles(const QRectF &obj, const QPointF &point)
    {
        for (int i = 0; i < 8; ++i)
        {
            QRectF handleRect = renderHandle(obj, static_cast<HandleType>(i));
            if (handleRect.contains(point))
            {
                return static_cast<HandleType>(i);
            }
        }

        return HandleType::None;
    }
};

#endif