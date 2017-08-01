#Step 2
### *1.准备工作*
1. 你应当安装好[klee-fp 作者教程](http://www.mail-archive.com/klee-dev@imperial.ac.uk/msg01034.html)

### *2.开始工作*
1. 在`example.txt`中，填写你的条件和运算式

2. 编译运行`main.cc`，生成`example.c`文件
3. 编译`example.c`,使用 **klee-fp** 、**ktest-tool**求解，解值在`result.txt`

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
4. 编译运行`analyse.cc`，在**checking**文件夹下生成所有路径的测试 
