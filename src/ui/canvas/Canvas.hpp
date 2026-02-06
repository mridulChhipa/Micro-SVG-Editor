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

#include "../../model/SVG.hpp"
#include "../../model/Rect.hpp"
#include "../../model/Circle.hpp"
#include "../../model/Line.hpp"
#include "../../model/Polyline.hpp"
#include "../../model/Hexagon.hpp"
#include "../../model/Text.hpp"
#include "HandleType.hpp"

using ShapeVariant = std::variant<
    std::shared_ptr<Rect>,
    std::shared_ptr<Circle>,
    std::shared_ptr<Line>,
    std::shared_ptr<Polyline>,
    std::shared_ptr<Hexagon>,
    std::shared_ptr<Text>>;

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
        if (selected_shape)
        {
            QPainterPath selPath;
            QPen selPen;
            createObject(selected_shape, selPath, selPen);
            QTransform transform = getObjectTransform(selected_shape);

            QPointF localPoint = event->pos();
            if (!transform.isIdentity())
            {
                QTransform inverted = transform.inverted();
                localPoint = inverted.map(localPoint);
            }

            QRectF boundingRect = selPath.boundingRect().adjusted(-adjust, -adjust, adjust, adjust);
            HandleType handle = hitTestHandles(boundingRect, localPoint);

            if (handle != HandleType::None)
            {
                is_resizing = true;
                curr_handle = handle;
                start_rect = boundingRect;
                last_point = event->pos();
                dragging = false;
                return;
            }
        }

        selected_shape = nullptr;
        dragging = false;
        is_resizing = false;
        curr_handle = HandleType::None;

        for (const GraphicsObjectPtr &obj : svg.objects | std::views::reverse)
        {
            QPainterPath path;
            QPen pen;
            createObject(obj, path, pen);
            QTransform transform = getObjectTransform(obj);

            QPointF localPoint = event->pos();
            if (!transform.isIdentity())
            {
                QTransform inverted = transform.inverted();
                localPoint = inverted.map(localPoint);
            }

            if (obj->type() == "line" || obj->type() == "polyline")
            {
                if (path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust).contains(localPoint))
                {
                    dragging = true;
                    selected_shape = obj;
                    last_point = event->pos();
                    break;
                }
            }
            else if (path.boundingRect().contains(localPoint))
            {
                dragging = true;
                selected_shape = obj;
                last_point = event->pos();
                break;
            }
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (!(event->buttons() & Qt::LeftButton) || !selected_shape || (!dragging && !is_resizing))
            return;

        QPoint delta = event->pos() - last_point;
        last_point = event->pos();

        QTransform transform = getObjectTransform(selected_shape);
        if (!transform.isIdentity())
        {
            QTransform inverted = transform.inverted();
            QPointF transformedDelta = inverted.map(QPointF(delta)) - inverted.map(QPointF(0, 0));
            delta = transformedDelta.toPoint();
        }

        if (!is_resizing)
        {
            applyDrag(delta);
        }
        else
        {
            applyResize(delta, curr_handle);
        }
        update();
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);
        dragging = false;
        is_resizing = false;
        curr_handle = HandleType::None;
    }

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

    void applyDrag(const QPoint &delta)
    {
        auto shape = toShapeVariant(selected_shape);
        if (!shape)
            return;

        std::visit([&delta](auto &&s)
                   {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>)
            {
                s->x += delta.x();
                s->y += delta.y();
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>)
            {
                s->x += delta.x();
                s->y += delta.y();
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>)
            {
                s->x1 += delta.x();
                s->y1 += delta.y();
                s->x2 += delta.x();
                s->y2 += delta.y();
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Polyline>> ||
                               std::is_same_v<T, std::shared_ptr<Hexagon>>)
            {
                for (auto &[x, y] : s->points)
                {
                    x += delta.x();
                    y += delta.y();
                }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>)
            {
                s->x += delta.x();
                s->y += delta.y();
            } }, *shape);
    }

    void applyResize(const QPoint &delta, HandleType handle)
    {
        auto shape = toShapeVariant(selected_shape);
        if (!shape)
            return;

        bool affectsLeft = (handle == HandleType::TopLeft || handle == HandleType::MiddleLeft || handle == HandleType::BottomLeft);
        bool affectsRight = (handle == HandleType::TopRight || handle == HandleType::MiddleRight || handle == HandleType::BottomRight);
        bool affectsTop = (handle == HandleType::TopLeft || handle == HandleType::TopCenter || handle == HandleType::TopRight);
        bool affectsBottom = (handle == HandleType::BottomLeft || handle == HandleType::BottomCenter || handle == HandleType::BottomRight);

        int dx = delta.x();
        int dy = delta.y();

        std::visit([&](auto &&s)
                   {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<Rect>>)
            {
                if (affectsLeft)   { s->x += dx; s->width  -= dx; }
                if (affectsRight)  { s->width  += dx; }
                if (affectsTop)    { s->y += dy; s->height -= dy; }
                if (affectsBottom) { s->height += dy; }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Circle>>)
            {
                double drX = 0, drY = 0;
                if (affectsLeft)        drX = -dx;
                else if (affectsRight)  drX = dx;
                if (affectsTop)         drY = -dy;
                else if (affectsBottom) drY = dy;

                double dr = std::abs(drX) > std::abs(drY) ? drX : drY;

                s->r = std::max(1.0, s->r + dr / 2.0);
                if (affectsLeft)   s->x += dx / 2.0;
                if (affectsRight)  s->x += dx / 2.0;
                if (affectsTop)    s->y += dy / 2.0;
                if (affectsBottom) s->y += dy / 2.0;
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Line>>)
            {
                if (affectsLeft || affectsTop)
                {
                    s->x1 += dx;
                    s->y1 += dy;
                }
                if (affectsRight || affectsBottom)
                {
                    s->x2 += dx;
                    s->y2 += dy;
                }
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Hexagon>>)
            {
                if (s->points.empty()) return;

                double minX = s->points[0].first, maxX = s->points[0].first;
                double minY = s->points[0].second, maxY = s->points[0].second;
                for (const auto &[x, y] : s->points)
                {
                    minX = std::min(minX, x);
                    maxX = std::max(maxX, x);
                    minY = std::min(minY, y);
                    maxY = std::max(maxY, y);
                }

                double oldWidth = maxX - minX;
                double oldHeight = maxY - minY;
                double centerX = (minX + maxX) / 2.0;
                double centerY = (minY + maxY) / 2.0;

                // Calculate scale based on the dominant axis
                double scaleChange = 0;
                if (affectsLeft || affectsRight)
                {
                    if (affectsLeft)  scaleChange = -dx / (oldWidth / 2.0);
                    if (affectsRight) scaleChange = dx / (oldWidth / 2.0);
                }
                if (affectsTop || affectsBottom)
                {
                    double dyScale = 0;
                    if (affectsTop)    dyScale = -dy / (oldHeight / 2.0);
                    if (affectsBottom) dyScale = dy / (oldHeight / 2.0);
                    
                    if (std::abs(dyScale) > std::abs(scaleChange))
                        scaleChange = dyScale;
                }

                double scale = 1.0 + scaleChange;
                if (scale < 0.1) scale = 0.1; // Minimum size

                // Scale all points uniformly from center
                for (auto &[x, y] : s->points)
                {
                    x = centerX + (x - centerX) * scale;
                    y = centerY + (y - centerY) * scale;
                }

                // Translate center if edge handles are used
                double centerDx = 0, centerDy = 0;
                if (affectsLeft && !affectsRight)   centerDx = dx / 2.0;
                if (affectsRight && !affectsLeft)   centerDx = dx / 2.0;
                if (affectsTop && !affectsBottom)   centerDy = dy / 2.0;
                if (affectsBottom && !affectsTop)   centerDy = dy / 2.0;

                for (auto &[x, y] : s->points)
                {
                    x += centerDx;
                    y += centerDy;
                }
            }else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>)
            {
                if (affectsLeft)  s->x += dx;
                if (affectsTop)   s->y += dy;
                int sizeDelta = 0;
                if (affectsRight)       sizeDelta += dx;
                else if (affectsLeft)   sizeDelta -= dx;
                if (affectsBottom)      sizeDelta += dy;
                else if (affectsTop)    sizeDelta -= dy;
                s->font_size = std::max(1.0, s->font_size + sizeDelta / 10.0);
            } }, *shape);
    }

    void buildShapePath(QPainterPath &path, const std::shared_ptr<Rect> &s)
    {
        path.addRoundedRect(QRectF(s->x, s->y, s->width, s->height), s->rx, s->ry);
    }

    void buildShapePath(QPainterPath &path, const std::shared_ptr<Circle> &s)
    {
        path.addEllipse(QPointF(s->x, s->y), s->r, s->r);
    }

    void buildShapePath(QPainterPath &path, const std::shared_ptr<Line> &s)
    {
        path.moveTo(QPointF(s->x1, s->y1));
        path.lineTo(QPointF(s->x2, s->y2));
    }

    void buildShapePath(QPainterPath &path, const std::shared_ptr<Polyline> &s)
    {
        buildPointPath(path, s->points, false);
    }

    void buildShapePath(QPainterPath &path, const std::shared_ptr<Hexagon> &s)
    {
        buildPointPath(path, s->points, true);
    }

    void buildShapePath(QPainterPath &path, const std::shared_ptr<Text> &s)
    {
        QFont font;
        font.setPointSizeF(s->font_size);
        font.setFamily(QString::fromStdString(s->font_family));
        path.addText(QPointF(s->x, s->y), font, QString::fromStdString(s->content));
    }

    void createObject(const GraphicsObjectPtr &obj, QPainterPath &path, QPen &pen)
    {
        auto shape = toShapeVariant(obj);
        if (!shape)
            return;

        pen.setColor(QColor(QString::fromStdString(obj->stroke)));
        pen.setWidth(obj->stroke_width);
        pen.setStyle(Qt::SolidLine);
        pen.setJoinStyle(obj->stroke_linejoin == "round"   ? Qt::RoundJoin
                         : obj->stroke_linejoin == "bevel" ? Qt::BevelJoin
                                                           : Qt::MiterJoin);
        pen.setCapStyle(obj->stroke_linecap == "round"    ? Qt::RoundCap
                        : obj->stroke_linecap == "square" ? Qt::SquareCap
                                                          : Qt::FlatCap);
        applyDashArray(pen, obj->stroke_dasharray);

        std::visit([&](auto &&s)
                   { buildShapePath(path, s); }, *shape);
    }

    QTransform getObjectTransform(const GraphicsObjectPtr &obj) const
    {
        QTransform transform;
        if (obj->transform.empty())
            return transform;

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
        return transform;
    }

    static void applyDashArray(QPen &pen, const std::string &dasharray)
    {
        if (dasharray == "none" || dasharray.empty())
            return;
        QString dashArrayStr = QString::fromStdString(dasharray);
        QStringList dashList = dashArrayStr.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
        QVector<qreal> dashes;
        for (const QString &dash : dashList)
            dashes.append(dash.toDouble() / 5.0);
        if (!dashes.isEmpty() && dashes.size() % 2 == 0)
            pen.setDashPattern(dashes);
    }

    template <typename PointContainer>
    static void buildPointPath(QPainterPath &path, const PointContainer &points, bool close)
    {
        bool first = true;
        for (const auto &[x, y] : points)
        {
            if (first)
            {
                path.moveTo(x, y);
                first = false;
            }
            else
            {
                path.lineTo(x, y);
            }
        }
        if (close)
            path.closeSubpath();
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
        currentPen.setCapStyle(obj->stroke_linecap == "round"    ? Qt::RoundCap
                               : obj->stroke_linecap == "square" ? Qt::SquareCap
                                                                 : Qt::FlatCap);
        currentPen.setJoinStyle(obj->stroke_linejoin == "round"   ? Qt::RoundJoin
                                : obj->stroke_linejoin == "bevel" ? Qt::BevelJoin
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

            QTransform transform = getObjectTransform(obj);
            if (!transform.isIdentity())
            {
                painter.setTransform(transform, true);
                painter.drawPath(path);
                painter.setTransform(QTransform());
            }
            else
            {
                painter.drawPath(path);
            }

            if (obj == selected_shape)
            {
                painter.save();

                if (!transform.isIdentity())
                    painter.setTransform(transform, true);

                QPen handlePen(Qt::blue, 1, Qt::DashLine);
                painter.setPen(handlePen);
                painter.setBrush(Qt::NoBrush);
                QRectF selRect = path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust);
                painter.drawRect(selRect);

                painter.setPen(Qt::blue);
                painter.setBrush(Qt::white);
                for (int i = 0; i < 8; ++i)
                {
                    QRectF handleRect = renderHandle(selRect, static_cast<HandleType>(i));
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
        return QRectF(x - handle_size / 2, y - handle_size / 2, handle_size, handle_size);
    }

    HandleType hitTestHandles(const QRectF &obj, const QPointF &point)
    {
        for (int i = 0; i < 8; ++i)
        {
            QRectF handleRect = renderHandle(obj, static_cast<HandleType>(i));
            if (handleRect.contains(point))
                return static_cast<HandleType>(i);
        }
        return HandleType::None;
    }
};

#endif