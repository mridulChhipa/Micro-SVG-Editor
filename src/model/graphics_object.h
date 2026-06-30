#ifndef MICRO_SVG_EDITOR_SRC_MODEL_GRAPHICS_OBJECT_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_GRAPHICS_OBJECT_H_

#include <memory>
#include <string>
#include <unordered_map>

namespace micro_svg_editor {

class ShapeVisitor;

class GraphicsObject {
 public:
  GraphicsObject() = default;
  virtual ~GraphicsObject() = default;

  virtual std::string ToSvg() const = 0;
  virtual std::unique_ptr<GraphicsObject> Clone() const = 0;

  // Dispatches to the matching ShapeVisitor::Visit overload (no RTTI).
  virtual void Accept(ShapeVisitor& visitor) = 0;

  // Moves the shape by (dx, dy).
  virtual void Translate(double dx, double dy) = 0;

  // Resizes the shape given a drag delta and which edges the grabbed handle
  // affects. Shapes that do not support resizing leave this a no-op.
  virtual void Resize(int dx, int dy, bool affects_left, bool affects_right,
                      bool affects_top, bool affects_bottom) = 0;

  // Capability queries with safe defaults; specific shapes override them.
  virtual bool PreferStrokeHitTest() const { return false; }
  virtual bool HasFont() const { return false; }
  virtual std::string FontFamily() const { return std::string(); }
  virtual double FontSize() const { return 0.0; }
  virtual bool CanSetBorderRadius() const { return false; }
  virtual void SetBorderRadius(double /*radius*/) {}

  void InitialiseStyle(
      const std::unordered_map<std::string, std::string>& attributes);
  std::string PrintStyle() const;

  // Accessors for the shared presentation attributes.
  const std::string& id() const { return id_; }
  void set_id(const std::string& v) { id_ = v; }
  const std::string& class_name() const { return class_name_; }
  void set_class_name(const std::string& v) { class_name_ = v; }
  const std::string& style() const { return style_; }
  void set_style(const std::string& v) { style_ = v; }
  const std::string& fill() const { return fill_; }
  void set_fill(const std::string& v) { fill_ = v; }
  float fill_opacity() const { return fill_opacity_; }
  void set_fill_opacity(float v) { fill_opacity_ = v; }
  const std::string& stroke() const { return stroke_; }
  void set_stroke(const std::string& v) { stroke_ = v; }
  float stroke_width() const { return stroke_width_; }
  void set_stroke_width(float v) { stroke_width_ = v; }
  float stroke_opacity() const { return stroke_opacity_; }
  void set_stroke_opacity(float v) { stroke_opacity_ = v; }
  const std::string& stroke_linecap() const { return stroke_linecap_; }
  void set_stroke_linecap(const std::string& v) { stroke_linecap_ = v; }
  const std::string& stroke_linejoin() const { return stroke_linejoin_; }
  void set_stroke_linejoin(const std::string& v) { stroke_linejoin_ = v; }
  const std::string& stroke_dasharray() const { return stroke_dasharray_; }
  void set_stroke_dasharray(const std::string& v) { stroke_dasharray_ = v; }
  const std::string& transform() const { return transform_; }
  void set_transform(const std::string& v) { transform_ = v; }
  float opacity() const { return opacity_; }
  void set_opacity(float v) { opacity_ = v; }
  const std::string& visibility() const { return visibility_; }
  void set_visibility(const std::string& v) { visibility_ = v; }

 private:
  std::string id_;
  std::string class_name_;
  std::string style_;
  std::string fill_{"red"};
  float fill_opacity_{1.0f};
  std::string stroke_{"black"};
  float stroke_width_{1.0f};
  float stroke_opacity_{1.0f};
  std::string stroke_linecap_{"butt"};
  std::string stroke_linejoin_{"miter"};
  std::string stroke_dasharray_{"none"};
  std::string transform_;
  float opacity_{1.0f};
  std::string visibility_{"visible"};
};

using GraphicsObjectPtr = std::unique_ptr<GraphicsObject>;

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_GRAPHICS_OBJECT_H_
