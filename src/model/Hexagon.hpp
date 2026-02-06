#ifndef HEXAGON_HPP
#define HEXAGON_HPP

#include "GraphicsObject.hpp"
#include <sstream>

class Hexagon : public GraphicsObject
{
public:
    double x, y;
    double side_length;
    std::vector<std::pair<double, double>> points;

    Hexagon() = default;
    Hexagon(const std::unordered_map<std::string, std::string> &attributes)
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

    ~Hexagon() = default;

    std::string type() const override { return "hexagon"; }

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