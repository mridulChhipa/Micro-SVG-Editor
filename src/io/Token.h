#ifndef MICRO_SVG_EDITOR_SRC_IO_TOKEN_H_
#define MICRO_SVG_EDITOR_SRC_IO_TOKEN_H_

#include <string>
#include <utility>

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

inline std::string TokenTypeToString(TokenType type) {
  switch (type) {
    case TokenType::kOpenTagStart:
      return "OPEN_TAG_START";
    case TokenType::kClosingTagStart:
      return "CLOSING_TAG_START";
    case TokenType::kTagEnd:
      return "TAG_END";
    case TokenType::kSelfClosingTagEnd:
      return "SELF_CLOSING_TAG_END";
    case TokenType::kEq:
      return "EQ";
    case TokenType::kTagName:
      return "TAG_NAME";
    case TokenType::kAttributeName:
      return "ATTRIBUTE_NAME";
    case TokenType::kAttributeValue:
      return "ATTRIBUTE_VALUE";
    case TokenType::kTextContent:
      return "TEXT_CONTENT";
    case TokenType::kComment:
      return "COMMENT";
    case TokenType::kXmlDecl:
      return "XML_DECL";
    case TokenType::kEndOfFile:
      return "END_OF_FILE";
    case TokenType::kInvalid:
      return "INVALID";
    default:
      return "UNKNOWN";
  }
}

#endif  // MICRO_SVG_EDITOR_SRC_IO_TOKEN_H_
