#include "src/model/Hexagon.h"

Hexagon::Hexagon(const std::unordered_map<std::string, std::string> &attributes)
{
  initialiseStyle(attributes);
  if (attributes.count("points"))
  {
    std::istringstream ss(attributes.at("points"));
    std::string point;
    points.clear();
    while (std::getline(ss, point, ' '))
    {
      std::istringstream point_ss(point);
      std::string x_str, y_str;
      if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str))
      {
        points.emplace_back(std::stod(x_str), std::stod(y_str));
      }
    }

    // iterate over C(6, 2) pairs of points to find the minimum distance as side_length
    side_length = std::numeric_limits<double>::max();
    for (int i = 0; i < 6; i++)
    {
      for (int j = i + 1; j < 6; j++)
      {
        double dx = points[i].first - points[j].first;
        double dy = points[i].second - points[j].second;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist < side_length)
        {
          side_length = dist;
        }
      }

      x += points[i].first;
      y += points[i].second;
    }

    x /= 6.0;
    y /= 6.0;
  }
}

std::string Hexagon::type() const { return "hexagon"; }

std::string Hexagon::toSVG() const
{
  std::ostringstream ss;
  ss << "<polygon points=\"";
  for (const auto &point : points)
  {
    ss << point.first << "," << point.second << " ";
  }
  ss << "\" " << printStyle() << "/>";
  return ss.str();
}

std::shared_ptr<GraphicsObject> Hexagon::clone() const
{
  auto copy = std::make_shared<Hexagon>();
  copy->x = x;
  copy->y = y;
  copy->side_length = side_length;
  copy->points = points;
  copy->id = id;
  copy->class_name = class_name;
  copy->style = style;
  copy->fill = fill;
  copy->fill_opacity = fill_opacity;
  copy->stroke = stroke;
  copy->stroke_width = stroke_width;
  copy->stroke_opacity = stroke_opacity;
  copy->stroke_linecap = stroke_linecap;
  copy->stroke_linejoin = stroke_linejoin;
  copy->stroke_dasharray = stroke_dasharray;
  copy->transform = transform;
  copy->opacity = opacity;
  copy->visibility = visibility;
  return copy;
}
