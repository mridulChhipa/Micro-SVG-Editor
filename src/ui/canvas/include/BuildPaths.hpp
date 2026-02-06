#ifndef BUILD_PATHS_HPP
#define BUILD_PATHS_HPP

template <typename PointContainer>
inline void Canvas::buildPointPath(QPainterPath &path, const PointContainer &points, bool close)
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

inline void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Rect> &s)
{
    path.addRoundedRect(QRectF(s->x, s->y, s->width, s->height), s->rx, s->ry);
}

inline void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Circle> &s)
{
    path.addEllipse(QPointF(s->x, s->y), s->r, s->r);
}

inline void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Line> &s)
{
    path.moveTo(QPointF(s->x1, s->y1));
    path.lineTo(QPointF(s->x2, s->y2));
}

inline void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Polyline> &s)
{
    buildPointPath(path, s->points, false);
}

inline void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Hexagon> &s)
{
    buildPointPath(path, s->points, true);
}

inline void Canvas::buildShapePath(QPainterPath &path, const std::shared_ptr<Text> &s)
{
    QFont font;
    font.setPointSizeF(s->font_size);
    font.setFamily(QString::fromStdString(s->font_family));
    path.addText(QPointF(s->x, s->y), font, QString::fromStdString(s->content));
}

#endif