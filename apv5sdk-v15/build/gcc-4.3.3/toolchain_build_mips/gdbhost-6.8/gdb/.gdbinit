echo Setting up the environment for debugging gdb.\n

set complaints 1

b internal_error

b info_command
commands
	silent
	return
end

dir /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gdb-6.8/gdb/../libiberty
dir /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gdb-6.8/gdb/../bfd
dir /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gdb-6.8/gdb
dir .
set prompt (top-gdb) 
