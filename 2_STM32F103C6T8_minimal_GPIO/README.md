# Overview

This is a minimal project which uses CMSIS device header provided by ST to give us access to convenient aliases for peripheral registers.

The example project initializes pin C14 as input with a built-in pull-up resistor; it listens for button presses on a button connected to ground. pin C13 is set to push-pull output, and it is connected to a built-in LED on ST's "Blue pill" board. 

While button pressed LED is blinking.

![electrial_schema](electical_sch.png)

# Uploading and Debugging

A Makefile is provided, and I've tested the program with a 'Blue pill' board using the STM32F103C6T8 chip. To upload the resulting program, you can use your choice of uploader. I use Texane's "stlink" project:

https://github.com/texane/stlink

It's a fairly simple process to upload:

1. Plug your board in via ST-LINK

2. Uplodad firmware
```bash
make flash
```

Once the code is uploaded, you can more or less treat the GDB debugging session like you would any other C/C++ program. You can set breakpoints, step through the code with `s`/`si`/`n`/`ni`/etc, inspect variables or memory addresses, and so on.

# How to build example
The project is built in the terminal using the command `make` or `make all`. As a build result main.bin file should be created.

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

The firmware waits for the button to be pressed and as long as the button is pressed, the LED on the board blinks.

It begins with defining the microcontroller model (`#define STM32F103x6`). This is necessary to select the correct definitions of the microcontroller's peripheral registers in `stm32f1xx.h`. An alternative way is to define the model using the -D switch at compile time (see -D$(MCU) in the Makefile).


Lets see main method now in main.c file. 

To use the processor pins, you must first connect a clock signal to them.
Both LED and pushbutton pin are on port C. 
Call `enable_portc_clock` to connect port C to MCU clock.
```C
static void enable_portc_clock() 
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable clock for PORTC
}
``` 
The second step is to configure the operating mode of the microcontroller pins.
In blue pill board LED is connected to the pin PC13. And external pushbutton is 
connected to PC14 pin. Call `gpio_init()` configure PC13 as digital output push-pull pin and 
PC14 as pullup input pin. Both pins mode is configured by setting bits in GPIOC->CRH MCU register.
Addittional step to pullup PC14 is set corresponding bit in GPIOC->ODR register.
```C
/*
  GPIO initialization

  Setup LED1 pin as push-pull output and Button pin as pull-up input
*/
static void gpio_init(void)
{
    // Set PC13: output mode, 2 MHz, push-pull
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1; // 2 MHz
    // CNF13 = 00 → push-pull

    // Set PC14: input with pull-up
    GPIOC->CRH &= ~(GPIO_CRH_MODE14 | GPIO_CRH_CNF14);
    GPIOC->CRH |= GPIO_CRH_CNF14_1; // CNF14 = 10 → input with PU/PD
    GPIOC->ODR |= (1 << BUTTON_PIN); // Enable pull-up
}
```

Last initialization step is LED off. To LED off PC13 should have high value.
main.c have to functions `led_off` and `led_on` to drive inboard LED. Each of this 
function set or clear corresponding bit in GPIOC->ODR register. ODR register controls
pins output value. 
```C
static void led_off(void)
{
    GPIOC->ODR |= (1 << LED1_PIN);
}

static void led_on(void)
{
    GPIOC->ODR &= ~(1 << LED1_PIN);
}
```

After initialization endless button pin value read cycle started by `while(1)`. 
GPIOC->IDR register contains bits corresponding to port C pins values.

Last part of puzzle is `ms_delay` function.
This function synchronously delays the execution of code by the number of milliseconds passed in its parameter.
For the sake of code simplicity ms_delay implemented as nested cycle of variable decrement. 
This is not good practice use described approach in application. Using a timer is a better approach.
  