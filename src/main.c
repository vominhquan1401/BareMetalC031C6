#include "main.h"

volatile int32_t TimeDelay;

uint32_t systick_initialize(uint32_t ticks) {
  SysTick->CTRL = 0;
  SysTick->LOAD = (uint32_t)(ticks - 1UL);
  NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
  SysTick->VAL = 0UL;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
                  SysTick_CTRL_ENABLE_Msk;
  return (0UL);
}

void SysTick_Handler(void) {
  if (TimeDelay > 0) {
    TimeDelay--;
  }
}

void delay(uint32_t time_ms) {
  TimeDelay = time_ms;
  while (TimeDelay != 0)
    ;
}
void gpio_init() {
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  GPIOA->MODER &= ~GPIO_MODER_MODE5;
  GPIOA->MODER |= GPIO_MODER_MODE5_0;

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;

  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;

  GPIOA->ODR &= GPIO_ODR_OD5;
}

void gpio_set() { GPIOA->BSRR = GPIO_BSRR_BS5; }

void gpio_reset() { GPIOA->BSRR = GPIO_BSRR_BR5; }

int main(void) {
  systick_initialize(1000);
  gpio_init();

  while (1) {
    gpio_set();
    delay(1000);
    gpio_reset();
    delay(1000);
  }
  return 0;
}