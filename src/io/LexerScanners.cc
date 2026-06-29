#include "src/io/Lexer.h"

Token Lexer::ScanTagName() {
  std::string s;
  while (!rd_.Eof() && IsNameChar(Cur())) s += Adv();

  return Make(TokenType::kTagName, s);
}

Token Lexer::ScanAttributeName() {
  std::string s;
  while (!rd_.Eof() && (IsNameChar(Cur()) || Cur() == '-' || Cur() == ':'))
    s += Adv();

  return Make(TokenType::kAttributeName, s);
}

Token Lexer::ScanAttributeValue() {
  char delim = Adv();
  std::string s;

  while (!rd_.Eof() && Cur() != delim) s += Adv();

  if (!rd_.Eof()) Adv();
  return Make(TokenType::kAttributeValue, s);
}

Token Lexer::ScanText() {
  std::string s;
  while (!rd_.Eof() && Cur() != '<') s += Adv();
  return Make(TokenType::kTextContent, s);
}
