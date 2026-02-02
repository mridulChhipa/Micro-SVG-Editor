#include "../io/Reader.hpp"
#include "../io/Lexer.hpp"
#include "../io/Parser.hpp"
#include <iostream>

int main()
{
    // std::string path = "src/testxmls/circle.svg";
    std::string path = "src/testxmls/test_case.svg";
    Reader reader(path);
    Lexer lexer(reader);
    Parser parser(lexer);

    parser.parse();
    
    return 0;
}