#ifndef LEXER_HELPERS_HPP
#define LEXER_HELPERS_HPP

inline Token Lexer::front()
{
  if (!has_buf)
  {
    buf = scan();
    has_buf = true;
  }
  return buf;
}

inline Token Lexer::next()
{
  if (has_buf)
  {
    has_buf = false;
    return buf;
  }

  Token tok = scan();
  return tok;
}

inline void Lexer::skip()
{
  while (!rd.eof() && std::isspace(static_cast<unsigned char>(cur())))
    adv();
}

inline bool Lexer::isNameChar(char c) const
{
  return std::isalnum(static_cast<unsigned char>(c));
}

inline bool Lexer::isNameStart(char c) const
{
  return std::isalpha(static_cast<unsigned char>(c));
}

inline Token Lexer::make(TokenType t, std::string v)
{
  return {t, std::move(v)};
}

#endif
