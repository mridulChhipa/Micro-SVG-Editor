#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QResizeEvent>

#include "../../model/SVG.hpp"
#include "../../model/Rect.hpp"
#include "../../model/Circle.hpp"
#include "../../model/Line.hpp"
#include "../../model/Polyline.hpp"
#include "../../model/Polygon.hpp"
#include "../../model/Text.hpp"

// Use QuadTrees for optimization if needed in future
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
        selected_object = nullptr;
        for (const GraphicsObjectPtr &obj : svg.objects)
        {
            // Placeholder for hit-testing logic
            if (obj->contains(event->pos()))
            {
                selected_object = obj;
                break;
            }
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if ((event->buttons() & Qt::LeftButton) && dragging)
        {
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && dragging)
        {
            dragging = false;
        }
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.fillRect(event->rect(), QColor("#414141")); // Clear the canvas with gray background
        drawSVG(painter);
    }

private:
    bool dragging{false};
    QPoint lastPoint;
    SVG svg;

    GraphicsObjectPtr selected_object{nullptr};

    void drawSVG(QPainter &painter)
    {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setRenderHint(QPainter::Antialiasing);

        for (const GraphicsObjectPtr &obj : svg.objects)
        {
            if (!obj)
                continue;

            if (obj->type() == "rect")
            {
                std::shared_ptr<Rect> rect = std::dynamic_pointer_cast<Rect>(obj);

                QPen pen;
                pen.setColor(QColor(QString::fromStdString(rect->stroke)));
                pen.setWidth(rect->stroke_width);
                pen.setStyle(Qt::SolidLine);
                pen.setJoinStyle(Qt::RoundJoin); // Makes the outer corners look smoother

                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(QString::fromStdString(rect->fill))));

                painter.drawRoundedRect(QRectF(rect->x, rect->y, rect->width, rect->height), rect->rx, rect->ry);
            }
            else if (obj->type() == "circle")
            {
                std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(obj);

                QPen pen;
                pen.setColor(QColor(QString::fromStdString(circle->stroke)));
                pen.setWidth(circle->stroke_width);
                pen.setStyle(Qt::SolidLine);
                pen.setJoinStyle(Qt::RoundJoin); // Makes the outer corners look smoother

                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(QString::fromStdString(circle->fill))));

                painter.drawEllipse(QPointF(circle->x, circle->y), circle->r, circle->r);
            }
            else if (obj->type() == "line")
            {
                std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(obj);

                QPen pen;
                pen.setColor(QColor(QString::fromStdString(line->stroke)));
                pen.setWidth(line->stroke_width);
                pen.setStyle(Qt::SolidLine);
                // Join style from object
                pen.setJoinStyle(Qt::PenJoinStyle(line->stroke_linecap == "round" ? Qt::RoundJoin : line->stroke_linecap == "bevel" ? Qt::BevelJoin
                                                                                                                                    : Qt::MiterJoin));
                // use stroke_dasharray to set custom dash pattern if available
                if (line->stroke_dasharray != "none")
                {
                    QString dashArrayStr = QString::fromStdString(line->stroke_dasharray);
                    QStringList dashList = dashArrayStr.split(",");
                    QVector<qreal> dashes;
                    for (const QString &dash : dashList)
                    {
                        dashes.append(dash.toDouble());
                    }
                    pen.setDashPattern(dashes);
                }
                painter.setPen(pen);

                painter.drawLine(QPointF(line->x1, line->y1), QPointF(line->x2, line->y2));
            }
            else if (obj->type() == "polyline")
            {
                // Handle polyline drawing
                std::shared_ptr<Polyline> polyline = std::dynamic_pointer_cast<Polyline>(obj);
                QPen pen;
                pen.setColor(QColor(QString::fromStdString(polyline->stroke)));
                pen.setWidth(polyline->stroke_width);
                pen.setStyle(Qt::SolidLine);
                pen.setJoinStyle(Qt::PenJoinStyle(polyline->stroke_linejoin == "round" ? Qt::RoundJoin : polyline->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                                                                                                              : Qt::MiterJoin));
                if (polyline->stroke_dasharray != "none")
                {
                    QString dashArrayStr = QString::fromStdString(polyline->stroke_dasharray);
                    QStringList dashList = dashArrayStr.split(",");
                    QVector<qreal> dashes;
                    for (const QString &dash : dashList)
                    {
                        dashes.append(dash.toDouble());
                    }
                    pen.setDashPattern(dashes);
                }

                painter.setPen(pen);

                QPolygon polygon;
                for (const auto &[x, y] : polyline->points)
                {
                    polygon << QPoint(static_cast<int>(x), static_cast<int>(y));
                }

                painter.drawPolyline(polygon);
            }
            else if (obj->type() == "polygon")
            {
                // Handle polygon drawing
                std::shared_ptr<Polygon> polygon = std::dynamic_pointer_cast<Polygon>(obj);
                QPen pen;
                pen.setColor(QColor(QString::fromStdString(polygon->stroke)));
                pen.setWidth(polygon->stroke_width);
                pen.setStyle(Qt::SolidLine);
                pen.setJoinStyle(Qt::PenJoinStyle(polygon->stroke_linejoin == "round" ? Qt::RoundJoin : polygon->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                                                                                                            : Qt::MiterJoin));
                if (polygon->stroke_dasharray != "none")
                {
                    QString dashArrayStr = QString::fromStdString(polygon->stroke_dasharray);
                    QStringList dashList = dashArrayStr.split(",");
                    QVector<qreal> dashes;
                    for (const QString &dash : dashList)
                    {
                        dashes.append(dash.toDouble());
                    }
                    pen.setDashPattern(dashes);
                }

                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(QString::fromStdString(polygon->fill))));

                QPolygon polygonPoints;
                for (const auto &[x, y] : polygon->points)
                {
                    polygonPoints << QPoint(static_cast<int>(x), static_cast<int>(y));
                }

                painter.drawPolygon(polygonPoints);
            }
            else if (obj->type() == "text")
            {
                std::shared_ptr<Text> text = std::dynamic_pointer_cast<Text>(obj);

                QPen pen;
                pen.setColor(QColor(QString::fromStdString(text->fill)));
                painter.setPen(pen);

                QFont font;
                font.setPointSizeF(text->font_size);
                painter.setFont(font);

                painter.drawText(QPointF(text->x, text->y), QString::fromStdString(text->content));
            }
        }
    }
};

#endif