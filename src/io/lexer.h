#ifndef MICRO_SVG_EDITOR_SRC_IO_LEXER_H_
#define MICRO_SVG_EDITOR_SRC_IO_LEXER_H_

#include <string>

#include "src/io/reader.h"
#include "src/io/token.h"

namespace micro_svg_editor {

class Lexer {
 public:
  explicit Lexer(Reader& r) : rd_(r) {}
  bool Eof() const;
  Token Front();
  Token Next();

 private:
  char Cur() const;
  char Adv();
  void Skip();
  bool IsNameStart(char c) const;
  bool IsNameChar(char c) const;
  Token Make(TokenType t, std::string v);

  Token Scan();
  Token ScanTagName();
  Token ScanAttributeName();
  Token ScanAttributeValue();
  Token ScanText();
  Token ScanComment();
  Token ScanXml();

  Reader& rd_;
  Token buf_;
  bool has_buf_{false};
  bool in_tag_{false};
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_IO_LEXER_H_
