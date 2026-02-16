#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.h"
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
  void parse()
  {
    while (!lexer.eof())
    {
      curr = lexer.next();
      if (curr.type == TokenType::OPEN_TAG_START)
      {
        curr = lexer.next();
        if (curr.type == TokenType::TAG_NAME && curr.s == "svg")
        {
          // Parse attributes
          curr = lexer.next();

          while (curr.type != TokenType::TAG_END && curr.type != TokenType::SELF_CLOSING_TAG_END)
          {
            if (curr.type == TokenType::ATTRIBUTE_NAME)
            {
              std::string attrName = curr.s;
              curr = lexer.next();

              if (curr.type == TokenType::EQ)
              {
                curr = lexer.next();

                if (curr.type == TokenType::ATTRIBUTE_VALUE)
                {
                  std::string attrValue = curr.s;
                  if (attrName == "width")
                    svg.width = std::stof(attrValue);
                  else if (attrName == "height")
                    svg.height = std::stof(attrValue);
                  else if (attrName == "xmlns")
                    svg.xmlns = attrValue;
                  else if (attrName == "viewBox")
                  {
                    svg.viewBox.clear();
                    std::istringstream ss(attrValue);
                    std::string val;
                    while (ss >> val)
                      svg.viewBox.push_back(std::stof(val));
                  }
                }
              }
            }
            curr = lexer.next();
          }
        }

        // Now the svg tag has been processed, process inner elements
        while (!lexer.eof())
        {
          curr = lexer.next();
          if (curr.type == TokenType::OPEN_TAG_START)
          {
            curr = lexer.next();
            if (curr.type == TokenType::TAG_NAME)
            {
              // Now we need to create a GraphicsObject based on curr.s
              std::string tag_name = curr.s;

              // Parse attributes'
              std::unordered_map<std::string, std::string> attributes;
              curr = lexer.next();

              while (curr.type != TokenType::TAG_END && curr.type != TokenType::SELF_CLOSING_TAG_END)
              {
                if (curr.type == TokenType::ATTRIBUTE_NAME)
                {
                  std::string attrName = curr.s;
                  curr = lexer.next();

                  if (curr.type == TokenType::EQ)
                  {
                    curr = lexer.next();

                    if (curr.type == TokenType::ATTRIBUTE_VALUE)
                    {
                      std::string attrValue = curr.s;
                      attributes[attrName] = attrValue;
                    }
                  }
                }

                curr = lexer.next();
              }

              if (tag_name == "text")
              {
                curr = lexer.next();
                if (curr.type == TokenType::TEXT_CONTENT)
                {
                  attributes["content"] = curr.s;
                  curr = lexer.next(); // Consume the closing tag
                }
              }

              // Here, based on tag_name and attributes, create the appropriate GraphicsObject
              if (tag_name == "rect")
                svg.add(std::make_shared<Rect>(attributes));
              else if (tag_name == "line")
                svg.add(std::make_shared<Line>(attributes));
              else if (tag_name == "circle")
                svg.add(std::make_shared<Circle>(attributes));
              else if (tag_name == "polyline")
                svg.add(std::make_shared<Polyline>(attributes));
              else if (tag_name == "polygon")
                svg.add(std::make_shared<Hexagon>(attributes));
              else if (tag_name == "path")
                svg.add(std::make_shared<Path>(attributes));
              else if (tag_name == "text")
                svg.add(std::make_shared<Text>(attributes));
            }
          }
        }
      }
    }
  }

  SVG getSVG() const { return svg; }
};
#endif