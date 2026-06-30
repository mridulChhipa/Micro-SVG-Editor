#ifndef MICRO_SVG_EDITOR_SRC_MODEL_POLYLINE_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_POLYLINE_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Polyline : public GraphicsObject {
 public:
  Polyline() = default;
  explicit Polyline(
      const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;
  void Resize(int dx, int dy, bool affects_left, bool affects_right,
              bool affects_top, bool affects_bottom) override;
  bool PreferStrokeHitTest() const override { return true; }

  std::vector<std::pair<float, float>>& points() { return points_; }
  const std::vector<std::pair<float, float>>& points() const { return points_; }

 private:
  std::vector<std::pair<float, float>> points_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_POLYLINE_H_
