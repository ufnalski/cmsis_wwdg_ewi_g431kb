// By: ufnalski

#include "wwdg.h"
#include "main.h"
#include "stm32g431xx.h"
#include "stm32g4xx_hal_gpio.h"

void WWDG_Init(void) {
  RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN; // enable WWDG clock
  while (!(RCC->APB1ENR1 & RCC_APB1ENR1_WWDGEN)) {
    __NOP();
  }

  WWDG->CFR |=
      (WWDG_CFR_WDGTB_0 | WWDG_CFR_WDGTB_1 |
       WWDG_CFR_WDGTB_2); // 1/(170 MHz / 4096 / 128) = ca. 3.08 ms, reset
  // ca. every 64 * 3.08 ms = ca. 197.1 ms

#ifndef USE_WINDOW
  WWDG->CFR |= (WWDG_CFR_W &
                0x7F); // max window length (no window, default (reset value))
#else
  WWDG->CFR &= ~(WWDG_CFR_W);
  WWDG->CFR |= (WWDG_CFR_W & 0x5F); // half of max window length
#endif

  WWDG->CFR |= WWDG_CFR_EWI; // early wake-up interrupt enable

  NVIC_SetPriority(WWDG_IRQn, 0);
  NVIC_EnableIRQ(WWDG_IRQn);

  WWDG->CR |= WWDG_CR_WDGA; // activate
}

void WWDG_Refresh(void) {
  WWDG->CR |= (WWDG_CR_T & 0x7F); // reload to max
}

void WWDG_IRQHandler(void) {
  if (WWDG->SR & WWDG_SR_EWIF) {
    WWDG->SR &= ~(WWDG_SR_EWIF); // clear interrupt flag
    HAL_GPIO_TogglePin(LOGIC_ANALYZER_PA1_GPIO_Port, LOGIC_ANALYZER_PA1_Pin);
#ifdef RELOAD_IN_EWI
    WWDG_Refresh();
#endif
  }
}
