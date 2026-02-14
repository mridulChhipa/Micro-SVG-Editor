#ifndef MOUSE_RELEASE_HPP
#define MOUSE_RELEASE_HPP

inline void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    dragging = false;
    is_resizing = false;
    curr_handle = HandleType::None;

    if (event->button() == Qt::LeftButton && is_drawing && currentTool == "Freehand")
    {
        SVG prevSVG = svg.clone();

        is_drawing = false;
        std::shared_ptr<Path> newPath = std::make_shared<Path>();
        for (int i = 0; i < current_path.elementCount(); ++i)
        {
            const QPainterPath::Element &e = current_path.elementAt(i);

            if (e.isMoveTo())
            {
                std::vector<std::pair<float, float>> points;
                points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

                newPath->commands.push_back({'M', points});
            }
            else if (e.isLineTo())
            {
                std::vector<std::pair<float, float>> points;
                points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

                newPath->commands.push_back({'L', points});
            }
            else if (e.isCurveTo())
            {
                if (i + 2 < current_path.elementCount())
                {
                    std::vector<std::pair<float, float>> points;

                    points.push_back({static_cast<float>(e.x), static_cast<float>(e.y)});

                    const QPainterPath::Element &cp2 = current_path.elementAt(i + 1);
                    points.push_back({static_cast<float>(cp2.x), static_cast<float>(cp2.y)});

                    const QPainterPath::Element &end = current_path.elementAt(i + 2);
                    points.push_back({static_cast<float>(end.x), static_cast<float>(end.y)});

                    newPath->commands.push_back({'C', points});

                    i += 2;
                }
            }
        }

        svg.objects.push_back(newPath);
        current_path = QPainterPath();
        undoStack.push_back(prevSVG);
        redoStack.clear();
        update();
    }

    if (isPerformingUndoRedo)
    {
        undoStack.push_back(undoStackTemp.back().clone());
        undoStackTemp.clear();
        redoStack.clear();
        undoRedoSVG.clear();
        isPerformingUndoRedo = false;
    }
}

#endif