#ifndef LEXER_SCANNERS_HPP
#define LEXER_SCANNERS_HPP

inline Token Lexer::scanTagName()
{
  std::string s;
  while (!rd.eof() && isNameChar(cur()))
    s += adv();

  return make(TokenType::TAG_NAME, s);
}

inline Token Lexer::scanAttributeName()
{
  std::string s;
  while (!rd.eof() && (isNameChar(cur()) || cur() == '-' || cur() == ':'))
    s += adv();

  return make(TokenType::ATTRIBUTE_NAME, s);
}

inline Token Lexer::scanAttributeValue()
{
  char delim = adv();
  std::string s;

  while (!rd.eof() && cur() != delim)
    s += adv();

  if (!rd.eof())
    adv();
  return make(TokenType::ATTRIBUTE_VALUE, s);
}

inline Token Lexer::scanText()
{
  std::string s;
  while (!rd.eof() && cur() != '<')
    s += adv();
  return make(TokenType::TEXT_CONTENT, s);
}

#endif
