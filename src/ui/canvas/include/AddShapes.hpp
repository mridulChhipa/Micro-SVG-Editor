#ifndef ADD_SHAPES_HPP
#define ADD_SHAPES_HPP

inline void Canvas::addShapeToCanvas(const std::string shapeType)
{
    if (shapeType == "Rectangle")
    {
        auto rect = std::make_shared<Rect>();
        rect->x = 50;
        rect->y = 50;
        rect->width = 100;
        rect->height = 100;
        svg.objects.push_back(rect);
    }
    else if (shapeType == "Circle")
    {
        auto circle = std::make_shared<Circle>();
        circle->x = 150;
        circle->y = 150;
        circle->r = 50;
        svg.objects.push_back(circle);
    }
    else if (shapeType == "Line")
    {
        auto line = std::make_shared<Line>();
        line->x1 = 500;
        line->y1 = 200;
        line->x2 = 300;
        line->y2 = 300;
        line->stroke = "blue";
        line->stroke_width = 5.0f;
        svg.objects.push_back(line);
    }
    else if (shapeType == "Polyline")
    {
        auto polyline = std::make_shared<Polyline>();
        polyline->points = {{350, 350}, {400, 400}, {450, 350}};
        svg.objects.push_back(polyline);
    }
    else if (shapeType == "Hexagon")
    {
        auto hexagon = std::make_shared<Hexagon>();
        hexagon->x = 500;
        hexagon->y = 500;
        hexagon->side_length = 50;
        // Calculate the 6 points of the hexagon
        for (int i = 0; i < 6; i++)
        {
            double angle = M_PI / 3.0 * i;
            double px = hexagon->x + hexagon->side_length * std::cos(angle);
            double py = hexagon->y + hexagon->side_length * std::sin(angle);
            hexagon->points.emplace_back(px, py);
        }
        svg.objects.push_back(hexagon);
    }
    else if (shapeType == "Text")
    {
        auto text = std::make_shared<Text>();
        text->x = 550;
        text->y = 550;
        text->content = "Hello, World!";
        svg.objects.push_back(text);
    }
}

#endif