#pragma once

#include "Lexer.h"
#include<string>
#include<array>
#include<fstream>

/*
	语法分析器
*/

class Parser
{
public:
	Parser(Lexer& lexer) {this->lexer = &lexer;}
	~Parser() {}
	bool BeginParse(); // 程序  启动语法分析
	void Output(std::ofstream& IRout,std::ofstream& ITout);
	
private:
	Token token;// 当前符号
	Lexer* lexer=nullptr;// 词法分析器
	bool end_flag=0;//指示是否发生复合语句嵌套，用于复合语句解析

	void getNextToken() { token = lexer->getNextToken(); }

	
	void ProgramHead(); // 程序首部
	void SubProgram(); //分程序
	void ConstDeclaration();// 常量声明
	void ConstDefinition();//常量定义
	void VarDeclaration();//变量声明

	void Statement();//语句
	void AssignmentStat();//赋值语句
	void ConditionalStat();//条件语句
	void LoopStat();//循环语句
	void CompoundStat();//复合语句
	
	

	std::string Expression();//表达式
	std::string Item();//项
	std::string Factor();//因子

	std::array<std::string,3> Condition();//条件


	
};


