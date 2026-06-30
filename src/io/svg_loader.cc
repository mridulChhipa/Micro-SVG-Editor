#include "src/io/svg_loader.h"

#include "src/io/lexer.h"
#include "src/io/parser.h"
#include "src/io/reader.h"

namespace micro_svg_editor {

SVG SvgLoader::Load(const QString& path) {
  Reader reader(path);
  Lexer lexer(reader);
  Parser parser(lexer);
  parser.Parse();
  return parser.GetSvg();
}

}  // namespace micro_svg_editor
