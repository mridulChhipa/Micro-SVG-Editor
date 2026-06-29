#ifndef MICRO_SVG_EDITOR_SRC_MODEL_HEXAGON_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_HEXAGON_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "src/model/GraphicsObject.h"

class Hexagon : public GraphicsObject {
 public:
  double x{0.0};
  double y{0.0};
  double side_length{0.0};
  std::vector<std::pair<double, double>> points;

  Hexagon() = default;
  explicit Hexagon(
      const std::unordered_map<std::string, std::string>& attributes);

  std::string Type() const override;
  std::string ToSvg() const override;
  std::shared_ptr<GraphicsObject> Clone() const override;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_HEXAGON_H_
