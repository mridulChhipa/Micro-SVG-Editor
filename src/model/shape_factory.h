#ifndef MICRO_SVG_EDITOR_SRC_MODEL_SHAPE_FACTORY_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_SHAPE_FACTORY_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "src/model/graphics_object.h"

namespace micro_svg_editor {

// Creates shapes from SVG tag names. Adding a shape type means registering one
// creator here; the parser never changes (open/closed), and it depends on this
// abstraction rather than on the concrete shape classes (dependency inversion).
class ShapeFactory {
 public:
  using Attributes = std::unordered_map<std::string, std::string>;

  ShapeFactory();

  // Returns a shape for the given SVG tag, or nullptr if the tag is unknown.
  std::unique_ptr<GraphicsObject> Create(const std::string& tag,
                                         const Attributes& attributes) const;

 private:
  using Creator = std::unique_ptr<GraphicsObject> (*)(const Attributes&);
  std::unordered_map<std::string, Creator> creators_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_SHAPE_FACTORY_H_
