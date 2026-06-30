#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_HANDLE_TYPE_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_HANDLE_TYPE_H_

namespace micro_svg_editor {

enum class HandleType {
  kTopLeft,
  kTopRight,
  kBottomLeft,
  kBottomRight,
  kTopCenter,
  kMiddleLeft,
  kMiddleRight,
  kBottomCenter,
  kNone
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_HANDLE_TYPE_H_
