#ifndef RECT_HPP
#define RECT_HPP

#include "GraphicsObject.hpp"
#include <sstream>

class Rect : public GraphicsObject
{
public:
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    float rx{0.0f}; // rx
    float ry{0.0f}; // ry

    Rect() = default;
    Rect(const std::unordered_map<std::string, std::string> &attributes)
    {
        initialiseStyle(attributes);
        x = attributes.count("x") ? std::stof(attributes.at("x")) : 0.0f;
        y = attributes.count("y") ? std::stof(attributes.at("y")) : 0.0f;
        width = attributes.count("width") ? std::stof(attributes.at("width")) : 0.0f;
        height = attributes.count("height") ? std::stof(attributes.at("height")) : 0.0f;
        rx = attributes.count("rx") ? std::stof(attributes.at("rx")) : 0.0f;
        ry = attributes.count("ry") ? std::stof(attributes.at("ry")) : 0.0f;
    }

    ~Rect() = default;

    std::string type() const override
    {
        return "rect";
    }

    std::string toSVG() const override
    {
        std::ostringstream ss;
        ss << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << width
           << "\" height=\"" << height << "\"";
        if (rx > 0.0f)
            ss << " rx=\"" << rx << "\"";
        if (ry > 0.0f)
            ss << " ry=\"" << ry << "\"";
        ss << printStyle() << " />";
        return ss.str();
    }
};

#endif