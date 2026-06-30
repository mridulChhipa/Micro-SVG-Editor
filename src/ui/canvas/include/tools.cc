#include "src/ui/canvas/canvas.h"
#include "src/ui/tools/tool_action.h"

namespace micro_svg_editor {

void Canvas::SetCurrentTool(const QString& tool_name) {
  curr_tool_ = tool_name;
  qDebug() << "Current tool set to:" << curr_tool_;

  // Editing tools run immediately; drawing tools have no action and stay set so
  // the next mouse press creates a shape.
  if (ToolAction* action = tool_registry_.Find(tool_name)) {
    SVG before = svg_.Clone();
    ToolContext ctx{this, svg_, selected_shape_};
    if (action->Apply(ctx)) history_.Push(std::move(before));
    curr_tool_ = "";
  }

  update();
}

}  // namespace micro_svg_editor
