#include "src/model/graphics_object.h"

#include <sstream>

namespace micro_svg_editor {

void GraphicsObject::InitialiseStyle(
    const std::unordered_map<std::string, std::string>& attributes) {
  id_ = attributes.count("id") ? attributes.at("id") : "";
  class_name_ = attributes.count("class") ? attributes.at("class") : "";
  style_ = attributes.count("style") ? attributes.at("style") : "";
  fill_ = attributes.count("fill") ? attributes.at("fill") : "red";
  fill_opacity_ = attributes.count("fill-opacity")
                      ? std::stof(attributes.at("fill-opacity"))
                      : 1.0f;
  stroke_ = attributes.count("stroke") ? attributes.at("stroke") : "black";
  stroke_width_ = attributes.count("stroke-width")
                      ? std::stof(attributes.at("stroke-width"))
                      : 1.0f;
  stroke_opacity_ = attributes.count("stroke-opacity")
                        ? std::stof(attributes.at("stroke-opacity"))
                        : 1.0f;
  stroke_linecap_ = attributes.count("stroke-linecap")
                        ? attributes.at("stroke-linecap")
                        : "butt";
  stroke_linejoin_ = attributes.count("stroke-linejoin")
                         ? attributes.at("stroke-linejoin")
                         : "miter";
  stroke_dasharray_ = attributes.count("stroke-dasharray")
                          ? attributes.at("stroke-dasharray")
                          : "none";
  transform_ = attributes.count("transform") ? attributes.at("transform") : "";
  opacity_ =
      attributes.count("opacity") ? std::stof(attributes.at("opacity")) : 1.0f;
  visibility_ =
      attributes.count("visibility") ? attributes.at("visibility") : "visible";
}

std::string GraphicsObject::PrintStyle() const {
  std::ostringstream ss;
  ss << " fill=\"" << fill_ << "\" fill-opacity=\"" << fill_opacity_
     << "\" stroke=\"" << stroke_ << "\" stroke-width=\"" << stroke_width_
     << "\" stroke-opacity=\"" << stroke_opacity_ << "\" stroke-linecap=\""
     << stroke_linecap_ << "\" stroke-linejoin=\"" << stroke_linejoin_
     << "\" stroke-dasharray=\"" << stroke_dasharray_ << "\" transform=\""
     << transform_ << "\" opacity=\"" << opacity_ << "\" visibility=\""
     << visibility_ << "\"";
  return ss.str();
}

}  // namespace micro_svg_editor
