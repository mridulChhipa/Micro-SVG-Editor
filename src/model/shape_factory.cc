#include "src/model/shape_factory.h"

#include <memory>

#include "src/model/circle.h"
#include "src/model/hexagon.h"
#include "src/model/line.h"
#include "src/model/path.h"
#include "src/model/polyline.h"
#include "src/model/rect.h"
#include "src/model/text.h"

namespace micro_svg_editor {
namespace {

template <typename T>
std::unique_ptr<GraphicsObject> Make(const ShapeFactory::Attributes& attrs) {
  return std::make_unique<T>(attrs);
}

}  // namespace

ShapeFactory::ShapeFactory() {
  creators_["rect"] = &Make<Rect>;
  creators_["line"] = &Make<Line>;
  creators_["circle"] = &Make<Circle>;
  creators_["polyline"] = &Make<Polyline>;
  creators_["polygon"] = &Make<Hexagon>;
  creators_["path"] = &Make<Path>;
  creators_["text"] = &Make<Text>;
}

std::unique_ptr<GraphicsObject> ShapeFactory::Create(
    const std::string& tag, const Attributes& attributes) const {
  auto it = creators_.find(tag);
  return it == creators_.end() ? nullptr : it->second(attributes);
}

}  // namespace micro_svg_editor
