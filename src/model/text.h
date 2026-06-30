#ifndef MICRO_SVG_EDITOR_SRC_MODEL_TEXT_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_TEXT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class Text : public GraphicsObject {
 public:
  Text() = default;
  explicit Text(const std::unordered_map<std::string, std::string>& attributes);

  std::string ToSvg() const override;
  std::unique_ptr<GraphicsObject> Clone() const override;
  void Accept(ShapeVisitor& visitor) override;
  void Translate(double dx, double dy) override;
  void Resize(int dx, int dy, bool affects_left, bool affects_right,
              bool affects_top, bool affects_bottom) override;
  bool HasFont() const override { return true; }
  std::string FontFamily() const override { return font_family_; }
  double FontSize() const override { return font_size_; }

  float x() const { return x_; }
  void set_x(float v) { x_ = v; }
  float y() const { return y_; }
  void set_y(float v) { y_ = v; }
  float dx() const { return dx_; }
  void set_dx(float v) { dx_ = v; }
  float dy() const { return dy_; }
  void set_dy(float v) { dy_ = v; }
  const std::string& font_family() const { return font_family_; }
  void set_font_family(const std::string& v) { font_family_ = v; }
  float font_size() const { return font_size_; }
  void set_font_size(float v) { font_size_ = v; }
  const std::string& font_weight() const { return font_weight_; }
  void set_font_weight(const std::string& v) { font_weight_ = v; }
  const std::string& text_anchor() const { return text_anchor_; }
  void set_text_anchor(const std::string& v) { text_anchor_ = v; }
  const std::string& content() const { return content_; }
  void set_content(const std::string& v) { content_ = v; }

 private:
  float x_{0.0f};
  float y_{0.0f};
  float dx_{0.0f};
  float dy_{0.0f};
  std::string font_family_{"Arial"};
  float font_size_{12.0f};
  std::string font_weight_{"normal"};
  std::string text_anchor_{"start"};
  std::string content_{"Sample Text"};
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_TEXT_H_
