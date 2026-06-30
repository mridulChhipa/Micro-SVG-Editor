#ifndef MICRO_SVG_EDITOR_SRC_IO_PARSER_H_
#define MICRO_SVG_EDITOR_SRC_IO_PARSER_H_

#include <string>
#include <unordered_map>

#include "src/io/lexer.h"
#include "src/model/shape_factory.h"
#include "src/model/svg.h"

namespace micro_svg_editor {

class Parser {
 public:
  explicit Parser(Lexer& l) : lexer_(l) {}
  void Parse();
  void AddTagName(std::string tag_name,
                  std::unordered_map<std::string, std::string>& attributes);
  SVG GetSvg() const;

 private:
  // Parse the <svg> root element's attributes (applied directly to svg_).
  void ParseRootAttributes();
  // Parse every child element of <svg>, adding each to svg_.
  void ParseChildren();
  // Read a tag's attribute list (until the tag end) into a name->value map.
  std::unordered_map<std::string, std::string> ReadAttributes();
  // Apply a single <svg> root attribute (width/height/xmlns/viewBox).
  void ApplyRootAttribute(const std::string& name, const std::string& value);

  Lexer& lexer_;
  Token curr_;
  SVG svg_;
  ShapeFactory factory_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_IO_PARSER_H_
