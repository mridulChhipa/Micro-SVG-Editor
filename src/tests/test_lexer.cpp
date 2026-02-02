#include "../io/Lexer.hpp"
#include <iostream>

const char *tokenTypeName(TokenType t)
{
    switch (t)
    {
    case TokenType::OPEN_TAG_START:
        return "OPEN_TAG_START";
    case TokenType::CLOSING_TAG_START:
        return "CLOSING_TAG_START";
    case TokenType::TAG_END:
        return "TAG_END";
    case TokenType::SELF_CLOSING_TAG_END:
        return "SELF_CLOSING_TAG_END";
    case TokenType::EQ:
        return "EQ";
    case TokenType::TAG_NAME:
        return "TAG_NAME";
    case TokenType::ATTRIBUTE_NAME:
        return "ATTRIBUTE_NAME";
    case TokenType::ATTRIBUTE_VALUE:
        return "ATTRIBUTE_VALUE";
    case TokenType::TEXT_CONTENT:
        return "TEXT_CONTENT";
    case TokenType::COMMENT:
        return "COMMENT";
    case TokenType::XML_DECL:
        return "XML_DECL";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    case TokenType::INVALID:
        return "INVALID";
    }
    return "UNKNOWN";
}

void testFile(const std::string &path)
{
    std::cout << "=== Testing: " << path << " ===\n";
    Reader reader(path);
    Lexer lexer(reader);

    while (!lexer.eof())
    {
        Token tok = lexer.next();
        std::cout << tokenTypeName(tok.type);
        if (!tok.s.empty() && tok.s.size() < 50)
            std::cout << " = \"" << tok.s << "\"";
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main()
{
    testFile("src/testxmls/rect.xml");
    testFile("src/testxmls/circle.xml");
    return 0;
}
