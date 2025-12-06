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

System code, linker script and headers copied from CMSIS distribution.

All application code placed to main.c file.

Firmware waits encoder button is pressed or encoder is rotated and LED blink once.

It begins with defining the microcontroller model (`#define STM32F103x6`). This is necessary to select the correct definitions of the microcontroller's peripheral registers in `stm32f1xx.h`. An alternative way is to define the model using the -D switch at compile time (see -D$(MCU) in the Makefile).

Lets see main method now in main.c file. 

To use the processor pins, you must first connect a clock signal to them.
Both LED and encoder pins are on port C. Extended Interrupt/Event Controller needs to connect a clock signal too.

Call `void clock_init()` to connect port C and AFIO (register for EXTI bus) to MCU clock.

```C
static void clock_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable clock for PORT C
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable clock for AFIO
}
```

The second step is to configure the operating mode of the microcontroller pins. Call `gpio_init()` configure pins. See STM32F103C6T8_minimal_GPIO example for gpio details.



