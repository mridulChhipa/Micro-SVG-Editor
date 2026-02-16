#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include "GraphicsObject.h"
#include <sstream>

class Polyline : public GraphicsObject
{
public:
  std::vector<std::pair<float, float>> points;

  Polyline() = default;
  Polyline(const std::unordered_map<std::string, std::string> &attributes)
  {
    initialiseStyle(attributes);
    if (attributes.count("points"))
    {
      std::istringstream ss(attributes.at("points"));
      std::string point;
      while (std::getline(ss, point, ' '))
      {
        int comma_pos = point.find(',');
        if (comma_pos != static_cast<int>(std::string::npos))
        {
          float x = std::stof(point.substr(0, comma_pos));
          float y = std::stof(point.substr(comma_pos + 1));
          points.emplace_back(x, y);
        }
      }
    }
  }

  ~Polyline() = default;

  std::string type() const override { return "polyline"; }

  std::string toSVG() const override
  {
    std::ostringstream ss;
    ss << "<polyline points=\"";
    for (const auto &point : points)
    {
      ss << point.first << "," << point.second << " ";
    }
    ss << "\" " << printStyle() << "/>";
    return ss.str();
  }

  std::shared_ptr<GraphicsObject> clone() const override
  {
    auto copy = std::make_shared<Polyline>();
    copy->points = points;
    copy->fill = fill;
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
};

#endif