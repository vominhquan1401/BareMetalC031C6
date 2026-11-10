#include "main.h"

void systick_initialize(void) {
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
}

int main(void) {
  systick_initialize();
  while (1) {
  }

  return 0;
}