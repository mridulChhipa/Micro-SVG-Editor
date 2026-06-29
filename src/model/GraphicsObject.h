#ifndef MICRO_SVG_EDITOR_SRC_MODEL_GRAPHICS_OBJECT_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_GRAPHICS_OBJECT_H_

#include <memory>
#include <string>
#include <unordered_map>

class GraphicsObject {
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

  GraphicsObject() = default;
  virtual ~GraphicsObject() = default;

  virtual std::string ToSvg() const = 0;
  virtual std::string Type() const = 0;
  virtual std::shared_ptr<GraphicsObject> Clone() const = 0;

  void InitialiseStyle(
      const std::unordered_map<std::string, std::string>& attributes);
  std::string PrintStyle() const;
};

using GraphicsObjectPtr = std::shared_ptr<GraphicsObject>;

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_GRAPHICS_OBJECT_H_
