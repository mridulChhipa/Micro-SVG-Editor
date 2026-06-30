#ifndef MICRO_SVG_EDITOR_SRC_UI_TOOLS_TOOL_REGISTRY_H_
#define MICRO_SVG_EDITOR_SRC_UI_TOOLS_TOOL_REGISTRY_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <QString>

#include "src/ui/tools/tool_action.h"

namespace micro_svg_editor {

// Maps a tool name to its action. Adding an editing tool means registering one
// here; no dispatch code needs to change (open/closed).
class ToolRegistry {
 public:
  ToolRegistry();

  // Returns the action for `tool_name`, or nullptr (e.g. for drawing tools).
  ToolAction* Find(const QString& tool_name) const;

 private:
  std::unordered_map<std::string, std::unique_ptr<ToolAction>> actions_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_TOOLS_TOOL_REGISTRY_H_
