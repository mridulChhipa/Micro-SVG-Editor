#ifndef MICRO_SVG_EDITOR_SRC_IO_SVG_LOADER_H_
#define MICRO_SVG_EDITOR_SRC_IO_SVG_LOADER_H_

#include <QString>

#include "src/io/loader.h"
#include "src/model/svg.h"

namespace micro_svg_editor {

// Loads an SVG document via the Reader -> Lexer -> Parser pipeline.
class SvgLoader : public ILoader {
 public:
  SVG Load(const QString& path) override;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_IO_SVG_LOADER_H_
