#ifndef MICRO_SVG_EDITOR_SRC_MODEL_LINE_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_LINE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Line : public GraphicsObject {
 public:
  Line() = default;
  explicit Line(const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;
  void Resize(int dx, int dy, bool affects_left, bool affects_right,
              bool affects_top, bool affects_bottom) override;
  bool PreferStrokeHitTest() const override { return true; }

  float x1() const { return x1_; }
  void set_x1(float v) { x1_ = v; }
  float y1() const { return y1_; }
  void set_y1(float v) { y1_ = v; }
  float x2() const { return x2_; }
  void set_x2(float v) { x2_ = v; }
  float y2() const { return y2_; }
  void set_y2(float v) { y2_ = v; }

 private:
  float x1_{0}, y1_{0};
  float x2_{0}, y2_{0};
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_LINE_H_
