#ifndef MICRO_SVG_EDITOR_SRC_MODEL_HEXAGON_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_HEXAGON_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Hexagon : public GraphicsObject {
 public:
  Hexagon() = default;
  explicit Hexagon(
      const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;

  double x() const { return x_; }
  void set_x(double v) { x_ = v; }
  double y() const { return y_; }
  void set_y(double v) { y_ = v; }
  double side_length() const { return side_length_; }
  void set_side_length(double v) { side_length_ = v; }
  std::vector<std::pair<double, double>>& points() { return points_; }
  const std::vector<std::pair<double, double>>& points() const {
    return points_;
  }

 private:
  double x_{0.0};
  double y_{0.0};
  double side_length_{0.0};
  std::vector<std::pair<double, double>> points_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_HEXAGON_H_
