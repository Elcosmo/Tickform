/* Functional approximate frequency check only. No DS3231 reference. */
#include "xo_counter.h"
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <stdint.h>

int diagnostic_cdc_send(const char *text);
static uint32_t previous_ms, previous_count;
static uint32_t minimum_hz = UINT32_MAX, maximum_hz;
static int ready;
static char report[768];

static void sample(uint32_t *ms, uint32_t *count)
{
    uint32_t mask = __get_PRIMASK();
    __disable_irq(); /* Short adjacent reads; SysTick quantization remains ~1 ms. */
    *ms = HAL_GetTick();
    *count = TIM2->CNT;
    __set_PRIMASK(mask);
}

void xo_counter_init(void)
{
    /* Gate 3 uses APB1 /1, so TIM2 kernel = PCLK1 = 170 MHz. */
    if (HAL_RCC_GetPCLK1Freq() != 170000000U ||
        (RCC->CFGR & RCC_CFGR_PPRE1) != 0U) {
        ready = 0;
        previous_ms = HAL_GetTick();
        GPIOC->BSRR = 1UL << 6;
        return;
    }
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_0;
    gpio.Mode = GPIO_MODE_AF_PP; /* AF input is driven by ETR, not GPIO output. */
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF14_TIM2;
    HAL_GPIO_Init(GPIOA, &gpio);
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM2_FORCE_RESET();
    __HAL_RCC_TIM2_RELEASE_RESET();
    /* Reset AF1 ETRSEL=0 selects external TIM2_ETR pin.
       CKD=0, upcounter, no capture/output channels, no IRQ or DMA. */
    TIM2->CR1 = 0U;
    TIM2->PSC = 0U;
    TIM2->ARR = UINT32_MAX;
    TIM2->SMCR = TIM_SMCR_ECE; /* Mode 2; ETP=0, ETPS=0 (/1), ETF=0. */
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR = 0U;
    TIM2->CNT = 0U;
    TIM2->CR1 = TIM_CR1_CEN;
    ready = (TIM2->SMCR == TIM_SMCR_ECE && TIM2->PSC == 0U &&
             TIM2->ARR == UINT32_MAX && (TIM2->CR1 & TIM_CR1_CEN) != 0U);
    sample(&previous_ms, &previous_count);
}

void xo_counter_poll(void)
{
    uint32_t now, count;
    if (!ready) {
        now = HAL_GetTick();
        if ((uint32_t)(now - previous_ms) >= 1000U) {
            diagnostic_cdc_send("Tickform FASE 3 - XO\r\n" "TIM2 ETR PA0: FAIL configuration\r\n");
            previous_ms = now;
        }
        return;
    }
    sample(&now, &count);
    uint32_t elapsed = now - previous_ms;
    if (elapsed < 1000U) { return; }
    uint32_t edges = count - previous_count; /* Modulo-2^32 handles counter wrap. */
    previous_ms = now;
    previous_count = count;
    /* Reject stalled software windows; avoids ambiguity from multiple wraps. */
    if (elapsed > 10000U) {
        diagnostic_cdc_send("Tickform FASE 3 - XO\r\n" "FAIL: invalid window >10000 ms; resampled\r\n");
        GPIOC->BSRR = 1UL << 6;
        return;
    }
    uint32_t hz = (uint32_t)(((uint64_t)edges * 1000U + elapsed / 2U) / elapsed);
    if (hz < minimum_hz) { minimum_hz = hz; }
    if (hz > maximum_hz) { maximum_hz = hz; }
    /* Consistent activity >=1000 Hz; expected frequency +/-5%, not ppm. */
    int signal = hz >= 1000U;
    int plausible = hz >= 23347200U && hz <= 25804800U;
    snprintf(report, sizeof(report),
             "Tickform FASE 3 - XO\r\n" "TIM2 ETR PA0 AF14: READY\r\n"
             "FUNCTIONAL / APPROXIMATE FREQUENCY CHECK - not ppm metrology\r\n"
             "Window: %lu nominal ms\r\nEdges: %lu\r\n"
             "Estimated frequency: %lu.%06lu MHz\r\n"
             "Signal detected: %s (>=1000 Hz)\r\n"
             "24.576 MHz plausibility: %s (+/-5%%)\r\n"
             "Min/Max since boot: %lu / %lu Hz\r\n\r\n",
             (unsigned long)elapsed, (unsigned long)edges,
             (unsigned long)(hz / 1000000U), (unsigned long)(hz % 1000000U),
             signal ? "PASS" : "FAIL no/low activity",
             plausible ? "PASS" : "FAIL",
             (unsigned long)minimum_hz, (unsigned long)maximum_hz);
    /* Existing CDC copies into persistent TX storage; busy/offline drops this report,
       never stalls the counter. Next window produces a fresh report. */
    diagnostic_cdc_send(report);
    if (plausible) { GPIOC->ODR ^= 1UL << 6; }
    else { GPIOC->BSRR = 1UL << 6; }
}
