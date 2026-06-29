#ifndef MICRO_SVG_EDITOR_SRC_MODEL_RECT_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_RECT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/GraphicsObject.h"

class Rect : public GraphicsObject {
 public:
  float x{0.0f};
  float y{0.0f};
  float width{0.0f};
  float height{0.0f};
  float rx{0.0f};
  float ry{0.0f};

  Rect() = default;
  explicit Rect(const std::unordered_map<std::string, std::string>& attributes);

  std::string Type() const override;
  std::string ToSvg() const override;
  std::shared_ptr<GraphicsObject> Clone() const override;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_RECT_H_
