#ifndef LEXER_HPP
#define LEXER_HPP

#include "src/io/Reader.h"
#include "src/io/Token.h"
#include <cctype>
#include <string>
#include <iostream>

class Lexer
{
  Reader &rd;
  Token buf;
  bool has_buf{false};
  bool in_tag{false};

  char cur() const;
  char adv();
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
  bool eof() const;
  Token front();
  Token next();
};

#endif