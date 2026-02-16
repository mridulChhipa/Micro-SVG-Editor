#ifndef RECT_HPP
#define RECT_HPP

#include "GraphicsObject.h"
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

  Rect() {};
  Rect(const std::unordered_map<std::string, std::string> &attributes);
  ~Rect(){};

  std::string type() const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
};

#endif