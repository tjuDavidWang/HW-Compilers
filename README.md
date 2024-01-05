# HW-Compilers
同济大学编译原理期末项目——Mini-Compiler

# 编译方法
通用c++程序编译方法即可，注意本程序应用了c++17标准

g++ 编译器：

```shell
g++ main.cpp Parser.cpp Lexer.cpp -o compiler -std=c++17
```

# 运行方式

命令行方法编译如下：（powershell命令，在编译结果compiler同级目录下）

```shell
./compiler source.pl0 -oIR IRfile1 -oIT ITfile1 
```

上述参数中，第一个是PL0源程序文件路径，-oIR表示生成的目标代码文件名，-oIT表示生成的符号表文件名。
