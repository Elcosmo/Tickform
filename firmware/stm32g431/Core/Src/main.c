#include "stm32g4xx.h"

/* Required by newlib initialization called from the official ST startup. */
void _init(void) {}

/* Polled 1 ms SysTick: no interrupts and no metrological timing claim. */
static void delay_ms(uint32_t milliseconds)
{
    SysTick->VAL = 0U;
    for (uint32_t elapsed = 0U; elapsed < milliseconds; ++elapsed) {
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) {
            /* Wait for the next wrap. */
        }
    }
}

int main(void)
{
    /* SystemInit from STM32CubeG4 leaves the reset HSI clock unchanged. */
    SystemCoreClockUpdate();
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    (void)RCC->AHB2ENR;

    /* WeAct CxU6: PC6, active-high. Preload OFF before enabling output. */
    GPIOC->BSRR = (1UL << (6U + 16U));
    GPIOC->OTYPER &= ~(1UL << 6U);
    GPIOC->OSPEEDR &= ~(3UL << 12U);
    GPIOC->PUPDR &= ~(3UL << 12U);
    GPIOC->MODER = (GPIOC->MODER & ~(3UL << 12U)) | (1UL << 12U);

    SysTick->LOAD = (SystemCoreClock / 1000U) - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (;;) {
        GPIOC->BSRR = (1UL << 6U);          /* LED ON */
        delay_ms(500U);
        GPIOC->BSRR = (1UL << (6U + 16U)); /* LED OFF */
        delay_ms(500U);
    }
}
