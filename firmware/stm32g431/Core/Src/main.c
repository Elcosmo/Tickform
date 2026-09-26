#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"
void gate4_usb_init(void);

#define TARGET_HZ 170000000UL
#define WAIT_LIMIT 1000000UL
#define PLL_CONFIG (RCC_PLLCFGR_PLLSRC_HSI | (3UL << RCC_PLLCFGR_PLLM_Pos) | \
                    (85UL << RCC_PLLCFGR_PLLN_Pos) | RCC_PLLCFGR_PLLREN)

/* Debugger-visible result: 0 configuring, 170 success, 1..7 failure. */
volatile uint32_t gate3_status;
volatile uint32_t gate3_cfgr;
volatile uint32_t gate3_pllcfgr;
volatile uint32_t gate3_flash_acr;
volatile uint32_t gate3_pwr_cr1;
volatile uint32_t gate3_pwr_cr5;

/* Required by newlib initialization called from the official ST startup. */
void _init(void) {}

static int wait_bits(volatile uint32_t *reg, uint32_t mask, uint32_t expected)
{
    for (uint32_t remaining = WAIT_LIMIT; remaining != 0U; --remaining) {
        if ((*reg & mask) == expected) {
            return 1;
        }
    }
    return 0;
}

static void snapshot(void)
{
    gate3_cfgr = RCC->CFGR;
    gate3_pllcfgr = RCC->PLLCFGR;
    gate3_flash_acr = FLASH->ACR;
    gate3_pwr_cr1 = PWR->CR1;
    gate3_pwr_cr5 = PWR->CR5;
}

static void failure(uint32_t code)
{
    gate3_status = code;
    snapshot();
    SysTick->CTRL = 0U;
    GPIOC->BSRR = (1UL << 6U); /* Explicit failure: LED continuously ON. */
    for (;;) {
        __NOP(); /* Halt, no automatic reset or silent fallback to HSI blink. */
    }
}

static void clock_170mhz(void)
{
    /* This diagnostic must start after reset, on HSI, as Gate 2 did. */
    RCC->CR |= RCC_CR_HSION;
    if (!wait_bits(&RCC->CR, RCC_CR_HSIRDY, RCC_CR_HSIRDY) ||
        (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
        failure(1U);
    }

    /* Range 1 Boost: R1MODE=0, VOS=01. Keep flash safe before acceleration. */
    PWR->CR5 &= ~PWR_CR5_R1MODE;
    PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | PWR_CR1_VOS_0;
    if (!wait_bits(&PWR->SR2, PWR_SR2_VOSF, 0U)) {
        failure(2U);
    }
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_4WS;
    if (!wait_bits(&FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS)) {
        failure(3U);
    }

    RCC->CR &= ~RCC_CR_PLLON;
    if (!wait_bits(&RCC->CR, RCC_CR_PLLRDY, 0U)) {
        failure(4U);
    }
    /* M=4 (encoded 3), N=85, R=2 (encoded 0), R enabled; P/Q disabled. */
    RCC->PLLCFGR = PLL_CONFIG;
    RCC->CR |= RCC_CR_PLLON;
    if (!wait_bits(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY)) {
        failure(5U);
    }

    /* Official G431 LL template: intermediate AHB /2 for upward transition. */
    RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 |
                              RCC_CFGR_PPRE2)) | RCC_CFGR_HPRE_DIV2;
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    if (!wait_bits(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL)) {
        failure(6U);
    }
    /* >=170 NOP cycles at 85 MHz: >=2 us, exceeding required 1 us dwell.
       Loop overhead only increases this delay; no extra timer is configured. */
    for (uint32_t cycles = 0U; cycles < 170U; ++cycles) {
        __NOP();
    }
    RCC->CFGR &= ~RCC_CFGR_HPRE; /* Final HCLK, PCLK1, PCLK2 all /1. */
    __DSB();
    __ISB();
    SystemCoreClockUpdate();
    snapshot();
    if (RCC->PLLCFGR != PLL_CONFIG ||
        (RCC->CR & RCC_CR_PLLRDY) == 0U ||
        (RCC->CFGR & (RCC_CFGR_SWS | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 |
                      RCC_CFGR_PPRE2)) != RCC_CFGR_SWS_PLL ||
        (PWR->CR1 & PWR_CR1_VOS) != PWR_CR1_VOS_0 ||
        (PWR->CR5 & PWR_CR5_R1MODE) != 0U ||
        (FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS ||
        SystemCoreClock != TARGET_HZ) {
        failure(7U);
    }
    gate3_status = 170U;
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void Error_Handler(void)
{
    __disable_irq();
    failure(8U); /* Gate 4 HAL/USB initialization failed. */
}

int main(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    (void)RCC->AHB2ENR;
    GPIOC->BSRR = (1UL << (6U + 16U));
    GPIOC->OTYPER &= ~(1UL << 6U);
    GPIOC->OSPEEDR &= ~(3UL << 12U);
    GPIOC->PUPDR &= ~(3UL << 12U);
    GPIOC->MODER = (GPIOC->MODER & ~(3UL << 12U)) | (1UL << 12U);
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    (void)RCC->APB1ENR1;

    clock_170mhz();
    /* HAL SysTick runs at 1 ms with reload 169999 after the unchanged Gate 3 clock setup. */
    if (HAL_Init() != HAL_OK || SysTick->LOAD != 169999U) {
        Error_Handler();
    }
    gate4_usb_init();
    for (;;) {
        GPIOC->BSRR = (1UL << 6U);
        HAL_Delay(500U);
        GPIOC->BSRR = (1UL << (6U + 16U));
        HAL_Delay(500U);
    }
}
