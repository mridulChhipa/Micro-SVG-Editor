#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "GraphicsObject.h"
#include <sstream>

class Circle : public GraphicsObject
{
public:
  float x{0.0f};
  float y{0.0f};
  float r{0.0f};

  Circle (){};
  Circle(const std::unordered_map<std::string, std::string> &attributes);
  ~Circle(){};

  std::string toSVG() const override;
  std::string type() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
};
#endif