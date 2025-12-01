# Overview

This is a project with some minimal code for booting a program on an STM32F103C6T8 chip.

A Makefile is provided, and I've tested the program with a 'Blue pill' board with the STM32F103C6T8 chip. To upload the resulting program, you can use your choice of uploader/debugger. I use GDB and Texane's "stlink" project:

https://github.com/texane/stlink

It's a fairly simple process to upload and debug a program on Debian 13:

0. Plug your board in - for ST's blue pill boards, you can just use an ST-Link v2.

1. Run `make flash` in the project directory to build and load code to the MCU the project.

2. Enter `st-util --connect-under-reset` in a terminal window after installing the utilities linked above. You should see output `INFO gdb-server.c: Listening at *:4242....` and waiting.

3. In anoter terminal run `gdb-multiarch main.elf` (or whatever your file is called, if it's not 'main.elf')

4. Point GDB to the port opened by 'st-util' - usually 4242 - with the command: `target extended-remote :4242`

5. Enter `load` to upload your program.

6. You're code is uploaded! Enter `continue` to start it running, and hit `ctrl+C` a few times (maybe followed by 'y' for 'yes') to interrupt it once it's started.

Once the code is uploaded, you can more or less treat the GDB debugging session like you would any other C/C++ program. You can set breakpoints, step through the code with `s`/`si`/`n`/`ni`/etc, inspect variables or memory addresses, and so on.

# How to build example
The project is built in the terminal using the command `make` or `make all`. As a build result main.bin file should be created.

Firmware upload described in "Overiew" section. 

Cleaning a project from build artifacts is performed by the command `make clean`.

# Explanation of example internals
The example contains the following source code files:
 - main.c - Application code example in C language. The code itself does not carry much value and is simply an example of C code. 
 - vector_table.S - assembler lanuage file that defines MCU interrpt handlers table (vector table).
 - core.S - assembler lanuage file that contains reset interrupt handler code

 After the compilation step, all source files generate object files (*.o), and the linker must place them in the correct order.

 Linker file `STM32F103C8T6.ld` describes linker logic.
 The first part is a vector table with two entries. The first entry is stack pointer and the second one address of reset_handler function.

 Reset handler called by the MCU and its source code placed in file core.S is initializes stack, SRAM and etc. At the last step reset_handler function transfer control to main function (in main.c) and application code starts to execute.

