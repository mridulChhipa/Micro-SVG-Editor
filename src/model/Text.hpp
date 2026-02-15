#ifndef TEXT_HPP
#define TEXT_HPP

#include "GraphicsObject.hpp"
#include <sstream>

class Text : public GraphicsObject
{
public:
  float x{0.0f};
  float y{0.0f};
  float dx{0.0f};
  float dy{0.0f};
  std::string font_family{"Arial"};
  float font_size{12.0f};
  std::string font_weight{"normal"};
  std::string text_anchor{"start"};

  std::string content{"Sample Text"};

  Text() = default;
  Text(const std::unordered_map<std::string, std::string> &attributes)
  {
    initialiseStyle(attributes);
    x = (attributes.count("x") ? std::stof(attributes.at("x")) : 0.0f);
    y = (attributes.count("y") ? std::stof(attributes.at("y")) : 0.0f);
    dx = (attributes.count("dx") ? std::stof(attributes.at("dx")) : 0.0f);
    dy = (attributes.count("dy") ? std::stof(attributes.at("dy")) : 0.0f);
    font_family = (attributes.count("font-family") ? attributes.at("font-family") : "Arial");
    font_size = (attributes.count("font-size") ? std::stof(attributes.at("font-size")) : 12.0f);
    font_weight = (attributes.count("font-weight") ? attributes.at("font-weight") : "normal");
    text_anchor = (attributes.count("text-anchor") ? attributes.at("text-anchor") : "start");
    content = (attributes.count("content") ? attributes.at("content") : "Sample Text");
  }

  ~Text() = default;

  std::string toSVG() const override
  {
    std::ostringstream ss;
    ss << "<text x=\"" << x << "\" y=\"" << y << "\" dx=\"" << dx << "\" dy=\"" << dy << "\""
       << " font-family=\"" << font_family << "\" font-size=\"" << font_size << "\""
       << " font-weight=\"" << font_weight << "\" text-anchor=\"" << text_anchor << "\""
       << printStyle() << ">" << content << "</text>";
    return ss.str();
  }

  std::string type() const override { return "text"; }

  std::shared_ptr<GraphicsObject> clone() const override
  {
    auto copy = std::make_shared<Text>();
    copy->x = x;
    copy->y = y;
    copy->dx = dx;
    copy->dy = dy;
    copy->font_family = font_family;
    copy->font_size = font_size;
    copy->font_weight = font_weight;
    copy->text_anchor = text_anchor;
    copy->content = content;

    // Clone style attributes
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
};

#endif