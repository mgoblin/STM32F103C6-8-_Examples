# Overview

This is a minimal project to demonostrate how to use a hardware interrupt for listening to either a button press, or incremental rotary encoder input. Behavior depends on the contents of the interrupt handler in `EXTI15_10_IRQHandler`.

This demo requires to connect push button to PC14.

If a simple button is connected to pin PC14, the LED can be set to toggle each time it is pressed.


# Uploading and Debugging

This project targets to STM32F103C6T8.

I've tested the program with 'Blue pill' board. To upload the resulting program, you can use your choice of uploader/debugger. I Texane's "stlink" project:

https://github.com/texane/stlink

It's a fairly simple process to upload and debug a program:

1. Plug your board in - for Blue pill board, you can just use ST-LINK V2.

2. Run
```bash
make flash
```

# How to build example

he project is built in the terminal using the command `make` or `make all`. As a build result main.bin file should be created.

Firmware upload described in "Overiew" section. 

Cleaning a project from build artifacts is performed by the command `make clean`.

# Explanation of example internals
Source code structured and some folders:
- device_headers - CMSIS headers
- startup - STM32F103C6T8 system code
- src - application code
- ld - linker script folder

