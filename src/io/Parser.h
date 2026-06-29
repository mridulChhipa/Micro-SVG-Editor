#ifndef MICRO_SVG_EDITOR_SRC_IO_PARSER_H_
#define MICRO_SVG_EDITOR_SRC_IO_PARSER_H_

#include <string>
#include <unordered_map>

#include "src/io/Lexer.h"
#include "src/model/SVG.h"

class Parser {
 public:
  explicit Parser(Lexer& l) : lexer_(l) {}
  void Parse();
  void AddTagName(std::string tag_name,
                  std::unordered_map<std::string, std::string>& attributes);
  SVG GetSvg() const;

 private:
  Lexer& lexer_;
  Token curr_;
  SVG svg_;
};

#endif  // MICRO_SVG_EDITOR_SRC_IO_PARSER_H_
