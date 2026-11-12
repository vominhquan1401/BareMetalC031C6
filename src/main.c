#include "main.h"

volatile int32_t TimeDelay;

uint32_t systick_initialize(uint32_t ticks) {
  SysTick->CTRL = 0;
  SysTick->LOAD = (uint32_t)(ticks - 1UL);
  NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
  SysTick->VAL = 0UL;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  return (0UL);
}

void SysTick_Handler(void) {
  if (TimeDelay > 0) {
    TimeDelay--;
  }
}

void delay(uint32_t time_ms) {
  TimeDelay = time_ms;
  while (TimeDelay != 0);
}

void gpio_enable_clock(GPIO_TypeDef *GPIOx) {
  if (GPIOx == GPIOA)
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  else if (GPIOx == GPIOB)
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  else if (GPIOx == GPIOC)
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
  else if (GPIOx == GPIOD)
    RCC->IOPENR |= RCC_IOPENR_GPIODEN;
  else if (GPIOx == GPIOF)
    RCC->IOPENR |= RCC_IOPENR_GPIOFEN;
}

void gpio_init(GPIO_TypeDef *GPIOx, uint8_t pin) {
  gpio_enable_clock(GPIOx);

  GPIOx->MODER &= ~(0x3UL << (pin * 2));
  GPIOx->MODER |= (0x1UL << (pin * 2));
  GPIOx->OTYPER &= ~(0x1UL << pin);
  GPIOx->PUPDR &= ~(0x3UL << (pin * 2));
  GPIOx->ODR = (0x1UL << pin);
}

void gpio_level_set(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t level) {
  if (level) {
    GPIOx->BSRR = (0x1UL << pin);
  } else {
    GPIOx->BSRR = (0x1UL << (pin + 16));
  }
}

void gpio_toggle(GPIO_TypeDef *GPIOx, uint8_t pin) {
  if (GPIOx->ODR & (0x1UL << pin)) {
    GPIOx->BSRR = (0x1UL << (pin + 16));
  } else {
    GPIOx->BSRR = (0x1UL << pin);
  }
}

int main(void) {
  systick_initialize(1000);
  gpio_init(GPIOA, 5);

  while (1) {
    gpio_toggle(GPIOA, 5);
    delay(1000);
  }

  return 0;
}
