#ifndef MICRO_SVG_EDITOR_SRC_MODEL_RECT_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_RECT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Rect : public GraphicsObject {
 public:
  Rect() = default;
  explicit Rect(const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;
  void Resize(int dx, int dy, bool affects_left, bool affects_right,
              bool affects_top, bool affects_bottom) override;
  bool CanSetBorderRadius() const override { return true; }
  void SetBorderRadius(double radius) override;

  float x() const { return x_; }
  void set_x(float v) { x_ = v; }
  float y() const { return y_; }
  void set_y(float v) { y_ = v; }
  float width() const { return width_; }
  void set_width(float v) { width_ = v; }
  float height() const { return height_; }
  void set_height(float v) { height_ = v; }
  float rx() const { return rx_; }
  void set_rx(float v) { rx_ = v; }
  float ry() const { return ry_; }
  void set_ry(float v) { ry_ = v; }

 private:
  float x_{0.0f};
  float y_{0.0f};
  float width_{0.0f};
  float height_{0.0f};
  float rx_{0.0f};
  float ry_{0.0f};
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_RECT_H_
