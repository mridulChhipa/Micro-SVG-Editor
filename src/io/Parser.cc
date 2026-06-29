#include "src/io/Parser.h"

#include <sstream>
#include <string>

void Parser::Parse() {
  while (!lexer_.Eof()) {
    curr_ = lexer_.Next();
    if (curr_.type == TokenType::kOpenTagStart) {
      curr_ = lexer_.Next();
      if (curr_.type == TokenType::kTagName && curr_.s == "svg") {
        // Parse attributes.
        curr_ = lexer_.Next();
        while (curr_.type != TokenType::kTagEnd &&
               curr_.type != TokenType::kSelfClosingTagEnd) {
          if (curr_.type == TokenType::kAttributeName) {
            std::string attr_name = curr_.s;
            curr_ = lexer_.Next();
            if (curr_.type == TokenType::kEq) {
              curr_ = lexer_.Next();
              if (curr_.type == TokenType::kAttributeValue) {
                std::string attr_value = curr_.s;
                if (attr_name == "width") {
                  svg_.width = std::stof(attr_value);
                } else if (attr_name == "height") {
                  svg_.height = std::stof(attr_value);
                } else if (attr_name == "xmlns") {
                  svg_.xmlns = attr_value;
                } else if (attr_name == "viewBox") {
                  svg_.viewBox.clear();
                  std::istringstream ss(attr_value);
                  std::string val;
                  while (ss >> val) svg_.viewBox.push_back(std::stof(val));
                }
              }
            }
          }
          curr_ = lexer_.Next();
        }
      }
      // Now the svg tag has been processed, process inner elements.
      while (!lexer_.Eof()) {
        curr_ = lexer_.Next();
        if (curr_.type == TokenType::kOpenTagStart) {
          curr_ = lexer_.Next();
          if (curr_.type == TokenType::kTagName) {
            // Now we need to create a GraphicsObject based on curr_.s.
            std::string tag_name = curr_.s;
            // Parse attributes.
            std::unordered_map<std::string, std::string> attributes;
            curr_ = lexer_.Next();
            while (curr_.type != TokenType::kTagEnd &&
                   curr_.type != TokenType::kSelfClosingTagEnd) {
              if (curr_.type == TokenType::kAttributeName) {
                std::string attr_name = curr_.s;
                curr_ = lexer_.Next();
                if (curr_.type == TokenType::kEq) {
                  curr_ = lexer_.Next();
                  if (curr_.type == TokenType::kAttributeValue) {
                    std::string attr_value = curr_.s;
                    attributes[attr_name] = attr_value;
                  }
                }
              }
              curr_ = lexer_.Next();
            }
            if (tag_name == "text") {
              curr_ = lexer_.Next();
              if (curr_.type == TokenType::kTextContent) {
                attributes["content"] = curr_.s;
                curr_ = lexer_.Next();  // Consume the closing tag.
              }
            }
            // Based on tag_name and attributes, create the GraphicsObject.
            AddTagName(tag_name, attributes);
          }
        }
      }
    }
  }
}

SVG Parser::GetSvg() const { return svg_; }
