#ifndef MAIN_H
#define MAIN_H

/* Include the CMSIS device header downloaded from ST's repo */
#include "stm32c031xx.h"

/* The NUCLEO-C031C6 User LED is connected to GPIO Port A, Pin 5 */
#define LED_PIN 5

/* Function prototypes */
void delay_cycles(volatile uint32_t count);

#endif /* MAIN_H */