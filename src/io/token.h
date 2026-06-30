#ifndef MICRO_SVG_EDITOR_SRC_IO_TOKEN_H_
#define MICRO_SVG_EDITOR_SRC_IO_TOKEN_H_

#include <string>
#include <utility>

namespace micro_svg_editor {

enum class TokenType {
  kOpenTagStart,
  kClosingTagStart,
  kTagEnd,
  kSelfClosingTagEnd,
  kEq,

  kTagName,
  kAttributeName,
  kAttributeValue,
  kTextContent,

  kComment,
  kXmlDecl,

  kEndOfFile,
  kInvalid
};

struct Token {
  TokenType type;
  std::string s;

  Token(TokenType t, std::string v) : type(t), s(std::move(v)) {}

  Token() : type(TokenType::kInvalid), s() {}
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_IO_TOKEN_H_
