## Step 2
### *1.总体描述*
我们输入cpp文件之后，由于klee不支持float，所以将float转化为整型数据，通过klee得到不同约束的约束表达式，将这些表达式输入到klee-fp
通过klee-fp进行约束求解，得到约束解之后验证约束解。

### *2.准备工作*
1. 你应当安装好 **klee-fp**
 - [klee-fp作者教程 (有些问题)](http://www.mail-archive.com/klee-dev@imperial.ac.uk/msg01034.html) 
 - [我们修改过的教程](klee-fp安装指南.md)
2. 下载并且编译成功修改后的 [klee](https://github.com/walkerWx/klee) 编译和安装参考 [here](https://klee.github.io/build-llvm34/) 
   
 

### *3.开始工作*
1. 生成表达式，编译matcher.cpp并生成可执行文件，具体过程可以[参考] (https://github.com/ZhiTingXin/Numa/blob/master/README.md)
2. 输入自己的cpp文件之后，用run.sh运行之后得到输出为./output/**.cc文件，同时得到expression.txt文件，其中是路径覆盖的表达式

 > 接下来的工作,你可以运行 **ktest.sh**

3. 在`example.txt`中，填写你的条件和运算式

4. 编译运行`main.cc`，生成`example.c`文件
5. 编译`example.c`,使用 **klee-fp** 、**ktest-tool**求解，解值在`result.txt`

	```
	clang -I ../../include -emit-llvm -O0 -c -g example.c -o example.bc
	```
	```
	klee example.bc
	```
	```
	#运行脚本
	ktest-tool --write-floats klee-last/$ktest_file
	```
6. 编译运行`analyse.cc`，在**checking**文件夹下生成所有路径的测试
7. 编译运行所有**checking / test**
8. 最后得到我们需要的数据