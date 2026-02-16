#include "src/model/SVG.h"

std::string SVG::toSVG() const
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

void SVG::add(GraphicsObjectPtr obj)
{
  objects.push_back(std::move(obj));
}
void SVG::remove(std::size_t idx)
{
  if (idx < objects.size())
    objects.erase(objects.begin() + idx);
}

void SVG::clear() { objects.clear(); }

SVG SVG::clone() const
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
