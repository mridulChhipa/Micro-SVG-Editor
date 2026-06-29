#ifndef MICRO_SVG_EDITOR_SRC_MODEL_LINE_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_LINE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/GraphicsObject.h"

class Line : public GraphicsObject {
 public:
  float x1{0}, y1{0};
  float x2{0}, y2{0};

  Line() = default;
  explicit Line(const std::unordered_map<std::string, std::string>& attributes);

  std::string Type() const override;
  std::string ToSvg() const override;
  std::shared_ptr<GraphicsObject> Clone() const override;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_LINE_H_
