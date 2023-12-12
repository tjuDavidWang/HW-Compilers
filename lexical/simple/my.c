#include "my.h"

void error(int n)
{
	printf("Error %3d: %s\n", n, err_msg[n]);
}

void lexer(FILE *fp)
{
    // 使用fgetc读取下一个字符
	ch = fgetc(fp);

	while (ch != EOF)
	{
        // 输入为分隔符，无实际意义
		while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
		{
			ch = fgetc(fp);
		}
        
		// 处理特殊情况
		if (ch=='\377')
		{
			break;
		}

        // 输入为字母,则应该为关键字或标识符
		if (isalpha(ch)) 
		{
            //读取到的完整单词
			char a[MAXIDLEN + 1]; 

			int k = 0;
            
			for (; (isalpha(ch) || isdigit(ch)) && k < MAXIDLEN; k++)
			{
				a[k] = ch;
				ch = fgetc(fp);
			}

            // 区别于字符数组与字符串
			a[k] = '\0';

			// 检查是否为关键字
			int i = 1;
			for (; i <= NRW; i++)
			{
				if (strcmp(a, wordReserved[i]) == 0)
					break;
			}

			// a在wordReserved中，是关键字
			if (i <= NRW)
			{
				sym = wsym[i]; 
			}

			// a不在wordReserved中，是标识符
			else
			{
				sym = SYM_IDENTIFIER; // symbol is an identifier
			}

			printf("(%d,%s)\n", sym, a);
		}
		
		// 输入为数字，则应该是整数
		else if (isdigit(ch))
		{ 
			sym = SYM_NUMBER;
			int k = 0;
			int num = 0;
			// 计算其数值
			while (isdigit(ch))
			{
				num = num * 10 + ch - '0';
				ch = fgetc(fp);
				k++;
			}
			if (k > MAXNUMLEN)
				error(25); // The number is too great.
			else
			{
				printf("(%d,%d)\n", sym, num);
			}
		}

		// 进行运算符号识别
		// :=
		else if (ch == ':')
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				sym = SYM_BECOMES; // :=
				ch = fgetc(fp);
				printf("(%d,:=)\n", sym);
			}
			else
			{
				sym = SYM_NULL; // illegal?
			}
		}
		// >,>=
		else if (ch == '>')
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				sym = SYM_GEQ; // >=
				ch = fgetc(fp);
				printf("(%d,>=)\n", sym);
			}
			else
			{
				sym = SYM_GTR; // >
				printf("(%d,=)\n", sym);
			}
		}
		// <,<=,<>
		else if (ch == '<')
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				sym = SYM_LEQ; // <=
				ch = fgetc(fp);
				printf("(%d,<=)\n", sym);
			}
			else if (ch == '>')
			{
				sym = SYM_NEQ; // <>
				ch = fgetc(fp);
			}
			else
			{
				sym = SYM_LES; // <
				printf("(%d,<)\n", sym);
			}
		}
		
		// 忽略注释{}
		else if (ch == '{')
		{ 
			int end = 1;
			while (end)
			{
				ch = fgetc(fp);
				if (ch == '}')
					end = 0;
			}
			ch = fgetc(fp);
		}
		
		// 其他运算符: '+', '-', '*', '/', '(', ')', '=', ',', '.', ';'
		else
		{ 
			//代码和识别关键字那里类似
			int i = 1;
			for (; i <= NSYM; i++)
			{
				if (ch == csym[i])
					break;
			}
			if (i <= NSYM)
			{
				sym = ssym[i];
				printf("(%d,%c)\n", sym, ch);
				ch = fgetc(fp);
			}
			//不应该出现的字符
			else
			{
				printf("Fatal Error: Unknown character.\n");
				exit(1);
			}
		}
	}
	printf("—————文件读取结束—————");
}

int main()
{
	FILE *fp = fopen("source1.txt", "r");
	lexer(fp);
	return 0;
}
