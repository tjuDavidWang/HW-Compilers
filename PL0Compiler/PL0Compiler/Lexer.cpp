#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include<algorithm>
#include <fstream>
#include <sstream>
#include<stdexcept>
#include"Lexer.h"
#include"PL0Exception.h"

/*
    词法分析器实现
*/



Token Lexer::getNextToken() {
    // 循环遍历源代码中的每个字符
    while (position < source.length()) {
        char current = source[position];

        // 根据当前的状态来处理字符
        switch (currentState) {
        case START:
            // 忽略空白字符，更新状态机
            if (isspace(current)) {
                handleWhitespace();
            }
            // 如果是字母，转移到标识符状态
            else if (isalpha(current)) {
                currentState = IN_IDENTIFIER;
            }
            // 如果是数字，转移到数字状态
            else if (isdigit(current)) {
                currentState = IN_NUMBER;
            }
            // 其他情况，处理为符号或特殊字符
            else {
                return handleSymbol();
            }
            break;

        case IN_IDENTIFIER:
            // 处理标识符或关键字并返回Token
            return identifierOrKeyword();

        case IN_NUMBER:
            // 处理数字并返回Token
            return number();

            // 处理 ':=' 符号
        case IN_ASSIGN:
            if (current == '=') {
                advance(); // 跳过 '='
                currentState = START;
                return Token(ASSIGN, ":=");
            }
            else {
                currentState = START;
                return Token(UNKNOWN, ":");
            }

            // 处理 '<', '<>', '<=' 符号
        case IN_LESS:
            if (current == '>') {
                advance(); // 跳过 '>'
                currentState = START;
                return Token(NOT_EQUAL, "<>");
            }
            else if (current == '=') {
                advance(); // 跳过 '='
                currentState = START;
                return Token(LESS_EQUAL, "<=");
            }
            else {
                currentState = START;
                return Token(LESS, "<");
            }

            // 处理 '>' 和 '>=' 符号
        case IN_GREATER:
            if (current == '=') {
                advance(); // 跳过 '='
                currentState = START;
                return Token(GREATER_EQUAL, ">=");
            }
            else {
                currentState = START;
                return Token(GREATER, ">");
            }

        default:
            // 如果状态未知，抛出异常
            throw PL0Exception("词法分析器处于未知状态");
        }
    }

    // 文件末尾时返回END_OF_FILE
    return Token(END_OF_FILE, "");
}

void Lexer::advance() {
    if (source[position] == '\n') {
        line++;
        col = 1;
    }
    else {
        col++;
    }
    position++;
}

void Lexer::handleWhitespace() {
    while (position < source.length() && isspace(source[position])) {
        advance();
    }
    currentState = START;
}

Token Lexer::identifierOrKeyword() {
    size_t start = position;
     while (position < source.length() && isalnum(source[position])) {
        advance();
    }
    std::string value = source.substr(start, position - start);
    std::string upperValue = value;
    std::transform(upperValue.begin(), upperValue.end(), upperValue.begin(),
        [](unsigned char c) { return std::toupper(c); });

    currentState = START;
    return Token(determineIdentifierType(upperValue), value);
}

Token Lexer::number() {
    size_t start = position;
    while (position < source.length() && isdigit(source[position])) {
        advance();
    }
    std::string value = source.substr(start, position - start);
    currentState = START;
    return Token(NUMBER, value);
}

TokenType Lexer::determineIdentifierType(const std::string& str) {
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"PROGRAM", PROGRAM}, {"VAR", VAR}, {"CONST", CONST}, {"BEGIN", BEGIN}, {"END", END}, {"WHILE", WHILE}, {"DO", DO}, {"IF", IF}, {"THEN", THEN} };

    auto it = keywords.find(str);
    if (it != keywords.end()) {
        return it->second;
    }
    return IDENTIFIER;
}

Token Lexer::handleSymbol() {
    char current = source[position];
    advance(); // 总是先读取当前字符

    switch (current) {
    case '+': return Token(PLUS, "+");
    case '-': return Token(MINUS, "-");
    case '*': return Token(MULTIPLY, "*");
    case '/': return Token(DIVIDE, "/");
    case '=': return Token(EQUAL, "=");
    case '<':
        if (source[position] == '>') {
            advance();
            return Token(NOT_EQUAL, "<>");
        }
        else if (source[position] == '=') {
            advance();
            return Token(LESS_EQUAL, "<=");
        }
        else {
            return Token(LESS, "<");
        }
    case '>':
        if (source[position] == '=') {
            advance();
            return Token(GREATER_EQUAL, ">=");
        }
        else {
            return Token(GREATER, ">");
        }
    case ':':
        if (source[position] == '=') {
            advance();
            return Token(ASSIGN, ":=");
        }
        else {
            throw PL0Exception("非法输入 ':',你要输入 ':='？");
        }
    case '(': return Token(LEFT_PAREN, "(");
    case ')': return Token(RIGHT_PAREN, ")");
    case ';': return Token(SEMICOLON, ";");
    case ',': return Token(COMMA, ",");
    default:
        throw PL0Exception("非法字符'" + std::string(1, current)+"'");
    }
}
