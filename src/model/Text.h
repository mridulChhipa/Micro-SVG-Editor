#ifndef MICRO_SVG_EDITOR_SRC_MODEL_TEXT_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_TEXT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/GraphicsObject.h"

class Text : public GraphicsObject {
 public:
  float x{0.0f};
  float y{0.0f};
  float dx{0.0f};
  float dy{0.0f};
  std::string font_family{"Arial"};
  float font_size{12.0f};
  std::string font_weight{"normal"};
  std::string text_anchor{"start"};

  std::string content{"Sample Text"};

  Text() = default;
  explicit Text(const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::string Type() const override;
  std::shared_ptr<GraphicsObject> Clone() const override;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_TEXT_H_
