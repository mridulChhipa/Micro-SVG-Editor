#include "src/io/lexer.h"

namespace micro_svg_editor {

Token Lexer::ScanComment() {
  if (rd_.Eof() || Cur() != '-') return Make(TokenType::kInvalid, "<!");
  Adv();
  if (rd_.Eof() || Cur() != '-') return Make(TokenType::kInvalid, "<!-");
  Adv();

  std::string s;
  while (!rd_.Eof()) {
    if (Cur() == '-') {
      Adv();
      if (!rd_.Eof() && Cur() == '-') {
        Adv();
        if (!rd_.Eof() && Cur() == '>') {
          Adv();
          break;
        }
        s += "--";
      } else {
        s += '-';
      }
    } else {
      s += Adv();
    }
  }
  return Make(TokenType::kComment, s);
}

Token Lexer::ScanXml() {
  std::string s;

  while (!rd_.Eof()) {
    if (Cur() == '?') {
      Adv();
      if (!rd_.Eof() && Cur() == '>') {
        Adv();
        break;
      }
      s += '?';
    } else {
      s += Adv();
    }
  }
  return Make(TokenType::kXmlDecl, s);
}

}  // namespace micro_svg_editor
