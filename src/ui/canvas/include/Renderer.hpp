#ifndef RENDERER_HPP
#define RENDERER_HPP

inline void Canvas::createBrush(const GraphicsObjectPtr &obj, QPainter &painter)
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

inline void Canvas::drawSVG(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);

    for (const GraphicsObjectPtr &obj : svg.objects)
    {
        QPainterPath path;
        QPen pen;
        createObject(obj, path, pen);
        painter.setPen(pen);
        createBrush(obj, painter);

        QTransform transform = findTransform(obj);
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

#endif