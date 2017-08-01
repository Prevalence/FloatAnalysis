## klee-fp安装指南

1. Clone the KLEE-FP repository  </br> **tip : 在git clone klee-fp时，git连接不上[地址](http://www.pcc.me.uk/gitweb/gitweb.cgi)，手动拷贝**
	
	`$ cd $BUILD_ROOT `   </br>
  	`$ mkdir klee-fp ` </br>
  	`$ cd klee-fp ` </br>
  	`$ git clone git://git.pcc.me.uk/~peter/klee-fp.git 	src`

2. Checkout a specific revision of LLVM.

	`$ mkdir $BUILD_ROOT/llvm_and_clang`  </br>
	`$ cd llvm_and_clang ` </br>
	`$ svn co -r 146372 http://llvm.org/svn/llvm-project/llvm/trunk src`  </br>

3. Checkout a specific revision of Clang into LLVM's tools folder.

	`$ cd $BUILD_ROOT/llvm_and_clang/src/tools ` </br>
	`$ svn co -r 146372 http://llvm.org/svn/llvm-project/cfe/trunk clang `

4. Checkout a specific revision of compiler-rt into LLVM's projects folder.

	`$ cd $BUILD_ROOT/llvm_and_clang/src/projects  `</	br>
	`$ svn co -r 146372 http://llvm.org/svn/llvm-	project/compiler-rt/trunk compiler-rt`

5. If you want to use the OpenCL runtime then you need to patch LLVM and Clang. </br> **tip : 按照我们的安装过程来看，这一步必须要完成**

	`$ cd $BUILD_ROOT/llvm_and_clang/src/`  </br>
	`$ patch -p1 <  
	$BUILD_ROOT/klee-fp/src/patches/llvm-Define-the-	KLEE-OpenCL-target.patch ` </br>
	`$ cd $BUILD_ROOT/llvm_and_clang/src/tools/clang`
	`$ patch -p1 <  $BUILD_ROOT/klee-fp/src/patches/clang-Define-the-	KLEE-OpenCL-target.patch ` </br>

6. Build LLVM and Clang. We'll do an out of source build because it
keeps things nice and tidy.

	`$ cd $BUILD_ROOT/llvm_and_clang/  ` </br>
	`$ mkdir bin  `</br>
	`$ cd bin/ ` </br>
	`$ ../src/configure --enable-optimized --enable-	debug-symbols ` </br>

	Now run make. We'll try to run this in parallel. 	Replace -jN with the
	number of jobs you want to run in parallel (e.g. -	j8)

	`$ make -jN`  </br>
	
	**tip : 你可能在编译过程会遇到如下这个问题,我们的解决办法是打开`sysinfo.cc`，删去538行的几个类型名，例如删除‘unsigned long long’**
	> /home/shenweijun/llvm_and_clang/src/projects/compiler-rt/lib/asan/sysinfo/sysinfo.cc:538:32: error:       conversion specifies type 'unsigned long' but the argument has type      'unsigned long long' [-Werror,-Wformat]                          "%08"PRIx64"-%08"PRIx64" %c%c%c%c %08"PRIx64" ..

7. Clone a version of ucblic for KLEE-FP and build it
	</br>**tip : 同样的，手动拷贝[ucblic](http://www.pcc.me.uk/gitweb/gitweb.cgi)**

	`$ cd $BUILD_ROOT`  </br>
	`$ git clone git://git.pcc.me.uk/~peter/klee-	uclibc.git ` </br>
	`$ cd klee-uclibc/ `  </br>

	Because of the particular options we chose to 	configure the LLVM build
	with we need to patch the build system so it finds 	the executables.

	`$ sed -i 's/Debug+Asserts/Release+Debug+Asserts/g' 	Rules.mak.llvm  `</br>

	If you chose to use different configure options for 	LLVM then take a
	look in $BUILD_ROOT/llvm_and_clang/bin and see 	what the name of build
	folder is (where the binaries and built libraries 	go).
	Build uclibc
  
	`$ cd $BUILD_ROOT/klee-uclibc ` </br>
	`$ make`  </br>

	You will get a compilation failure as Clang cannot 	build ctrn.o. It
	will probably look something like...

	clang -cc1as: fatal error: error in backend: Size 	expression must be absolute.

	According to the author of KLEE-FP this object file 	is not needed so
	we can trick the build system in to thinking it's 	already been built
	and continue with the rest of build by doing

	`$ touch lib/crtn.o ` </br>
	`$ make ` </br>
	**tip : 你可能在编译过程会遇到如下这个问题**
	> Makefile.in:15: Rules.mak: No such file or directory  </br>
	
	**解决办法，在make之前 ./configure --with-llvm=`llvm-root-dir`(llvm的路径)** </br>	**要先 configure 之后再 make**
8. Build STP. If you already have mainline KLEE installed you can just
use the version of STP it uses. If not then you build as follows.</br>
	**tip : 作者给出的stp安装有问题，我们使用来自klee官网给出的[stp安装方法](http://klee.github.io/build-stp/), 你可能需要先将你的cmake更新到3.2版本以上**

	`$ cd $BUILD_ROOT/`  </br>
	`$ mkdir stp ` </br>
	`$ cd stp/`  </br>
	`$ mkdir install` </br>
	`$ svn co -r 940  
	https://stp-fast-prover.svn.sourceforge.net/svnroot/	stp-fast-prover/trunk/stp
	src_bin ` </br>
	`$ cd src_bin`  </br>
	`$ ./scripts/configure --with-prefix=$BUILD_ROOT/stp/	install 
	--with-cryptominisat2`
	`$ make OPTIMIZE=-O2 CFLAGS_M32= install ` </br>

9. Patch KLEE-FP. There seems to be a mistmatch between function
declarations in KLEE-FP and on the system I'm using (Ubuntu 12.04) for
getnameinfo() in /usr/include/netdb.h . At the moment you need to
apply a patch (I have attached to this e-mail). Let me know if you can
build without this patch.

	`$ cd $BUILD_ROOT/klee-fp/src ` </br>
	`$ patch -p1 < klee-fp-getnameinfo-header-	mismatch.patch ` </br>

10. Build KLEE-FP. If you don't want the OpenCL runtime make sure you
pass --disable-opencl to configure instead of --enable-opencl.

	`$ cd $BUILD_ROOT/klee-fp/`  </br>
	`$ mkdir bin`  </br>
	`$ cd bin/`  </br>
	`$ ../src/configure --enable-posix-runtime  
		--enable-opencl 
		--with-uclibc=$BUILD_ROOT/klee-uclibc 
		--with-stp=$BUILD_ROOT/stp/install 
		--with-llvmsrc=$BUILD_ROOT/llvm\_and\_clang/src 
		--with-llvmobj=$BUILD_ROOT/llvm\_and\_clang/bin `
	

	Now run make. We'll try to run this in parallel. 	Replace -jN with the
	number of jobs you want to run in parallel (e.g. -	j8)

	`$ make -jN ` </br>
	
	**如果你在之前安装过clang，在使用过程中，你可能会遇到以下问题**
	> KLEE: ERROR: error loading program 'float.bc': Unknown bitstream version </br>
	
	**这是因为你的clang的版本的问题, klee-fp使用的是clang-3.1，就是你在前面步骤安装的那个版本**