#ifndef TEXT_HPP
#define TEXT_HPP

#include "GraphicsObject.h"
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

  Text() {};
  Text(const std::unordered_map<std::string, std::string> &attributes);
  ~Text(){};

  std::string toSVG() const override;

  std::string type() const override;

  std::shared_ptr<GraphicsObject> clone() const override;
};

#endif