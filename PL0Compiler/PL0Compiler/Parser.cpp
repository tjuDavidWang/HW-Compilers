
#include"Parser.h"
#include"PL0Exception.h"

/*
	<条件> => <表达式><关系运算符><表达式>

	Condition将指针指向Condition后一项
*/
void Parser::Condition() {
	Expression();
	if (token.getType() != TokenType::EQUAL &&
		token.getType() != TokenType::NOT_EQUAL &&
		token.getType() != TokenType::LESS &&
		token.getType() != TokenType::LESS_EQUAL &&
		token.getType() != TokenType::GREATER &&
		token.getType() != TokenType::GREATER_EQUAL
		) {
		throw PL0Exception("非法的关系运算符", lexer->getLine(), lexer->getCol());
	}
	Expression();
}

/*
	<条件语句> => IF <条件> THEN <语句>
*/
void Parser::ConditionalStat() {
	// 此时token已经指向IF
	Condition();
	
	if (token.getType() != TokenType::THEN) {
		throw PL0Exception("条件语句格式错误", lexer->getLine(), lexer->getCol());
	}
	getNextToken();
	Statement();
}

/*
	<循环语句> => WHILE <条件> DO <语句>
*/
void Parser::LoopStat() {
	Condition();
	
	if (token.getType() != TokenType::DO) {
		throw PL0Exception("循环语句格式错误", lexer->getLine(), lexer->getCol());
	}
	getNextToken();
	Statement();
}


/*
	<因子> => <标识符>|<无符号整数>|(<表达式>)
*/
void Parser::Factor() {
	
	if (token.getType() == TokenType::IDENTIFIER) {
		// 识别为标识符

	}
	else if (token.getType() == TokenType::NUMBER) {
		// 识别为数字

	}
	else if (token.getType() == TokenType::LEFT_PAREN) {
		// 识别为表达式


		Expression();
	
		if (token.getType() != TokenType::RIGHT_PAREN) {
			throw PL0Exception("缺少右括号", lexer->getLine(), lexer->getCol());
		}

		

	}
	else {
		throw PL0Exception("缺少因子或因子格式错误", lexer->getLine(), lexer->getCol());
	}
}

/*
	<项> => <因子>|<项><乘法运算符><因子>
	可以等价为：
	<项> => <因子> { <乘法运算符> <因子> }

	Item将指针指向Item后一项
*/
void Parser::Item() {
	Factor();
	getNextToken();
	while (true) {
		if (token.getType() == TokenType::MULTIPLY || token.getType() == TokenType::DIVIDE) {
			// 存在后继因子
			// 语义处理
			getNextToken();
			Factor();
			getNextToken();
		}
		else
			break;
	}


}

/*
	<表达式> => [+|-]项|<表达式><加法运算符><项>
	可以等价为：
	<表达式> => [<加法运算符>] 项 { <加法运算符> 项 }

	Expression将指针指向Expression后一项
*/
void Parser::Expression() {
	getNextToken();
	if (token.getType() == TokenType::PLUS || token.getType() == TokenType::MINUS) {
		// 语义处理

		getNextToken();
	}
	Item();//Item函数已经将指针指向了Item后一项
	while (true) {
		if (token.getType() == TokenType::PLUS || token.getType() == TokenType::MINUS) {
			// 存在后继项
			// 语义处理
			getNextToken();
			Item();//Item函数已经将指针指向了Item后一项
		}
		else
			break;
	}
	
}

/*
	<赋值语句> => <标识符> := <表达式>
*/
void Parser::AssignmentStat() {
	// 此时token已经指向IDENTIFIER
	std::string sym_name = token.getValue();

	getNextToken();
	if (token.getType() != TokenType::ASSIGN) {
		throw PL0Exception("赋值语句格式错误", lexer->getLine(), lexer->getCol());
	}
	Expression();
	// finish 以下为语义分析、中间代码生成
}

/*
	<复合语句> => BEGIN <语句>{;<语句>}END
*/
void Parser::CompoundStat() {
	// 此时TOKEN指向BEGIN
	token = lexer->getNextToken();;
	Statement();
	while (true) {
		if (token.getType() == TokenType::SEMICOLON) {
			//空语句后分号
			getNextToken();
			Statement();
		}
		else if (token.getType() == TokenType::END) {
			//空语句后END
			break;
		}

		else {
			//正常语句结束
			getNextToken();
			if (token.getType() == TokenType::SEMICOLON) {
				//正常语句后分号
				getNextToken();
				Statement();
			}
			else if (token.getType() == TokenType::END) {
				//正常语句后END
				break;
			}
			else {
				throw PL0Exception("复合语句格式错误", lexer->getLine(), lexer->getCol());
			}
		}
	}
}
/*
	<变量说明> => VAR<标识符>{,<标识符>};
*/
void Parser::VarDeclaration() {
	// 此时token已经指向VAR
	getNextToken();
	if (token.getType() != TokenType::IDENTIFIER) {
		throw PL0Exception("变量说明格式错误", lexer->getLine(), lexer->getCol());
	}
	std::string sym_name = token.getValue();//sym_name为标识符名，此处生成中间代码
	getNextToken();
	while (true) {
		if (token.getType() == TokenType::COMMA) {
			getNextToken();
			if (token.getType() != TokenType::IDENTIFIER) {
				throw PL0Exception("变量说明格式错误", lexer->getLine(), lexer->getCol());
			}
			std::string sym_name = token.getValue();//sym_name为标识符名，此处生成中间代码
			getNextToken();
		}
		else break;
	}
	if (token.getType() != TokenType::SEMICOLON) {
		throw PL0Exception("常量说明格式错误，缺少分号", lexer->getLine(), lexer->getCol());
	}
}

/*
	<常量定义> => <标识符>=<无符号整数>
*/
void Parser::ConstDefinition() {
	getNextToken();
	if (token.getType() != TokenType::IDENTIFIER) {
		throw PL0Exception("常量定义格式错误", lexer->getLine(), lexer->getCol());
	}
	std::string sym_name = token.getValue();
	getNextToken();
	if (token.getType() != TokenType::EQUAL) {
		throw PL0Exception("常量定义格式错误", lexer->getLine() , lexer->getCol());
	}
	getNextToken();
	std::string sym_num = token.getValue();
	int value = std::stoi(sym_num);
	if (token.getType() != TokenType::NUMBER) {
		throw PL0Exception("常量定义格式错误", lexer->getLine(), lexer->getCol());
	}
	// finish 以下为语义分析、中间代码生成，sym_name、sym_num可用

}

/*
	<常量说明> => CONST <常量定义>{, <常量定义>};
*/
void Parser::ConstDeclaration() {
	// 此时token已经指向CONST
	ConstDefinition();
	getNextToken();
	while (true) {
		if (token.getType() == TokenType::COMMA) {
			ConstDefinition();
			getNextToken();
		}
		else break;
	}
	if (token.getType() != TokenType::SEMICOLON) {
		throw PL0Exception("常量说明格式错误，缺少分号", lexer->getLine(), lexer->getCol());
	}
	// finish 以下为语义分析、中间代码生成
}

/*
	<语句> => <赋值语句>|<条件语句>|<循环语句>|<复合语句>|<空语句>
*/
void Parser::Statement() {
	// 此时token应指向语句第一个符号
	switch (token.getType()) {
	case TokenType::IDENTIFIER:
		AssignmentStat();
		break;
	case TokenType::IF:
		ConditionalStat();
		break;
	case TokenType::WHILE:
		LoopStat();
		break;
	case TokenType::BEGIN:
		CompoundStat();
		break;
	case TokenType::SEMICOLON:
	case TokenType::END:
		//复合语句中的空语句，不作处理
		break;
	default:
		throw PL0Exception("无法识别语句种类", lexer->getLine(), lexer->getCol());
	}
}
/*
	<分程序> => [<常量说明>][<变量说明>]<语句>
	FIRST = {CONST VAR IDENTIFIER IF WHILE BEGIN END_OF_FILE}
	语句包括空语句，END_OF_FILE是空语句的FOLLOW
*/
void Parser::SubProgram() {
	getNextToken();
	if (token.getType() == TokenType::CONST) {
		ConstDeclaration();
		getNextToken();
	}
	if (token.getType() == TokenType::VAR) {
		VarDeclaration();
		getNextToken();
	}
	if (token.getType() == TokenType::END_OF_FILE)
		return;//分程序中的空语句，直接返回
	Statement();

	// finish 以下为语义分析、中间代码生成

}

/*
	<程序> => <程序首部><分程序>
	FIRST = {PROGRAM}
*/
bool Parser::BeginParse() {
	try {
		ProgramHead();
		SubProgram();
		return true;
	}
	catch (PL0Exception err) {
		std::cout << err.msg() << std::endl;
	}
	return false;
}

/*
	<程序首部> => PROGRAM<标识符>
	FIRST = {PROGRAM}
*/
void Parser::ProgramHead() {
	getNextToken();
	if (token.getType() != TokenType::PROGRAM) {
		throw PL0Exception("程序首部缺少关键字PROGRAM", lexer->getLine(), lexer->getCol());
	}
	getNextToken();
	if (token.getType() != TokenType::IDENTIFIER) {
		throw PL0Exception("程序首部缺少标识符" , lexer->getLine(), lexer->getCol());
	}
	// finish 以下为语义分析、中间代码生成
	std::string program_name = token.getValue();

}
