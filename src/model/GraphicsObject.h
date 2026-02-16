#ifndef GRAPHICS_OBJECT_HPP
#define GRAPHICS_OBJECT_HPP

#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <utility>

class GraphicsObject
{
public:
  std::string id;
  std::string class_name;
  std::string style;
  std::string fill{"red"};
  float fill_opacity{1.0f};
  std::string stroke{"black"};
  float stroke_width{1.0f};
  float stroke_opacity{1.0f};
  std::string stroke_linecap{"butt"};
  std::string stroke_linejoin{"miter"};
  std::string stroke_dasharray{"none"};
  std::string transform;
  float opacity{1.0f};
  std::string visibility{"visible"};

  GraphicsObject() {};
  ~GraphicsObject() {};
  virtual std::string toSVG() const = 0;
  virtual std::string type() const = 0;

  void initialiseStyle(const std::unordered_map<std::string, std::string> &attributes);

  std::string printStyle() const;

  virtual std::shared_ptr<GraphicsObject> clone() const = 0;
};

using GraphicsObjectPtr = std::shared_ptr<GraphicsObject>;

#endif