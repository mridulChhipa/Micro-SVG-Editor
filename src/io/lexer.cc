#include "src/io/lexer.h"

namespace micro_svg_editor {

char Lexer::Cur() const { return rd_.Front(); }

char Lexer::Adv() { return rd_.Next(); }

bool Lexer::Eof() const { return rd_.Eof() && !has_buf_; }

}  // namespace micro_svg_editor
