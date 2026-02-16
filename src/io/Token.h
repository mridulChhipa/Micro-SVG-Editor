#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstddef>
#include <string>

enum class TokenType
{
  OPEN_TAG_START,
  CLOSING_TAG_START,
  TAG_END,
  SELF_CLOSING_TAG_END,
  EQ,

  TAG_NAME,
  ATTRIBUTE_NAME,
  ATTRIBUTE_VALUE,
  TEXT_CONTENT,

  COMMENT,
  XML_DECL,

  END_OF_FILE,
  INVALID
};

struct Token
{
  TokenType type;
  std::string s;

  Token(TokenType t, std::string v)
      : type(t), s(std::move(v)) {}

  Token() : type(TokenType::INVALID), s() {}
};

inline std::string tokenTypeToString(TokenType type)
{
  switch (type)
  {
  case TokenType::OPEN_TAG_START:
    return "OPEN_TAG_START";
  case TokenType::CLOSING_TAG_START:
    return "CLOSING_TAG_START";
  case TokenType::TAG_END:
    return "TAG_END";
  case TokenType::SELF_CLOSING_TAG_END:
    return "SELF_CLOSING_TAG_END";
  case TokenType::EQ:
    return "EQ";
  case TokenType::TAG_NAME:
    return "TAG_NAME";
  case TokenType::ATTRIBUTE_NAME:
    return "ATTRIBUTE_NAME";
  case TokenType::ATTRIBUTE_VALUE:
    return "ATTRIBUTE_VALUE";
  case TokenType::TEXT_CONTENT:
    return "TEXT_CONTENT";
  case TokenType::COMMENT:
    return "COMMENT";
  case TokenType::XML_DECL:
    return "XML_DECL";
  case TokenType::END_OF_FILE:
    return "END_OF_FILE";
  case TokenType::INVALID:
    return "INVALID";
  default:
    return "UNKNOWN";
  }
}

#endif