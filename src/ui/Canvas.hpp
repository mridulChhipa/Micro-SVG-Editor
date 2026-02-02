#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QResizeEvent>

#include "../model/SVG.hpp"
#include "../model/Rect.hpp"
#include "../model/Circle.hpp"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr) : QWidget(parent), drawing(false)
    {
        setAttribute(Qt::WA_StaticContents);
        image = QImage(size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::white);
    }

    void updateCanvas(const SVG &newSvg)
    {
        svg = newSvg;
        update();
    }

    void clearCanvas()
    {
        image.fill(Qt::white);
        update();
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        if (width() > image.width() || height() > image.height())
        {
            int newW = std::max(width(), image.width());
            int newH = std::max(height(), image.height());
            resizeImage(&image, QSize(newW, newH));
            update();
        }
        QWidget::resizeEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            lastPoint = event->pos();
            drawing = true;
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if ((event->buttons() & Qt::LeftButton) && drawing)
        {
            drawLineTo(event->pos());
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && drawing)
        {
            drawLineTo(event->pos());
            drawing = false;
        }
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        QRect dirtyRect = event->rect();
        painter.drawImage(dirtyRect, image, dirtyRect);
        drawSVG(painter);
    }

private:
    bool drawing;
    QPoint lastPoint;
    QImage image;
    SVG svg;

    void drawLineTo(const QPoint &endPoint)
    {
        QPainter painter(&image);
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastPoint, endPoint);

        int rad = 2;
        update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        lastPoint = endPoint;
    }

    void resizeImage(QImage *image, const QSize &newSize)
    {
        if (image->size() == newSize)
            return;
        QImage newImage(newSize, QImage::Format_RGB32);
        newImage.fill(Qt::white);
        QPainter painter(&newImage);
        painter.drawImage(QPoint(0, 0), *image);
        *image = newImage;
    }

    void drawSVG(QPainter &painter)
    {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setRenderHint(QPainter::Antialiasing);

        for (const auto &obj : svg.objects)
        {
            if (!obj)
                continue;

            if (obj->type() == "rectangle")
            {
                auto rect = std::dynamic_pointer_cast<Rect>(obj);
                painter.drawRect(QRectF(rect->x, rect->y, rect->width, rect->height));
            }
            else if (obj->type() == "circle")
            {
                auto circle = std::dynamic_pointer_cast<Circle>(obj);
                painter.drawEllipse(QPointF(circle->x, circle->y), circle->r, circle->r);
            }
        }
    }
};

#endif