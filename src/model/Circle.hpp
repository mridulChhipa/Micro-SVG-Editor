#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "GraphicsObject.hpp"
#include <sstream>

class Circle : public GraphicsObject
{
public:
    float x{0.0f};
    float y{0.0f};
    float r{0.0f};

    Circle() = default;
    Circle(const std::unordered_map<std::string, std::string> &attributes)
    {
        initialiseStyle(attributes);
        x = std::stof(attributes.at("cx"));
        y = std::stof(attributes.at("cy"));
        r = std::stof(attributes.at("r"));
    }

    ~Circle() = default;

    std::string toSVG() const override
    {
        std::ostringstream ss;
        ss << "<circle cx=\"" << x << "\" cy=\"" << y
           << "\" r=\"" << r << "\"" << printStyle() << " />";
        return ss.str();
    }

    std::string type() const override { return "circle"; }
};
#endif