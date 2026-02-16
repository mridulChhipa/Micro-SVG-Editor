#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include "GraphicsObject.h"
#include <sstream>

class Polyline : public GraphicsObject
{
public:
  std::vector<std::pair<float, float>> points;

  Polyline() {};
  Polyline(const std::unordered_map<std::string, std::string> &attributes);
  ~Polyline() {};

  std::string type() const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
};

#endif