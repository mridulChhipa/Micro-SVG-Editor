#ifndef PARSER_HPP
#define PARSER_HPP

#include "src/io/Lexer.h"
#include "src/model/SVG.h"
#include "src/model/Rect.h"
#include "src/model/Circle.h"
#include "src/model/Line.h"
#include "src/model/Polyline.h"
#include "src/model/Path.h"
#include "src/model/Hexagon.h"
#include "src/model/Text.h"

#include <unordered_map>
#include <iostream>

class Parser
{
  Lexer &lexer;
  Token curr;
  SVG svg;

public:
  Parser(Lexer &l) : lexer(l) {}
  void parse();
  void addTagName(std::string tag_name, std::unordered_map<std::string, std::string> &attributes);
  SVG getSVG() const;
};
#endif