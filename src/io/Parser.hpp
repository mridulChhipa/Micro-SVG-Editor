#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "../model/SVG.hpp"
#include "../model/Rect.hpp"
#include "../model/Circle.hpp"
#include "../model/Line.hpp"
#include "../model/Polyline.hpp"
#include "../model/Polygon.hpp"
#include "../model/Text.hpp"

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
                                    // Handle other attributes as needed
                                    else if (attrName == "xmlns")
                                        svg.xmlns = attrValue;
                                    else if (attrName == "viewBox")
                                    {
                                        svg.viewBox.clear();
                                        std::istringstream ss(attrValue);
                                        std::string val;
                                        while (ss >> val)
                                        {
                                            svg.viewBox.push_back(std::stof(val));
                                        }
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
                            std::string tagName = curr.s;

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

                            if (tagName == "text")
                            {
                                curr = lexer.next();
                                if (curr.type == TokenType::TEXT_CONTENT)
                                {
                                    attributes["content"] = curr.s;
                                    curr = lexer.next(); // Consume the closing tag
                                }
                            }

                            // Here, based on tagName and attributes, create the appropriate GraphicsObject
                            if (tagName == "rect")
                                svg.add(std::make_shared<Rect>(attributes));
                            else if (tagName == "line")
                                svg.add(std::make_shared<Line>(attributes));
                            else if (tagName == "circle")
                                svg.add(std::make_shared<Circle>(attributes));
                            else if (tagName == "polyline")
                                svg.add(std::make_shared<Polyline>(attributes));
                            else if (tagName == "polygon")
                                svg.add(std::make_shared<Polygon>(attributes));
                            if (tagName == "text")
                                svg.add(std::make_shared<Text>(attributes));

                            std::cout << "Created " << tagName << " with attributes:\n";
                            // for (const auto &attr : attributes)
                            // {
                            //     std::cout << "  " << attr.first << " = " << attr.second << "\n";
                            // }
                        }
                    }
                }
            }
        }

        // std::cout << svg.toSVG();
    }

    SVG getSVG() const { return svg; }
};
#endif