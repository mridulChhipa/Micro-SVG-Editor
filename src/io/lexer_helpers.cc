#include "src/io/lexer.h"

#include <cctype>

namespace micro_svg_editor {

/*
We treat the most recently scanned token as a buffer; if it has been read we set
the flag false, otherwise we scan a new one and mark that we have a buffer.
*/

Token Lexer::Front() {
  if (!has_buf_) {
    buf_ = Scan();
    has_buf_ = true;
  }
  return buf_;
}

Token Lexer::Next() {
  if (has_buf_) {
    has_buf_ = false;
    return buf_;
  }

  Token tok = Scan();  // Generate the next token.
  return tok;
}

// Skips every kind of whitespace character, not just a plain space
// (like \r, \n, etc).
void Lexer::Skip() {
  while (!rd_.Eof() && std::isspace(static_cast<unsigned char>(Cur()))) Adv();
}

// Whether a character may open or continue a tag/attribute name. These two
// helpers encode the naming rules.
bool Lexer::IsNameChar(char c) const {
  return std::isalnum(static_cast<unsigned char>(c));
}

bool Lexer::IsNameStart(char c) const {
  return std::isalpha(static_cast<unsigned char>(c));
}

// Creates a Token using its defined constructor.
Token Lexer::Make(TokenType t, std::string v) { return {t, std::move(v)}; }

}  // namespace micro_svg_editor
