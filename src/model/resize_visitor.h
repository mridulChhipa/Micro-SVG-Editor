#ifndef MICRO_SVG_EDITOR_SRC_MODEL_RESIZE_VISITOR_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_RESIZE_VISITOR_H_

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

// Resizes a shape by a drag delta on the edges the grabbed handle affects.
// Shapes that do not support resizing (Polyline, Path, Text) are left as-is.
class ResizeVisitor : public DefaultShapeVisitor {
 public:
  ResizeVisitor(int dx, int dy, bool affects_left, bool affects_right,
                bool affects_top, bool affects_bottom)
      : dx_(dx),
        dy_(dy),
        affects_left_(affects_left),
        affects_right_(affects_right),
        affects_top_(affects_top),
        affects_bottom_(affects_bottom) {}

  void Visit(Rect& shape) override;
  void Visit(Circle& shape) override;
  void Visit(Line& shape) override;
  void Visit(Hexagon& shape) override;

 private:
  int dx_, dy_;
  bool affects_left_, affects_right_, affects_top_, affects_bottom_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_RESIZE_VISITOR_H_
