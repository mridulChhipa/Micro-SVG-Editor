#include "src/io/Lexer.h"

char Lexer::cur() const
{
    return rd.front();
}

char Lexer::adv()
{
    return rd.next();
}

bool Lexer::eof() const
{
    return rd.eof() && !has_buf;
}
