#ifndef MICRO_SVG_EDITOR_SRC_MODEL_PATH_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_PATH_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Path : public GraphicsObject {
 public:
  using CommandList =
      std::vector<std::pair<char, std::vector<std::pair<float, float>>>>;

  Path() = default;
  explicit Path(const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;

  CommandList& commands() { return commands_; }
  const CommandList& commands() const { return commands_; }

 private:
  CommandList commands_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_PATH_H_
