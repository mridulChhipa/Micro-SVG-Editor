#ifndef ADD_SHAPES_HPP
#define ADD_SHAPES_HPP

inline void Canvas::addShapeToCanvas(const std::string shapeType, QPointF location)
{
    bool shapeAdded = false;
    SVG prevState = svg; // Save current state for undo
    if (shapeType == "Rectangle")
    {
        auto rect = std::make_shared<Rect>();
        rect->x = location.x();
        rect->y = location.y();
        rect->width = 100;
        rect->height = 100;
        svg.objects.push_back(rect);
        shapeAdded = true;
    }
    else if (shapeType == "Circle")
    {
        auto circle = std::make_shared<Circle>();
        circle->x = location.x();
        circle->y = location.y();
        circle->r = 50;
        svg.objects.push_back(circle);
        shapeAdded = true;
    }
    else if (shapeType == "Line")
    {
        auto line = std::make_shared<Line>();
        line->x1 = location.x();
        line->y1 = location.y();
        line->x2 = line->x1 + 100;
        line->y2 = line->y1 + 100;
        line->stroke = "blue";
        line->stroke_width = 5.0f;
        svg.objects.push_back(line);
        shapeAdded = true;
    }
    else if (shapeType == "Polyline")
    {
        auto polyline = std::make_shared<Polyline>();
        polyline->points = {{350, 350}, {400, 400}, {450, 350}};
        svg.objects.push_back(polyline);
        shapeAdded = true;
    }
    else if (shapeType == "Hexagon")
    {
        auto hexagon = std::make_shared<Hexagon>();
        hexagon->x = location.x();
        hexagon->y = location.y();
        hexagon->side_length = 50;
        
        for (int i = 0; i < 6; i++)
        {
            double angle = M_PI / 3.0 * i;
            double px = hexagon->x + hexagon->side_length * std::cos(angle);
            double py = hexagon->y + hexagon->side_length * std::sin(angle);
            hexagon->points.emplace_back(px, py);
        }
        svg.objects.push_back(hexagon);
        shapeAdded = true;
    }
    else if (shapeType == "Text")
    {
        auto text = std::make_shared<Text>();
        text->x = location.x();
        text->y = location.y();
        text->content = "Hello, World!";
        svg.objects.push_back(text);
        shapeAdded = true;
    }

    if (shapeAdded)
    {
        undoStack.push_back(prevState);
        redoStack.clear();
    }
}

#endif