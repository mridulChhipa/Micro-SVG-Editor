#ifndef MICRO_SVG_EDITOR_SRC_UI_TOOLS_TOOL_ACTION_H_
#define MICRO_SVG_EDITOR_SRC_UI_TOOLS_TOOL_ACTION_H_

#include <QWidget>

namespace micro_svg_editor {

class SVG;
class GraphicsObject;

// Everything a tool action needs: a dialog parent, the document, and the
// current selection (mutable so actions can change or clear it).
struct ToolContext {
  QWidget* parent;
  SVG& svg;
  GraphicsObject*& selected;
};

// A one-shot editing tool (Strategy/Command). Drawing tools are not actions;
// they leave the current tool set so the next click creates a shape.
class ToolAction {
 public:
  virtual ~ToolAction() = default;

  // Runs the action; returns true if it modified the document (for undo).
  virtual bool Apply(ToolContext& ctx) = 0;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_TOOLS_TOOL_ACTION_H_
