#ifndef HEXAGON_HPP
#define HEXAGON_HPP

#include "GraphicsObject.h"
#include <sstream>
#include <limits>
#include <cmath>

class Hexagon : public GraphicsObject
{
public:
  double x, y;
  double side_length;
  std::vector<std::pair<double, double>> points;

  Hexagon() {};
  Hexagon(const std::unordered_map<std::string, std::string> &attributes);
  ~Hexagon() {};

  std::string type() const override;

  std::string toSVG() const override;

  std::shared_ptr<GraphicsObject> clone() const override;
};

#endif