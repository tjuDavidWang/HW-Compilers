#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include <fstream>
#include <sstream>
#include<stdexcept>

// 所有词法符号枚举类型
enum TokenType {
    PROGRAM, VAR, CONST, BEGIN, END, WHILE, DO, IF, THEN, 
    IDENTIFIER, NUMBER, 
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN, EQUAL, NOT_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON, COMMA,
    END_OF_FILE, UNKNOWN
};

// 词法分析器状态
enum State {
    START, IN_COMMENT, IN_NUMBER, IN_IDENTIFIER, IN_ASSIGN, IN_LESS, IN_GREATER, IN_ERROR
};


// 符号类
class Token {
public:
    Token(TokenType type, std::string value) : type(type), value(value) {}

    TokenType getType() const { return type; }
    std::string getValue() const { return value; }

private:
    TokenType type;
    std::string value;
};

// 词法分析类
class Lexer {
private:
    std::string source;
    size_t position;
    int line, col; // 添加行号和列号，用于输出错误信息

    State currentState;
public:
    Lexer(const std::string& source) : 
        source(source), position(0), line(1), col(1), currentState(START) {};
    
    // 获得下一个符号
    Token getNextToken();

private:
    void advance() ;
    void handleWhitespace() ;
    Token identifierOrKeyword() ;
    Token number() ;
    TokenType determineIdentifierType(const std::string &str) ;
    Token handleSymbol() ;

};