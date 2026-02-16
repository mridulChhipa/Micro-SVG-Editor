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

  std::string toSVG() const
  {
    std::ostringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << "<svg width=\"" << width << "\" height=\"" << height << "\" ";
    ss << "xmlns=\"" << xmlns << "\">\n";
    for (const auto &obj : objects)
      ss << "  " << obj->toSVG() << "\n";
    ss << "</svg>\n";
    return ss.str();
  }

  void add(GraphicsObjectPtr obj)
  {
    objects.push_back(std::move(obj));
  }
  void remove(std::size_t idx)
  {
    if (idx < objects.size())
      objects.erase(objects.begin() + idx);
  }

  void clear() { objects.clear(); }

  SVG clone() const
  {
    SVG copy;
    copy.width = width;
    copy.height = height;
    copy.xmlns = xmlns;
    copy.viewBox = viewBox;
    for (const auto &obj : objects)
      copy.objects.push_back(obj->clone()); // Deep copy of shared_ptr
    return copy;
  }
};

#endif