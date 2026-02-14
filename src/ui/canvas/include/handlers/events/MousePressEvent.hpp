#ifndef MOUSE_PRESS_HPP
#define MOUSE_PRESS_HPP

inline void Canvas::mousePressEvent(QMouseEvent *event)
{
    QPointF canvasPoint = toCanvasCoordinates(event->pos());
    if (isPerformingUndoRedo)
        return;
    if (currentTool != "" && currentTool != "Freehand")
    {
        std::cout << "Adding shape: " << currentTool.toStdString() << std::endl;
        addShapeToCanvas(currentTool.toStdString(), toCanvasCoordinates(event->pos()));
        update();
    }

    if (event->button() == Qt::LeftButton && currentTool == "Freehand")
    {
        current_path = QPainterPath();
        current_path.moveTo(canvasPoint);
        is_drawing = true;
        return;
    }

    if (selected_shape)
    {
        isPerformingUndoRedo = false;
        QPainterPath selPath;
        QPen selPen;
        createObject(selected_shape, selPath, selPen);
        QTransform transform = findTransform(selected_shape);

        QPointF localPoint = toCanvasCoordinates(event->pos());
        if (!transform.isIdentity()) { QTransform inverted = transform.inverted(); localPoint = inverted.map(localPoint); }

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
            last_point = toCanvasCoordinates(event->pos()).toPoint();
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

        QPointF localPoint = toCanvasCoordinates(event->pos());
        if (!transform.isIdentity()) { QTransform inverted = transform.inverted(); localPoint = inverted.map(localPoint); }

        if (obj->type() == "line" || obj->type() == "polyline")
        {
            if (path.boundingRect().adjusted(-adjust, -adjust, adjust, adjust).contains(localPoint))
            {
                dragging = true;
                selected_shape = obj;
                last_point = toCanvasCoordinates(event->pos()).toPoint();
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
            last_point = toCanvasCoordinates(event->pos()).toPoint();
            undoStackTemp.push_back(svg.clone());
            undoRedoSVG = svg.toSVG();
            isPerformingUndoRedo = true;
            break;
        }
    }
}

#endif