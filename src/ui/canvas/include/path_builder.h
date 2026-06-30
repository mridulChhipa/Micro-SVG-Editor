#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_INCLUDE_PATH_BUILDER_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_INCLUDE_PATH_BUILDER_H_

#include <QPainterPath>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

// A ShapeVisitor that appends each shape's geometry to a QPainterPath.
class PathBuilder : public ShapeVisitor {
 public:
  explicit PathBuilder(QPainterPath& path) : path_(path) {}

  void Visit(Rect& shape) override;
  void Visit(Circle& shape) override;
  void Visit(Line& shape) override;
  void Visit(Polyline& shape) override;
  void Visit(Path& shape) override;
  void Visit(Hexagon& shape) override;
  void Visit(Text& shape) override;

 private:
  QPainterPath& path_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_INCLUDE_PATH_BUILDER_H_
