#ifndef MICRO_SVG_EDITOR_SRC_MODEL_SHAPE_VISITOR_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_SHAPE_VISITOR_H_

namespace micro_svg_editor {

class Rect;
class Circle;
class Line;
class Polyline;
class Path;
class Hexagon;
class Text;

// Visitor over the closed shape hierarchy. It lets callers run type-specific
// behaviour through virtual dispatch (GraphicsObject::Accept) instead of RTTI.
class ShapeVisitor {
 public:
  virtual ~ShapeVisitor() = default;

  virtual void Visit(Rect& shape) = 0;
  virtual void Visit(Circle& shape) = 0;
  virtual void Visit(Line& shape) = 0;
  virtual void Visit(Polyline& shape) = 0;
  virtual void Visit(Path& shape) = 0;
  virtual void Visit(Hexagon& shape) = 0;
  virtual void Visit(Text& shape) = 0;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_SHAPE_VISITOR_H_
