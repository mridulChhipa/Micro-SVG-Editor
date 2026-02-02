#ifndef LEXER_CORE_HPP
#define LEXER_CORE_HPP

inline Token Lexer::scan()
{
    if (rd.eof())
        return make(TokenType::END_OF_FILE, "");

    if (rd.eof())
        return make(TokenType::END_OF_FILE, "");

    char ch = cur();

    if (ch == '<')
    {
        adv();
        if (!rd.eof() && cur() == '/')
        {
            adv();
            inTag = true;
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
            return scanXmlDecl();
        }
        inTag = true;
        return make(TokenType::OPEN_TAG_START, "<");
    }

    if (ch == '>')
    {
        adv();
        inTag = false;
        return make(TokenType::TAG_END, ">");
    }

    if (ch == '/')
    {
        adv();
        if (!rd.eof() && cur() == '>')
        {
            adv();
            inTag = false;
            return make(TokenType::SELF_CLOSING_TAG_END, "/>");
        }
        return make(TokenType::INVALID, "/");
    }

    if (ch == '=')
    {
        adv();
        return make(TokenType::EQ, "=");
    }

    if (inTag)
    {
        if (isNameStart(ch))
            return scanTagName();

        if (ch == ' ')
        {
            while (cur() == ' ')
            {
                adv();
            }

            if (isNameStart(cur()))
                return scanAttributeName();
        }

        if (ch == '"' || ch == '\'')
            return scanAttributeValue();
        adv();
        return make(TokenType::INVALID, std::string(1, ch));
    }

    return scanText();
}

#endif
