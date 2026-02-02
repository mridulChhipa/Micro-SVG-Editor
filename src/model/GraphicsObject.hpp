#ifndef GRAPHICS_OBJECT_HPP
#define GRAPHICS_OBJECT_HPP

#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <utility>

class GraphicsObject
{
public:
    std::string id;
    std::string class_name;
    std::string style;
    std::string fill;
    float fill_opacity;
    std::string stroke;
    float stroke_width;
    float stroke_opacity;
    std::string stroke_linecap;
    std::string stroke_linejoin;
    std::string stroke_dasharray;
    std::string transform;
    float opacity;
    std::string visibility;

    ~GraphicsObject() = default;
    virtual std::string toSVG() const = 0;
    virtual std::string type() const = 0;

    void initialiseStyle(const std::unordered_map<std::string, std::string> &attributes)
    {
        id = (attributes.count("id") ? attributes.at("id") : "");
        class_name = (attributes.count("class") ? attributes.at("class") : "");
        style = (attributes.count("style") ? attributes.at("style") : "");
        fill = (attributes.count("fill") ? attributes.at("fill") : "none");
        fill_opacity = (attributes.count("fill-opacity") ? std::stof(attributes.at("fill-opacity")) : 1.0f);
        stroke = (attributes.count("stroke") ? attributes.at("stroke") : "black");
        stroke_width = (attributes.count("stroke-width") ? std::stof(attributes.at("stroke-width")) : 1.0f);
        stroke_opacity = (attributes.count("stroke-opacity") ? std::stof(attributes.at("stroke-opacity")) : 1.0f);
        stroke_linecap = (attributes.count("stroke-linecap") ? attributes.at("stroke-linecap") : "butt");
        stroke_linejoin = (attributes.count("stroke-linejoin") ? attributes.at("stroke-linejoin") : "miter");
        stroke_dasharray = (attributes.count("stroke-dasharray") ? attributes.at("stroke-dasharray") : "none");
        transform = (attributes.count("transform") ? attributes.at("transform") : "");
        opacity = (attributes.count("opacity") ? std::stof(attributes.at("opacity")) : 1.0f);
        visibility = (attributes.count("visibility") ? attributes.at("visibility") : "visible");
    }

    std::string printStyle() const
    {
        std::ostringstream ss;
        ss << " fill=\"" << fill << "\" fill-opacity=\"" << fill_opacity << "\" stroke=\"" << stroke
           << "\" stroke-width=\"" << stroke_width << "\" stroke-opacity=\"" << stroke_opacity << "\""
           << " stroke-linecap=\"" << stroke_linecap << "\" stroke-linejoin=\"" << stroke_linejoin << "\""
           << " stroke-dasharray=\"" << stroke_dasharray << "\" transform=\"" << transform << "\""
           << " opacity=\"" << opacity << "\" visibility=\"" << visibility << "\"";
        return ss.str();
    }
};

using GraphicsObjectPtr = std::shared_ptr<GraphicsObject>;

#endif