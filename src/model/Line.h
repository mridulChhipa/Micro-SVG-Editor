#ifndef LINE_HPP
#define LINE_HPP

#include "GraphicsObject.h"
#include <sstream>

class Line : public GraphicsObject
{
public:
  float x1{0}, y1{0};
  float x2{0}, y2{0};

  Line() {};
  Line(const std::unordered_map<std::string, std::string> &attributes);
  ~Line(){};

  std::string type() const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
};

#endif