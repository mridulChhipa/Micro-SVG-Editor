#include "src/io/parser.h"

#include <sstream>
#include <string>
#include <unordered_map>

namespace micro_svg_editor {

void Parser::Parse() {
  while (!lexer_.Eof()) {
    curr_ = lexer_.Next();
    if (curr_.type != TokenType::kOpenTagStart) continue;

    curr_ = lexer_.Next();
    if (curr_.type == TokenType::kTagName && curr_.s == "svg")
      ParseRootAttributes();

    // Once the <svg> tag has been processed, process the inner elements.
    ParseChildren();
  }
}

void Parser::ParseRootAttributes() {
  curr_ = lexer_.Next();
  while (curr_.type != TokenType::kTagEnd &&
         curr_.type != TokenType::kSelfClosingTagEnd) {
    if (curr_.type == TokenType::kAttributeName) {
      std::string name = curr_.s;
      curr_ = lexer_.Next();
      if (curr_.type == TokenType::kEq) {
        curr_ = lexer_.Next();
        if (curr_.type == TokenType::kAttributeValue)
          ApplyRootAttribute(name, curr_.s);
      }
    }
    curr_ = lexer_.Next();
  }
}

void Parser::ApplyRootAttribute(const std::string& name,
                                const std::string& value) {
  if (name == "width") {
    svg_.set_width(std::stof(value));
  } else if (name == "height") {
    svg_.set_height(std::stof(value));
  } else if (name == "xmlns") {
    svg_.set_xmlns(value);
  } else if (name == "viewBox") {
    svg_.view_box().clear();
    std::istringstream ss(value);
    std::string val;
    while (ss >> val) svg_.view_box().push_back(std::stof(val));
  }
}

std::unordered_map<std::string, std::string> Parser::ReadAttributes() {
  std::unordered_map<std::string, std::string> attributes;
  curr_ = lexer_.Next();
  while (curr_.type != TokenType::kTagEnd &&
         curr_.type != TokenType::kSelfClosingTagEnd) {
    if (curr_.type == TokenType::kAttributeName) {
      std::string name = curr_.s;
      curr_ = lexer_.Next();
      if (curr_.type == TokenType::kEq) {
        curr_ = lexer_.Next();
        if (curr_.type == TokenType::kAttributeValue)
          attributes[name] = curr_.s;
      }
    }
    curr_ = lexer_.Next();
  }
  return attributes;
}

void Parser::ParseChildren() {
  while (!lexer_.Eof()) {
    curr_ = lexer_.Next();
    if (curr_.type != TokenType::kOpenTagStart) continue;

    curr_ = lexer_.Next();
    if (curr_.type != TokenType::kTagName) continue;

    std::string tag_name = curr_.s;
    std::unordered_map<std::string, std::string> attributes = ReadAttributes();

    if (tag_name == "text") {
      curr_ = lexer_.Next();
      if (curr_.type == TokenType::kTextContent) {
        attributes["content"] = curr_.s;
        curr_ = lexer_.Next();  // Consume the closing tag.
      }
    }
    AddTagName(tag_name, attributes);
  }
}

// SVG is move-only (owns its objects via unique_ptr), so return a deep copy.
SVG Parser::GetSvg() const { return svg_.Clone(); }

}  // namespace micro_svg_editor
