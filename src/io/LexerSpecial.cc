#include "src/io/Lexer.h"

Token Lexer::scanComment()
{
  if (rd.eof() || cur() != '-')
    return make(TokenType::INVALID, "<!");
  adv();
  if (rd.eof() || cur() != '-')
    return make(TokenType::INVALID, "<!-");
  adv();

  std::string s;
  while (!rd.eof())
  {
    if (cur() == '-')
    {
      adv();
      if (!rd.eof() && cur() == '-')
      {
        adv();
        if (!rd.eof() && cur() == '>')
        {
          adv();
          break;
        }
        s += "--";
      }
      else
        s += '-';
    }
    else
      s += adv();
  }
  return make(TokenType::COMMENT, s);
}

Token Lexer::scanXml()
{
  std::string s;

  while (!rd.eof())
  {
    if (cur() == '?')
    {
      adv();
      if (!rd.eof() && cur() == '>')
      {
        adv();
        break;
      }
      s += '?';
    }
    else
      s += adv();
  }
  return make(TokenType::XML_DECL, s);
}
