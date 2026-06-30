#ifndef MICRO_SVG_EDITOR_SRC_MODEL_SVG_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_SVG_H_

#include <cstddef>
#include <string>
#include <vector>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

class SVG {
 public:
  std::string ToSvg() const;

  void Add(GraphicsObjectPtr obj);
  void Remove(std::size_t idx);
  void Clear();

  SVG Clone() const;

  float width() const { return width_; }
  void set_width(float v) { width_ = v; }
  float height() const { return height_; }
  void set_height(float v) { height_ = v; }
  const std::string& xmlns() const { return xmlns_; }
  void set_xmlns(const std::string& v) { xmlns_ = v; }
  std::vector<float>& view_box() { return view_box_; }
  const std::vector<float>& view_box() const { return view_box_; }
  std::vector<GraphicsObjectPtr>& objects() { return objects_; }
  const std::vector<GraphicsObjectPtr>& objects() const { return objects_; }

 private:
  float width_{800};
  float height_{600};
  std::string xmlns_{"http://www.w3.org/2000/svg"};
  std::vector<float> view_box_;  // x, y, width, height
  std::vector<GraphicsObjectPtr> objects_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_SVG_H_
