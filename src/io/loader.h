#ifndef MICRO_SVG_EDITOR_SRC_IO_LOADER_H_
#define MICRO_SVG_EDITOR_SRC_IO_LOADER_H_

#include <QString>

#include "src/model/svg.h"

namespace micro_svg_editor {

// Abstraction for loading an SVG document from a file. The UI depends on this
// interface rather than on the concrete Reader/Lexer/Parser pipeline (DIP).
class ILoader {
 public:
  virtual ~ILoader() = default;
  virtual SVG Load(const QString& path) = 0;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_IO_LOADER_H_
