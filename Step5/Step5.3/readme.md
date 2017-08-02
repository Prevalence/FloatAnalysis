#将插入语句后的文件用不同的测试数值测试后的运行结果分别写入input_one.txt和input_two.txt
#输入的样例如下
	hhh.cpp(文件名)
	line:2,statement:c==3,bool:1
	line:13,statement:result==false,bool:0
#运行launcher，即可得到输出结果
#输出的样例如下
	hhh.cpp:(出现error)
	Error occur at 1 branch and 1 line!
	First c==3 is false while second c==3 is true
	
	hhh.cpp:(没有error)
	There are no errors in this file!
	******