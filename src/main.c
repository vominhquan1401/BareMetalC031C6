#include "main.h"

/**
 * @brief Simple blocking delay loop
 */
void delay_cycles(volatile uint32_t count) {
  while (count--) {
    __asm("nop"); /* Prevents aggressive compiler optimization */
  }
}

int main(void) {
  /* 1. Enable the clock for GPIO Port A via the Reset and Clock Control (RCC)
   * register */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  /* 2. Configure PA5 as a General Purpose Output Mode
   * The MODER register allocates 2 bits per pin. Pin 5 uses bits 10 and 11.
   * Output mode is '01'. We clear the mask first, then set the bit.
   */
  GPIOA->MODER &= ~(GPIO_MODER_MODE5_Msk);
  GPIOA->MODER |= (1U << GPIO_MODER_MODE5_Pos);

  /* 3. Infinite application loop */
  while (1) {
    /* Toggle PA5 using the Output Data Register (ODR) */
    GPIOA->ODR ^= GPIO_ODR_OD5;

    /* Wait approximately 500ms (rough cycle count for 48MHz default clock) */
    delay_cycles(2000000);
  }

  return 0;
}