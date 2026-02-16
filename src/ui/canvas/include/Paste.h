#ifndef PASTE_HPP
#define PASTE_HPP

inline void Canvas::paste()
{
    if (clipboard_shape != nullptr)
    {
        undo_stack.push_back(svg.clone());
        redo_stack.clear();

        // Paste object by slightly shifting them so that they don't overlap
        if (clipboard_shape->type() == "text")
        {
            auto textShape = std::dynamic_pointer_cast<Text>(clipboard_shape->clone());
            textShape->x += 50;
            textShape->y += 50;
            svg.add(textShape);
        }
        else if (clipboard_shape->type() == "path")
        {
            auto pathShape = std::dynamic_pointer_cast<Path>(clipboard_shape->clone());
            for (auto &[cmd, points] : pathShape->commands)
                for (auto &[x, y] : points)
                {
                    x += 50;
                    y += 50;
                }

            svg.add(pathShape);
        }
        else if (clipboard_shape->type() == "rect")
        {
            auto rectShape = std::dynamic_pointer_cast<Rect>(clipboard_shape->clone());
            rectShape->x += 50;
            rectShape->y += 50;
            svg.add(rectShape);
        }
        else if (clipboard_shape->type() == "circle")
        {
            auto circleShape = std::dynamic_pointer_cast<Circle>(clipboard_shape->clone());
            circleShape->x += 50; // Offset the pasted shape
            circleShape->y += 50;
            svg.add(circleShape);
        }
        else if (clipboard_shape->type() == "line")
        {
            auto lineShape = std::dynamic_pointer_cast<Line>(clipboard_shape->clone());
            lineShape->x1 += 50; // Offset the pasted shape
            lineShape->y1 += 50;
            lineShape->x2 += 50;
            lineShape->y2 += 50;
            svg.add(lineShape);
        }
        else if (clipboard_shape->type() == "polyline")
        {
            auto polylineShape = std::dynamic_pointer_cast<Polyline>(clipboard_shape->clone());
            for (auto &[x, y] : polylineShape->points)
            {
                x += 50;
                y += 50;
            }
            svg.add(polylineShape);
        }
        else if (clipboard_shape->type() == "hexagon")
        {
            auto hexagonShape = std::dynamic_pointer_cast<Hexagon>(clipboard_shape->clone());
            hexagonShape->x += 50;
            hexagonShape->y += 50;
            for (auto &[x, y] : hexagonShape->points)
            {
                x += 50;
                y += 50;
            }
            svg.add(hexagonShape);
        }

        selected_shape = nullptr;
        update();
    }
}

#endif