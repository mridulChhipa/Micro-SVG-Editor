#ifndef MICRO_SVG_EDITOR_SRC_MODEL_CIRCLE_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_CIRCLE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Circle : public GraphicsObject {
 public:
  Circle() = default;
  explicit Circle(
      const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;
  void Resize(int dx, int dy, bool affects_left, bool affects_right,
              bool affects_top, bool affects_bottom) override;

  float x() const { return x_; }
  void set_x(float v) { x_ = v; }
  float y() const { return y_; }
  void set_y(float v) { y_ = v; }
  float r() const { return r_; }
  void set_r(float v) { r_ = v; }

 private:
  float x_{0.0f};
  float y_{0.0f};
  float r_{0.0f};
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_CIRCLE_H_
