#include "src/io/Lexer.h"
/*
We treat most recently scanned token as buffer if it has been read ,
then we set it false otherwise get some new buffer and say that we have a buffer Yo-ho-ho :)
*/

Token Lexer::front()
{
  if (!has_buf)
  {
    buf = scan();
    has_buf = true;
  }
  return buf;
}

Token Lexer::next()
{
  if (has_buf)
  {
    has_buf = false;
    return buf;
  }

  Token tok = scan(); // Generate the next token
  return tok;
}

// Skips all types of whitespace characters not just an empty space (like \r, \n, etc)
void Lexer::skip()
{
  while (!rd.eof() && std::isspace(static_cast<unsigned char>(cur())))
    adv();
}

// If what we consider as a tag name or attribute name opening is valid or not
// These following two functions take care of naming rules
bool Lexer::isNameChar(char c) const
{
  return std::isalnum(static_cast<unsigned char>(c));
}

bool Lexer::isNameStart(char c) const
{
  return std::isalpha(static_cast<unsigned char>(c));
}

// Creates a Token by calling it firstly defined constructor
Token Lexer::make(TokenType t, std::string v)
{
  return {t, std::move(v)};
}
