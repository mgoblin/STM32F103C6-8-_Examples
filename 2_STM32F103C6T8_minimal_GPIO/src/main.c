#include "stm32f1xx.h"
#include <stm32f103x6.h>

#define LED1_PIN (13)   // LED1 is connected to PC13 and has LOW level to turn on
#define BUTTON_PIN (14) // Button is connected to PC14 and has LOW level when pressed
#define BUTTON_VALUE_MASK (1 << BUTTON_PIN)

#define BLINK_DELAY_MS 500

/*
  Enables the clock for GPIO Port C.

  This function sets the IOPCEN bit in the RCC APB2ENR register to enable
  the clock for GPIO Port C. The clock must be enabled before configuring
  or accessing any pins on this port.
*/
static void enable_portc_clock() 
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable clock for PORTC
}

/*
  Delay function

  Blocking delay is not good approach and use only to 
  simplify example code.
*/
static void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=500;
      while (x-- > 0)
         __asm("nop");
   }
}

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

static void led_off(void)
{
    GPIOC->ODR |= (1 << LED1_PIN);
}

static void led_on(void)
{
    GPIOC->ODR &= ~(1 << LED1_PIN);
}

int main(void)
{
    enable_portc_clock();

    // Configure LED1 and Button
    gpio_init();

    led_off();

    while (1)
    {
        // Check if button is pressed
        // For simplicity and demo purposes only, we dont use button debounce
        // Without debounce, button press may trigger multiple blinks.
        // In this case debounce is not needed because one blink iteration has 
        // approximate 1 second duration and ms_delay blocks button polling.
        if (!(GPIOC->IDR & BUTTON_VALUE_MASK))
        {
            // LED1 blinks once per press
            led_off();
            ms_delay(BLINK_DELAY_MS);
            led_on();
            ms_delay(BLINK_DELAY_MS);
        } 
        else 
        {
            led_off();
        }
    }
}