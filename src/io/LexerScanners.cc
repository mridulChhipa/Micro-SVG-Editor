#include "src/io/Lexer.h"

Token Lexer::scanTagName()
{
  std::string s;
  while (!rd.eof() && isNameChar(cur()))
    s += adv();

  return make(TokenType::TAG_NAME, s);
}

Token Lexer::scanAttributeName()
{
  std::string s;
  while (!rd.eof() && (isNameChar(cur()) || cur() == '-' || cur() == ':'))
    s += adv();

  return make(TokenType::ATTRIBUTE_NAME, s);
}

Token Lexer::scanAttributeValue()
{
  char delim = adv();
  std::string s;

  while (!rd.eof() && cur() != delim)
    s += adv();

  if (!rd.eof())
    adv();
  return make(TokenType::ATTRIBUTE_VALUE, s);
}

Token Lexer::scanText()
{
  std::string s;
  while (!rd.eof() && cur() != '<')
    s += adv();
  return make(TokenType::TEXT_CONTENT, s);
}
