#ifndef LEXER_HPP
#define LEXER_HPP

#include "Reader.hpp"
#include "Token.hpp"
#include <cctype>
#include <string>
#include <iostream>

class Lexer
{
  Reader &rd;
  Token buf;
  bool has_buf{false};
  bool in_tag{false};

  char cur() const { return rd.front(); }
  char adv() { return rd.next(); }
  void skip();
  bool isNameStart(char c) const;
  bool isNameChar(char c) const;
  Token make(TokenType t, std::string v);

  Token scan();
  Token scanTagName();
  Token scanAttributeName();
  Token scanAttributeValue();
  Token scanText();
  Token scanComment();
  Token scanXml();

public:
  explicit Lexer(Reader &r) : rd(r) {}
  bool eof() const { return rd.eof() && !has_buf; }
  Token front();
  Token next();
};

#include "LexerHelpers.hpp"
#include "LexerCore.hpp"
#include "LexerScanners.hpp"
#include "LexerSpecial.hpp"

#endif