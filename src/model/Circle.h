#ifndef MICRO_SVG_EDITOR_SRC_MODEL_CIRCLE_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_CIRCLE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/GraphicsObject.h"

class Circle : public GraphicsObject {
 public:
  float x{0.0f};
  float y{0.0f};
  float r{0.0f};

  Circle() = default;
  explicit Circle(
      const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::string Type() const override;
  std::shared_ptr<GraphicsObject> Clone() const override;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_CIRCLE_H_
