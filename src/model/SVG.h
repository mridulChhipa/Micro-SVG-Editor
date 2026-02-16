#ifndef SVG_HPP
#define SVG_HPP

#include "GraphicsObject.h"
#include <sstream>
#include <vector>

class SVG
{
public:
  float width{800}, height{600};
  std::string xmlns{"http://www.w3.org/2000/svg"};
  std::vector<float> viewBox; // x, y, width, height

  std::vector<GraphicsObjectPtr> objects;

  std::string toSVG() const;

  void add(GraphicsObjectPtr obj);
  void remove(std::size_t idx);
  void clear();

  SVG clone() const;
};

#endif