### CondTranslator

​	**这个Translator用于找出输入源代码中的条件分支语句，并且进行打印语句的插入并且转化为新的源代码**

- [demo.C](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/demo.C)即为Translator
- [build.sh](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/build.sh)是运行Translator的脚本
- [test.C](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/test.C)是用于测试的输入代码
- [demoMakefile](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/demoMakefile)是运行的makefile,无需修改
- [run.sh](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/run.sh)是运行插入后源代码的脚本

### demo.C

​	`travel(SgStatement* statement)`用于遍历从main函数开始的每一句语句。如果遇到了不同的结构体，分别进行继续遍历或者插入打印语句再遍历的操作

​	`void insertIntoIf(SgStatement* parent)`用于在if语句中插入打印语句

​	`void insertIntoSwitch(SgStatement* parent)`用于在switch语句中插入打印语句

​	`SgStatement* build(int line,string stmt)`输入行数和条件语句，构建一个插入语句

​	`vector<SgFunctionDeclaration*> filter(SgDeclarationStatementPtrList& list)`用于筛选出在当前文件中已定义的所有方法