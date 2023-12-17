#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include <fstream>
#include <sstream>
#include<stdexcept>

enum TokenType {
    PROGRAM, VAR, CONST, BEGIN, END, WHILE, DO, IF, THEN, 
    IDENTIFIER, NUMBER, 
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN, EQUAL, NOT_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON, COMMA,
    END_OF_FILE, UNKNOWN
};

enum State {
    START, IN_COMMENT, IN_NUMBER, IN_IDENTIFIER, IN_ASSIGN, IN_LESS, IN_GREATER, IN_ERROR
};

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

class Token {
public:
    Token(TokenType type, std::string value) : type(type), value(value) {}

    TokenType getType() const { return type; }
    std::string getValue() const { return value; }

private:
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source) : source(source), position(0), line(1), col(1), currentState(START) {}

    Token getNextToken() {
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
                    } else {
                        currentState = START;
                        return Token(UNKNOWN, ":");
                    }

                // 处理 '<', '<>', '<=' 符号
                case IN_LESS:
                    if (current == '>') {
                        advance(); // 跳过 '>'
                        currentState = START;
                        return Token(NOT_EQUAL, "<>");
                    } else if (current == '=') {
                        advance(); // 跳过 '='
                        currentState = START;
                        return Token(LESS_EQUAL, "<=");
                    } else {
                        currentState = START;
                        return Token(LESS, "<");
                    }

                // 处理 '>' 和 '>=' 符号
                case IN_GREATER:
                    if (current == '=') {
                        advance(); // 跳过 '='
                        currentState = START;
                        return Token(GREATER_EQUAL, ">=");
                    } else {
                        currentState = START;
                        return Token(GREATER, ">");
                    }

                default:
                    // 如果状态未知，抛出异常
                    throw std::runtime_error("Lexer in unknown state");
            }
        }

        // 文件末尾时返回END_OF_FILE
        return Token(END_OF_FILE, "");
    }



private:
    std::string source;
    size_t position;
    int line, col; // 添加行号和列号，用于输出错误信息

    State currentState;

    void advance() {
        if (source[position] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        position++;
    }

    void handleWhitespace() {
        while (position < source.length() && isspace(source[position])) {
            advance();
        }
        currentState = START;
    }

    Token identifierOrKeyword() {
        size_t start = position;
        while (position < source.length() && isalnum(source[position])) {
            advance();
        }
        std::string value = source.substr(start, position - start);
        currentState = START;
        return Token(determineIdentifierType(value), value);
    }

    Token number() {
        size_t start = position;
        while (position < source.length() && isdigit(source[position])) {
            advance();
        }
        std::string value = source.substr(start, position - start);
        currentState = START;
        return Token(NUMBER, value);
    }

    TokenType determineIdentifierType(const std::string &str) {
        static const std::unordered_map<std::string, TokenType> keywords = {
            {"PROGRAM", PROGRAM}, {"VAR", VAR}, {"CONST", CONST}, {"BEGIN", BEGIN}, {"END", END}, {"WHILE", WHILE}, {"DO", DO}, {"IF", IF}, {"THEN", THEN}};

        auto it = keywords.find(str);
        if (it != keywords.end()) {
            return it->second;
        }
        return IDENTIFIER;
    }


    Token handleSymbol() {
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
                } else if (source[position] == '=') {
                    advance(); 
                    return Token(LESS_EQUAL, "<=");
                } else {
                    return Token(LESS, "<");
                }
            case '>':
                if (source[position] == '=') {
                    advance(); 
                    return Token(GREATER_EQUAL, ">=");
                } else {
                    return Token(GREATER, ">");
                }
            case ':':
                if (source[position] == '=') {
                    advance(); 
                    return Token(ASSIGN, ":=");
                } else {
                    throw std::runtime_error("Unexpected character: ':', expected ':='");
                }
            case '(': return Token(LEFT_PAREN, "(");
            case ')': return Token(RIGHT_PAREN, ")");
            case ';': return Token(SEMICOLON, ";");
            case ',': return Token(COMMA, ",");
            default:
                throw std::runtime_error("Unexpected character: " + std::string(1, current));
        }
    }

};


int main() {
    // 指定源代码文件的路径
    std::string filePath = "source.pl0";

    // 使用文件流对象读取文件
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

    // 循环获取所有的Token，直到遇到文件结束符
    Token token = lexer.getNextToken();
    while (token.getType() != END_OF_FILE) {
        // 使用映射表来获取Token类型的名称
        std::string tokenName = tokenNames[token.getType()];

        // 打印Token的类型名称、类型和值
        std::cout << "Token: " << tokenName << " (" << token.getType() << "), Value: " << token.getValue()<< std::endl;

        // 获取下一个Token
        token = lexer.getNextToken();
    }

    return 0;
}
