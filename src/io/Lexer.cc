#include "src/io/Lexer.h"

char Lexer::Cur() const { return rd_.Front(); }

char Lexer::Adv() { return rd_.Next(); }

bool Lexer::Eof() const { return rd_.Eof() && !has_buf_; }
