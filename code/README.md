# HW-Compilers
同济大学编译原理期末项目——Mini-Compiler

PL/0是一种教学级编程语言，专为编译原理的学习而设计，它通过简化的语法和结构，使我们能够轻松掌握词法分析、语法分析和代码生成等编译过程的基础概念，适用于实现教学目的的编译器项目。

本项目实现了输入PL/0源码，输出四元式格式三地址中间代码的编译器

## 构建
本项目可在Linux环境下使用gcc进行构建
~~~shell
HW-Compiler > ls
Lexer.cpp  Lexer.h  main.cpp  Parser.cpp  Parser.h  PL0Exception.h  README.md  source
HW-Compiler > g++ *.cpp -o compiler
~~~
在Windows中可以采用如下的方式构建：
~~~shell
g++ main.cpp Parser.cpp Lexer.cpp -o compiler -std=c++17
compiler source/source.pl0 -oIR IRfile2 -oIT ITfile2
~~~
## 使用方法
Usage: compiler \<InputSourceFile> -oIR \<IRFile> -oIT \<ITFile>

使用终端运行compiler可执行文件，后跟随要编译的源文件名称
~~~shell
HW-Compiler > ./compiler ./source/source.pl0
~~~
编译器默认在当前目录下生成中间代码文件IR.txt和符号表文件IT.csv
~~~shell
HW-Compiler > ls
compiler  IR.txt  IT.csv  Lexer.cpp  Lexer.h  main.cpp  Parser.cpp  Parser.h  PL0Exception.h  README.md  source
~~~
也可以使用命令行参数-oIR指定生成的中间代码文件，-oIT指定生成的符号表文件
~~~shell
HW-Compiler > ./compiler ./source/source.pl0 -oIR ./result/IR.txt -oIT ./result/IT.txt
HW-Compiler > ls -R
.:
compiler  Lexer.cpp  Lexer.h  main.cpp  Parser.cpp  Parser.h  PL0Exception.h  README.md  result  source

./result:
IR.txt  IT.txt

./source:
source.pl0
~~~
