#include "src/io/lexer.h"

namespace micro_svg_editor {

Token Lexer::Scan() {
  // Generate end of file tokens as required.
  if (rd_.Eof()) return Make(TokenType::kEndOfFile, "");

  char c = Cur();

  // Different checks to detect different kinds of tokens
  // (decided by manual observation in different svgs).
  if (c == '<') {
    Adv();
    if (!rd_.Eof() && Cur() == '/') {
      Adv();
      in_tag_ = true;
      return Make(TokenType::kClosingTagStart, "</");
    }
    if (!rd_.Eof() && Cur() == '!') {
      Adv();
      return ScanComment();
    }
    if (!rd_.Eof() && Cur() == '?') {
      Adv();
      return ScanXml();
    }
    in_tag_ = true;
    return Make(TokenType::kOpenTagStart, "<");
  }

  if (c == '>') {
    Adv();
    in_tag_ = false;
    return Make(TokenType::kTagEnd, ">");
  }

  if (c == '/') {
    Adv();
    if (!rd_.Eof() && Cur() == '>') {
      Adv();
      in_tag_ = false;
      return Make(TokenType::kSelfClosingTagEnd, "/>");
    }
    return Make(TokenType::kInvalid, "/");
  }

  if (c == '=') {
    Adv();
    return Make(TokenType::kEq, "=");
  }

  if (in_tag_) {
    // If inside a tag then check for a valid tag name and generate the matching
    // token, similarly for attributes and so on.
    if (IsNameStart(c)) return ScanTagName();

    if (c == ' ') {
      while (Cur() == ' ') Adv();

      if (IsNameStart(Cur())) return ScanAttributeName();
    }

    if (c == '"' || c == '\'') return ScanAttributeValue();
    Adv();
    return Make(TokenType::kInvalid, "" + c);
  }

  return ScanText();
}

}  // namespace micro_svg_editor
