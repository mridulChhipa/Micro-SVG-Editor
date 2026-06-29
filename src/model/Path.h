#ifndef MICRO_SVG_EDITOR_SRC_MODEL_PATH_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_PATH_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "src/model/GraphicsObject.h"

class Path : public GraphicsObject {
 public:
  std::vector<std::pair<char, std::vector<std::pair<float, float>>>> commands;

  Path() = default;
  explicit Path(const std::unordered_map<std::string, std::string>& attributes);

  std::string Type() const override;
  std::string ToSvg() const override;
  std::shared_ptr<GraphicsObject> Clone() const override;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_PATH_H_
