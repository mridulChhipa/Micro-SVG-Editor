#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "GraphicsObject.hpp"
#include <sstream>

class Polygon : public GraphicsObject
{
public:
    std::vector<std::pair<float, float>> points;

    Polygon() = default;
    Polygon(const std::unordered_map<std::string, std::string> &attributes)
    {
        initialiseStyle(attributes);
        if (attributes.count("points"))
        {
            std::istringstream ss(attributes.at("points"));
            std::string point;
            while (std::getline(ss, point, ' '))
            {
                int commaPos = point.find(',');
                if (commaPos != static_cast<int>(std::string::npos))
                {
                    float x = std::stof(point.substr(0, commaPos));
                    float y = std::stof(point.substr(commaPos + 1));
                    points.emplace_back(x, y);
                }
            }
        }
    }

    ~Polygon() = default;

    std::string type() const override { return "polygon"; }

    std::string toSVG() const override
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
};

#endif