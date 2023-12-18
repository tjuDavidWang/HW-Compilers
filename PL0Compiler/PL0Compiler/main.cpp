#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include <fstream>
#include <sstream>
#include<stdexcept>
#include"Lexer.h"
#include"Parser.h"
std::unordered_map<TokenType, std::string> tokenNames = {
    {PROGRAM, "PROGRAM"}, {VAR, "VAR"}, {CONST, "CONST"}, {BEGIN, "BEGIN"},
    {END, "END"}, {WHILE, "WHILE"}, {DO, "DO"}, {IF, "IF"}, {THEN, "THEN"},
    {IDENTIFIER, "IDENTIFIER"}, {NUMBER, "NUMBER"},
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULTIPLY, "MULTIPLY"}, {DIVIDE, "DIVIDE"},
    {ASSIGN, "ASSIGN"}, {EQUAL, "EQUAL"}, {NOT_EQUAL, "NOT_EQUAL"},
    {LESS, "LESS"}, {LESS_EQUAL, "LESS_EQUAL"}, {GREATER, "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LEFT_PAREN, "LEFT_PAREN"}, {RIGHT_PAREN, "RIGHT_PAREN"},
    {SEMICOLON, "SEMICOLON"}, {COMMA, "COMMA"},
    {END_OF_FILE, "END_OF_FILE"}, {UNKNOWN, "UNKNOWN"}
};
int main() {
    // 指定源代码文件的路径
    std::string filePath = "source/source.pl0";

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "failed to open: " << filePath << std::endl;
        return 1;
    }

    // 读取文件内容到字符串
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    // 创建一个词法分析器实例
    Lexer lexer(sourceCode);
    Parser parser(lexer);
    bool flag=parser.BeginParse();

    if (flag)std::cout << "语法分析成功" << std::endl;

    // 词法分析测试程序：
    //// 循环获取所有的Token，直到遇到文件结束符
    //Token token = lexer.getNextToken();
    //while (token.getType() != END_OF_FILE) {
    //    // 使用映射表来获取Token类型的名称
    //    std::string tokenName = tokenNames[token.getType()];

    //    // 打印Token的类型名称、类型和值
    //    std::cout << "Token: " << tokenName << " (" << token.getType() << "), Value: " << token.getValue() << std::endl;

    //    // 获取下一个Token
    //    token = lexer.getNextToken();
    //}

    return 0;
}
