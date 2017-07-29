### rose安装指南

- 首先请确定处于Linux环境并且该环境的硬件设施满足4G以上内存和10G以上磁盘空间



1. **下载Rose**

   直接使用git clone就好，命令为`git clone https://github.com/rose-compiler/rose`

   ​

2. build rose

   在rose文件夹中打开终端，输入`./build`

   这时可能会出现错误，提示缺少一些组件（例如flex和bison和automake），提示什么就装什么就好

   ​

3. configure rose

   为了configure成功，必须先安装Boost库

   请参照[rose的boost安装指南](http://rosecompiler.org/ROSE_HTML_Reference/group__installing__boost.html)

   注意：

   - 推荐使用1.51版本的rose，因为经过尝试，高版本的boost似乎极易出现问题，而现版本rose会检查boost的版本是否高于1.51。

   - 直接使用`wget`下载boost可能比较慢，可以直接下载一个boost的压缩包

   - 直接解压下来的boost并不是我们需要的文件，我们需要新建一个文件夹，假设取名为boost_installation

     路径为xx/boost_installation，那么boost安装命令中的第一句BOOST_ROOT的值就应该为xx/boost_installation

   - 如果你使用的确实是1.51版本的boost，别忘了网页中的提示**When compiling boost versions 1.53 and earlier, add "-sNO_COMPRESSION=1 -sNO_ZLIB=1" to the "b2" command before or after the other "-s" switch.**记得要加上这部分的参数

   - 网页中的命令都是在下载下来的boost解压后的文件夹内执行	

   **安装boost成功后boost_installation文件夹下会出现include和lib文件夹**

   boost安装成功后准备configure

   在rose文件夹下新建一个文件夹，取名假设为rose_build，进入此文件夹，输入命令

   `ROSE_SRC=/absolute/path/to/the/rose/source/tree`

   `BOOST_ROOT=/path/to/my/boost/installation/directory`

   `export LD_LIBRARY_PATH="$BOOST_ROOT/lib:$LD_LIBRARY_PATH"`

   `$ROSE_SRC/configure --prefix=$HOME/my-rose-installation --enable-languages=c,c++ --with-boost=$BOOST_ROOT`

   **第一句的ROSE_SRC的值就是rose文件夹所在的目录(xx/xx/rose)**

   **第二句的BOOST_ROOT的值为boost_installation所在的目录(xx/boost_installation)**

   如果boost没有出问题的话基本这一步是OK的，boost出错请检查boost的安装路径和版本。

4. 正式安装rose

   **在正式安装rose之前我们需要确认gcc的版本，可能需要降低，因为在安装rose途中可能会自动联网安装edg，而如果gcc版本太高会导致自动计算的链接无效，所以为了确保万无一失，请将gcc版本降低**

   参考链接为[rose的gcc降级参考](https://github.com/rose-compiler/rose/blob/master/docs/readmes/README.Debian)

   该页面的命令不用全部执行，只要执行到`g++ --version`后看到gcc版本确实为4.8即可

   最后终于可以开始安装rose了，请检查网络情况，机器电量，磁盘空间，内存占用，4G内存大概需要4小时的时间

   在rose_build目录下输入`make install-rose-library`

   屏幕疯狂滚动

   四个小时过去了

   成功了

   **如果中途出现boost::xxxxxx undefined reference的话，将`BOOST_ROOT=/path/to/my/boost/installation/directory`和`export LD_LIBRARY_PATH="$BOOST_ROOT/lib:$LD_LIBRARY_PATH"`再敲一遍就好**

   ​