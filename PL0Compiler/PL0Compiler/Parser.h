#pragma once

#include "Lexer.h"


/*
	语法分析器
*/

class Parser
{
public:
	Parser(Lexer& lexer) {this->lexer = &lexer;}
	~Parser() {}
	bool BeginParse(); // 程序  启动语法分析
	
private:
	Token token;// 当前符号
	Lexer* lexer=nullptr;// 词法分析器

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
	

	void Expression();//表达式
	void Item();//项
	void Factor();//因子

	void Condition();//条件


	
};


