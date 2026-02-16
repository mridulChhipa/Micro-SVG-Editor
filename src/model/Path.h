#ifndef PATH_HPP
#define PATH_HPP

#include "GraphicsObject.h"
#include <sstream>
#include <string>

class Path : public GraphicsObject
{
public:
  std::vector<std::pair<char, std::vector<std::pair<float, float>>>> commands;

  Path() {};
  Path(const std::unordered_map<std::string, std::string> &attributes);
  ~Path() {};

  std::string type() const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
};
#endif