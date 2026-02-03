#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QResizeEvent>

#include "../../model/SVG.hpp"
#include "../../model/Rect.hpp"
#include "../../model/Circle.hpp"

class Canvas : public QGraphicsView
{
    Q_OBJECT

public:
    Canvas(QWidget *parent) : QGraphicsView(parent)
    {
        scene = new QGraphicsScene(this);
        setScene(scene);

        setStyleSheet("background: transparent; border: none;");

        setBackgroundBrush(QColor("#303030"));

        // artboard = scene->addRect(0, 0, 800, 600);
        // artboard->setBrush(Qt::white);
        // artboard->setPen(Qt::NoPen);

        setRenderHint(QPainter::Antialiasing);
        setAlignment(Qt::AlignCenter);
        setDragMode(QGraphicsView::ScrollHandDrag);
    }

    // Need to render SVG objects here
    void updateCanvas(const SVG &newSVG)
    {
        svg = newSVG;
        scene->clear();
        // artboard = scene->addRect(0, 0, svg.width, svg.height);
        // artboard->setBrush(Qt::white);
        // artboard->setPen(Qt::NoPen);
        // Render each SVG object onto the scene
        for (const auto &obj : svg.objects)
        {
            if (obj->type() == "rect")
            {
                auto rectObj = std::dynamic_pointer_cast<Rect>(obj);
                if (rectObj)
                {
                    QGraphicsRectItem *rectItem = scene->addRoundedRect(rectObj->x, rectObj->y, rectObj->width, rectObj->height,);
                    rectItem->setBrush(QBrush(QColor(QString::fromStdString(rectObj->fill))));
                    rectItem->setPen(Qt::NoPen);
                }
            }
            // Handle other SVG object types (e.g., Circle) similarly
        }

        // fitInView(artboard, Qt::KeepAspectRatio);
    }

protected:
private:
    SVG svg;

    QGraphicsScene *scene;
    QGraphicsRectItem *artboard;
};

#endif