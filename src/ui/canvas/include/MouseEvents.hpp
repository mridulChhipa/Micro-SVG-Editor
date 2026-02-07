#ifndef MOUSE_EVENTS_HPP
#define MOUSE_EVENTS_HPP

inline void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (isPerformingUndoRedo)
        return;
    if (currentTool != "")
    {
        std::cout << "Adding shape: " << currentTool.toStdString() << std::endl;
        addShapeToCanvas(currentTool.toStdString());
        update();
    }

    if (selected_shape)
    {
        isPerformingUndoRedo = false;
        QPainterPath selPath;
        QPen selPen;
        createObject(selected_shape, selPath, selPen);
        QTransform transform = findTransform(selected_shape);

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

            isPerformingUndoRedo = true;

            undoRedoSVG = svg.toSVG();
            undoStackTemp.push_back(svg.clone()); // Save current state for potential undo

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
    isPerformingUndoRedo = false;

    for (const GraphicsObjectPtr &obj : svg.objects | std::views::reverse)
    {
        QPainterPath path;
        QPen pen;
        createObject(obj, path, pen);
        QTransform transform = findTransform(obj);

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
                undoStackTemp.push_back(svg.clone());
                undoRedoSVG = svg.toSVG();
                isPerformingUndoRedo = true;
                break;
            }
        }
        else if (path.boundingRect().contains(localPoint))
        {
            dragging = true;
            selected_shape = obj;
            last_point = event->pos();
            undoStackTemp.push_back(svg.clone());
            undoRedoSVG = svg.toSVG();
            isPerformingUndoRedo = true;
            break;
        }
    }
}

inline void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton) || !selected_shape || (!dragging && !is_resizing))
        return;

    QPoint delta = event->pos() - last_point;
    last_point = event->pos();

    QTransform transform = findTransform(selected_shape);
    if (!transform.isIdentity())
    {
        QTransform inverted = transform.inverted();
        QPointF transformedDelta = inverted.map(QPointF(delta)) - inverted.map(QPointF(0, 0));
        delta = transformedDelta.toPoint();
    }

    if (!is_resizing)
        applyDrag(delta);
    else
        applyResize(delta, curr_handle);
    update();
}

inline void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    dragging = false;
    is_resizing = false;
    curr_handle = HandleType::None;

    if (isPerformingUndoRedo)
    {
        // std::cout << undoRedoSVG << std::endl;
        // std::cout << "Pushing to undo stack Check 1 " << (undoStackTemp.back().toSVG() == svg.toSVG() ? "No changes" : "Changes detected") << std::endl;
        // std::cout << "Pushing to undo stack Check 2 " << (undoStackTemp.back().toSVG() == undoRedoSVG ? "No changes" : "Changes detected") << std::endl;
        // std::cout << "Pushing to undo stack Check 3 " << (undoRedoSVG == svg.toSVG() ? "No changes" : "Changes detected") << std::endl;
        undoStack.push_back(undoStackTemp.back().clone());
        undoStackTemp.clear();
        redoStack.clear();
        undoRedoSVG.clear();
        isPerformingUndoRedo = false;
    }
}

#endif