#include "src/model/GraphicsObject.h"

#include <sstream>

void GraphicsObject::InitialiseStyle(
    const std::unordered_map<std::string, std::string>& attributes) {
  id = attributes.count("id") ? attributes.at("id") : "";
  class_name = attributes.count("class") ? attributes.at("class") : "";
  style = attributes.count("style") ? attributes.at("style") : "";
  fill = attributes.count("fill") ? attributes.at("fill") : "red";
  fill_opacity = attributes.count("fill-opacity")
                     ? std::stof(attributes.at("fill-opacity"))
                     : 1.0f;
  stroke = attributes.count("stroke") ? attributes.at("stroke") : "black";
  stroke_width = attributes.count("stroke-width")
                     ? std::stof(attributes.at("stroke-width"))
                     : 1.0f;
  stroke_opacity = attributes.count("stroke-opacity")
                       ? std::stof(attributes.at("stroke-opacity"))
                       : 1.0f;
  stroke_linecap = attributes.count("stroke-linecap")
                       ? attributes.at("stroke-linecap")
                       : "butt";
  stroke_linejoin = attributes.count("stroke-linejoin")
                        ? attributes.at("stroke-linejoin")
                        : "miter";
  stroke_dasharray = attributes.count("stroke-dasharray")
                         ? attributes.at("stroke-dasharray")
                         : "none";
  transform = attributes.count("transform") ? attributes.at("transform") : "";
  opacity =
      attributes.count("opacity") ? std::stof(attributes.at("opacity")) : 1.0f;
  visibility =
      attributes.count("visibility") ? attributes.at("visibility") : "visible";
}

std::string GraphicsObject::PrintStyle() const {
  std::ostringstream ss;
  ss << " fill=\"" << fill << "\" fill-opacity=\"" << fill_opacity
     << "\" stroke=\"" << stroke << "\" stroke-width=\"" << stroke_width
     << "\" stroke-opacity=\"" << stroke_opacity << "\" stroke-linecap=\""
     << stroke_linecap << "\" stroke-linejoin=\"" << stroke_linejoin
     << "\" stroke-dasharray=\"" << stroke_dasharray << "\" transform=\""
     << transform << "\" opacity=\"" << opacity << "\" visibility=\""
     << visibility << "\"";
  return ss.str();
}
