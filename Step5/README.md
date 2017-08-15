### CondTranslator

**运行步骤：**

	#### 注意demoMakefile中的内容并不保证能在其他设备中运行（因为rose的安装路径，boost的安装路径等在不同的机器上是不一致的，所以请不要完全复制粘贴，建议按照以下步骤来修改exampleMakefile）

1. 进入`rose/rose_build/exampleTranslators/documentedExamples/simpleTranslatorExamples`目录下，注意不是`rose/exampleTranslators···`

​	会存在一份exampleMakefile

2. 打开exampleMakefile

​	修改第23行，将值设置为`-I 本机rose的安装文件目录`

​	例如`CPPFLAGS              = -I /home/john/my-rose-installation/include/rose`

​	修改第45行的值，$(ROSE_SOURCE_DIR)/解析器文件

​	请将解析器文件修改为demo.C

​	此行修改后结果为`/bin/sh ../../../libtool --mode=compile $(CXX) $(CXXFLAGS)  $(CPPFLAGS) -I$(ROSE_INCLUDE_DIR) $(BOOST_CPPFLAGS) -c -o exampleTranslator.lo $(ROSE_SOURCE_DIR)/demo.C`

​	修改48行，在此行末尾加入boost文件支持

​	加入`-Lboost的lib的目录 -lboost_system`

​	此行修改后可能为如下形式`/bin/sh ../../../libtool --mode=link $(CXX) $(CXXFLAGS) $(LDFLAGS) -o exampleTranslator exampleTranslator.lo $(ROSE_LIBS) -L/home/john/boost/lib -lboost_system`	

3. 将demo.C放置于`rose/exampleTranslators/documentedExamples/simpleTranslatorExamples`

   注意不是`rose/rose_build/exampleTranslators/···`

4. 将输入代码也放入`rose/exampleTranslators/documentedExamples/simpleTranslatorExamples`，假设输入代码的名称为test.C（请关注输入代码名称，后续可能会用到）

5. 修改脚本build.sh中./exampleTranslator一句后面的内容，后面的内容应该是test.C的路径（xxxxxxx/rose/exampleTranslators/documentedExamples/simpleTranslatorExamples/test.C）

6. 无需修改run.sh

7. 将build.sh和run.sh放到`rose/rose_build/exampleTranslators/documentedExamples/simpleTranslatorExamples`目录下

8. 最后各个文件的分布应如下

   `rose/rose_build/exampleTranslators/documentedExamples/simpleTranslatorExamples`存在build.sh		run.sh		exampleMakefile

   `rose/exampleTranslators/documentedExamples/simpleTranslatorExamples`存在demo.C和test.C

9. 在`rose/rose_build/exampleTranslators/documentedExamples/simpleTranslatorExamples`目录下打开终端，输入`./build.sh`会生成exampleTranslator和rose_test.C

10. 运行`./run.sh`会生成修改后的rose_test.C和打印运行rose_test.C的结果

Tip:如果输入代码的名字叫name.C则自动生成的新代码的文件会叫rose_name.C，所以需要修改run.sh中的rose_test.C为rose_name.C



​	**这个Translator用于找出输入源代码中的条件分支语句，并且进行打印语句的插入并且转化为新的源代码**

- [demo.C](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/demo.C)即为Translator

- [build.sh](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/build.sh)是运行Translator的脚本，运行完后会生成新的Translator并处理输入

- [test.C](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/test.C)是用于测试的输入代码

- [demoMakefile](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/demoMakefile)是运行的makefile,无需修改

- [run.sh](https://github.com/Prevalence/FloatAnalysis/blob/master/Step5/run.sh)是运行插入后源代码的脚本，运行完后会出现新的代码的运行结果，即插入的打印语句的结果

  ​

### demo.C

​	`travel(SgStatement* statement)`用于遍历从main函数开始的每一句语句。如果遇到了不同的结构体，分别进行继续遍历或者插入打印语句再遍历的操作

​	`void insertIntoIf(SgStatement* parent)`用于在if语句中插入打印语句

​	`void insertIntoSwitch(SgStatement* parent)`用于在switch语句中插入打印语句

​	`SgStatement* build(int line,string stmt)`输入行数和条件语句，构建一个插入语句

​	`vector<SgFunctionDeclaration*> filter(SgDeclarationStatementPtrList& list)`用于筛选出在当前文件中已定义的所有方法