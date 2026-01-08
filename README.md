
**Nanyang Technological University**\
**SC2005 Operating Systems**

# About xv6
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix Version 6 (v6). xv6 loosely follows the structure and style of v6, but is implemented for a modern RISC-V multiprocessor using ANSI C.

This version is based on xv6 2023 version by MIT available at `git://g.csail.mit.edu/xv6-labs-2023`. We are grateful for MIT for rendering its teaching operating system widely available. 

Further information about xv6 can found in its online book at https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf.

# Installation 
This version was tested in Ubuntu 22.04 and Ubuntu 24.04. The following tools are required: QEMU 5.1+, GDB 8.3+, GCC, and Binutils. Install with: 
```sh
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu 
```

To build and run xv6, use `make qemu`. 
To quit qemu type: `Ctrl-a x` (press Ctrl and a at the same time, followed by x). 

# Error reports 

Please contact your professors and teaching assistants for any error report. You can also submit pull requests directly in GitHub.