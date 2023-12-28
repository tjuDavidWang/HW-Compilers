#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include "Lexer.h"
#include "Parser.h"
std::unordered_map<TokenType, std::string> tokenNames = {
    {PROGRAM, "PROGRAM"},
    {VAR, "VAR"},
    {CONST, "CONST"},
    {BEGIN, "BEGIN"},
    {END, "END"},
    {WHILE, "WHILE"},
    {DO, "DO"},
    {IF, "IF"},
    {THEN, "THEN"},
    {IDENTIFIER, "IDENTIFIER"},
    {NUMBER, "NUMBER"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULTIPLY, "MULTIPLY"},
    {DIVIDE, "DIVIDE"},
    {ASSIGN, "ASSIGN"},
    {EQUAL, "EQUAL"},
    {NOT_EQUAL, "NOT_EQUAL"},
    {LESS, "LESS"},
    {LESS_EQUAL, "LESS_EQUAL"},
    {GREATER, "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {SEMICOLON, "SEMICOLON"},
    {COMMA, "COMMA"},
    {END_OF_FILE, "END_OF_FILE"},
    {UNKNOWN, "UNKNOWN"}};
int main(int argc, char *argv[])
{
    // 参数解析
    std::string inputFile;
    std::string IRFile = "IR.txt";
    std::string ITFile = "IT.csv";
    int error = -1;
    for (int i = 1; i < argc;)
    {
        std::string arg = std::string(argv[i]);
        if (arg[0] == '-')
        {
            if (arg == "-oIR")
            {
                if (i == argc-1||std::string(argv[i+1])[0]=='-')
                {
                    error = 1;
                    break;
                }
                else
                {
                    IRFile = std::string(argv[i+1]);
                    i+=2;
                }
            }
            else if (arg == "-oIT")
            {
                if (i == argc-1||std::string(argv[i+1])[0]=='-')
                {
                    error = 2;
                    break;
                }
                else
                {
                    ITFile = std::string(argv[i+1]);
                    i+=2;
                }
            }
        }
        else
        {
            inputFile = arg;
            error = 0;
            i++;
        }   
    }
    if(error == -1)
    {
        std::cout<<"compiler: fatal error: no input file"<<std::endl;
    }
    if(error == 1)
    {
        std::cout<<"compiler: fatal error: no output file after -oIR"<<std::endl;
    }
    if(error == 2)
    {
        std::cout<<"compiler: fatal error: no output file after -oIT"<<std::endl;
    }
    if (error != 0)
    {
        std::cout << "Usage: compiler <InputSourceFile> -oIR <IRFile> -oIT <ITFile>" << std::endl;
        std::cout << "IRFile will be default IR.txt and ITFile will be default IT.txt" << std::endl;
        std::cout << "compilation terminated"<<std::endl;
        return 0;
    }

    std::ifstream file(inputFile);
    if (!file.is_open())
    {
        std::cerr << "failed to open: " << inputFile << std::endl;
        return 1;
    }

    // 读取文件内容到字符串
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();
    file.close();

    // 创建一个词法分析器实例
    Lexer lexer(sourceCode);
    Parser parser(lexer);
    bool flag = parser.BeginParse();
    std::ofstream IRout(IRFile);
    std::ofstream ITout(ITFile);
    parser.Output(IRout, ITout);
    IRout.close();
    ITout.close();

    // 词法分析测试程序：
    //// 循环获取所有的Token，直到遇到文件结束符
    // Token token = lexer.getNextToken();
    // while (token.getType() != END_OF_FILE) {
    //     // 使用映射表来获取Token类型的名称
    //     std::string tokenName = tokenNames[token.getType()];

    //    // 打印Token的类型名称、类型和值
    //    std::cout << "Token: " << tokenName << " (" << token.getType() << "), Value: " << token.getValue() << std::endl;

    //    // 获取下一个Token
    //    token = lexer.getNextToken();
    //}

    return 0;
}
