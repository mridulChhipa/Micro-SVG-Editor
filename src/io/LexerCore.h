#ifndef LEXER_CORE_HPP
#define LEXER_CORE_HPP

inline Token Lexer::scan()
{
  if (rd.eof())
    return make(TokenType::END_OF_FILE, "");

  if (rd.eof())
    return make(TokenType::END_OF_FILE, "");

  char c = cur();

  if (c == '<')
  {
    adv();
    if (!rd.eof() && cur() == '/')
    {
      adv();
      in_tag = true;
      return make(TokenType::CLOSING_TAG_START, "</");
    }
    if (!rd.eof() && cur() == '!')
    {
      adv();
      return scanComment();
    }
    if (!rd.eof() && cur() == '?')
    {
      adv();
      return scanXml();
    }
    in_tag = true;
    return make(TokenType::OPEN_TAG_START, "<");
  }

  if (c == '>')
  {
    adv();
    in_tag = false;
    return make(TokenType::TAG_END, ">");
  }

  if (c == '/')
  {
    adv();
    if (!rd.eof() && cur() == '>')
    {
      adv();
      in_tag = false;
      return make(TokenType::SELF_CLOSING_TAG_END, "/>");
    }
    return make(TokenType::INVALID, "/");
  }

  if (c == '=')
  {
    adv();
    return make(TokenType::EQ, "=");
  }

  if (in_tag)
  {
    if (isNameStart(c))
      return scanTagName();

    if (c == ' ')
    {
      while (cur() == ' ')
        adv();

      if (isNameStart(cur()))
        return scanAttributeName();
    }

    if (c == '"' || c == '\'')
      return scanAttributeValue();
    adv();
    return make(TokenType::INVALID, "" + c);
  }

  return scanText();
}

#endif
