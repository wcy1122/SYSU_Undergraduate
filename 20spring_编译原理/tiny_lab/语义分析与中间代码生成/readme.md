## 词法分析实验

#### 文件结构
$code$ 目录下为代码，其中

- $main.c$ 为主函数
- $error.h$ 为编译错误相关结构体和函数
- $scan.h$ 为词法分析主函数
- $parser.h$ 为词法分析的主函数
- $trie.h$ 为存储语法树的结构体和相关函数
- $token.h$ 为存储token的结构体
- $utils.h$ 存放一些功能函数
- $code.tny$ 为测试样例代码
- $analysis.h$ 为语义分析的主函数
- $cgen.h$ 为中间代码生成的主函数
- $symtab.h$ 存放符号表相关数据结构

$exe$ 目录下的 $test.exe$ 为可执行文件
$report$ 目录下提供 $markdown$ 版本和 $pdf$ 版本的实验报告

#### 运行方式
将 $test.txt$ 放到code目录下运行

> ./test.exe code
