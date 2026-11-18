/*
 * Tickform FASE 5 — Variante Diagnostica F5-D02
 * Target: WeAct Studio STM32G431 Core Board V1.0 (STM32G431CBU6)
 * Scopo: Diagnostica interna a registri SPI2/I2S2 -> DMAMUX1 -> DMA1 Channel 1
 */

#include "i2s_rx_diag.h"
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int diagnostic_cdc_send(const char *text);

/* Peripherals */
static I2S_HandleTypeDef hi2s2;
static DMA_HandleTypeDef hdma_i2s2_rx;

/* Static circular DMA buffer in standard SRAM */
static uint16_t i2s_dma_buffer[I2S_RX_BUFFER_HALFWORDS] __attribute__((aligned(4)));

/* ISR counters and flags */
static volatile uint32_t s_half_cplt_count = 0;
static volatile uint32_t s_full_cplt_count = 0;
static volatile uint32_t s_error_callback_count = 0;
static volatile uint32_t s_ovr_count = 0;
static volatile uint32_t s_last_hal_error = 0;
static volatile uint32_t s_raw_irq_dma_count = 0;
static volatile uint32_t s_raw_irq_spi_count = 0;

/* Test states */
enum {
    TEST_STATE_IDLE = 0,
    TEST_STATE_RUNNING,
    TEST_STATE_COMPLETED
};

static int s_test_state = TEST_STATE_IDLE;
static uint32_t s_start_tick = 0;
static uint32_t s_last_report_tick = 0;
static uint32_t s_last_led_tick = 0;
static int s_mid_snapshot_done = 0;
static uint32_t s_cndtr_prev = 0;

static i2s_diag_metrics_t s_metrics;
static const char *s_classification = "";
static const char *s_analysis_detail = "";
static int s_tx_stage = 0;              /* 0: idle / wait for cycle; 1..5: transmitting part 1..5 */
static uint32_t s_last_part_tick = 0;   /* Tick when the last part was accepted */
static uint32_t s_tx_stage_start_tick = 0; /* Tick when current stage was entered (for timeout) */
static char s_seg_buffer[1024];         /* Single scratch buffer for the active part */

/* =========================================================================
 * Interrupt Handlers
 * ========================================================================= */

void DMA1_Channel1_IRQHandler(void)
{
    s_raw_irq_dma_count++;
    HAL_DMA_IRQHandler(&hdma_i2s2_rx);
}

void SPI2_IRQHandler(void)
{
    s_raw_irq_spi_count++;
    HAL_I2S_IRQHandler(&hi2s2);
}

/* =========================================================================
 * HAL I2S Callbacks
 * ========================================================================= */

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2) {
        s_half_cplt_count++;
    }
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2) {
        s_full_cplt_count++;
    }
}

void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2) {
        s_error_callback_count++;
        s_last_hal_error = hi2s->ErrorCode;
        if ((hi2s->ErrorCode & HAL_I2S_ERROR_OVR) != 0U) {
            s_ovr_count++;
        }
    }
}

/* =========================================================================
 * Snapshot Helper (DOES NOT read SPI2->DR to preserve FIFO)
 * ========================================================================= */

static void capture_snapshot(i2s_diag_snapshot_t *snap, uint32_t tick)
{
    snap->tick_ms = tick;
    snap->spi2_cr1 = SPI2->CR1;
    snap->spi2_cr2 = SPI2->CR2;
    snap->spi2_sr = SPI2->SR;
    snap->spi2_i2scfgr = SPI2->I2SCFGR;
    snap->spi2_i2spr = SPI2->I2SPR;
    snap->dma1_isr = DMA1->ISR;
    snap->dma1_ccr = DMA1_Channel1->CCR;
    snap->dma1_cndtr = DMA1_Channel1->CNDTR;
    snap->dma1_cpar = DMA1_Channel1->CPAR;
    snap->dma1_cmar = DMA1_Channel1->CMAR;
    snap->dmamux1_ccr = DMAMUX1_Channel0->CCR;
    snap->gpiob_moder = GPIOB->MODER;
    snap->gpiob_afrh = GPIOB->AFR[1];
    snap->gpiob_idr = GPIOB->IDR;
    snap->rcc_ahb1enr = RCC->AHB1ENR;
    snap->rcc_ahb2enr = RCC->AHB2ENR;
    snap->rcc_apb1enr1 = RCC->APB1ENR1;
    snap->nvic_dma1_ch1_enabled = NVIC_GetEnableIRQ(DMA1_Channel1_IRQn);
    snap->nvic_spi2_enabled = NVIC_GetEnableIRQ(SPI2_IRQn);
    snap->raw_irq_dma_count = s_raw_irq_dma_count;
    snap->raw_irq_spi_count = s_raw_irq_spi_count;
}

/* =========================================================================
 * Initialization
 * ========================================================================= */

void i2s_rx_diag_init(void)
{
    /* 1. Reset counters and state */
    s_half_cplt_count = 0;
    s_full_cplt_count = 0;
    s_error_callback_count = 0;
    s_ovr_count = 0;
    s_last_hal_error = 0;
    s_raw_irq_dma_count = 0;
    s_raw_irq_spi_count = 0;
    s_mid_snapshot_done = 0;
    s_classification = "";
    s_analysis_detail = "";
    s_tx_stage = 0;
    s_last_part_tick = 0;
    s_tx_stage_start_tick = 0;
    memset(&s_metrics, 0, sizeof(s_metrics));
    memset(i2s_dma_buffer, 0, sizeof(i2s_dma_buffer));

    /* 2. Enable peripheral clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_SPI2_CLK_ENABLE();

    /* 3. Configure GPIO pins for I2S2 (AF5):
     *    PB12 -> I2S2_WS (AF5)
     *    PB13 -> I2S2_CK (AF5)
     *    PB15 -> I2S2_SD (AF5)
     */
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &gpio);

    /* 4. Configure DMA1 Channel 1 for SPI2_RX */
    hdma_i2s2_rx.Instance = DMA1_Channel1;
    hdma_i2s2_rx.Init.Request = DMA_REQUEST_SPI2_RX;
    hdma_i2s2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2s2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2s2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2s2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_i2s2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_i2s2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_i2s2_rx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_i2s2_rx) != HAL_OK) {
        GPIOC->BSRR = (1UL << 6U);
        return;
    }

    __HAL_LINKDMA(&hi2s2, hdmarx, hdma_i2s2_rx);

    /* 5. Configure NVIC for DMA and SPI2 */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_SetPriority(SPI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);

    /* 6. Configure SPI2 in I2S Slave Receiver mode */
    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_SLAVE_RX;
    hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_DEFAULT;
    hi2s2.Init.CPOL = I2S_CPOL_LOW;

    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
        GPIOC->BSRR = (1UL << 6U);
        return;
    }

    /* 7. Start circular DMA reception */
    if (HAL_I2S_Receive_DMA(&hi2s2, (uint16_t *)i2s_dma_buffer, I2S_RX_BUFFER_SLOTS) != HAL_OK) {
        GPIOC->BSRR = (1UL << 6U);
        return;
    }

    /* 8. Enable SPI error interrupt */
    __HAL_I2S_ENABLE_IT(&hi2s2, I2S_IT_ERR);

    /* 9. Mark test as running and capture initial snapshot */
    s_start_tick = HAL_GetTick();
    s_last_report_tick = s_start_tick;
    s_last_led_tick = s_start_tick;

    capture_snapshot(&s_metrics.snap_init, 0U);
    s_metrics.cndtr_initial = s_metrics.snap_init.dma1_cndtr;
    s_metrics.cndtr_min = s_metrics.cndtr_initial;
    s_metrics.cndtr_max = s_metrics.cndtr_initial;
    s_cndtr_prev = s_metrics.cndtr_initial;
    s_metrics.cndtr_changes = 0U;

    s_metrics.expected_cpar = (uint32_t)&(SPI2->DR);
    s_metrics.expected_cmar = (uint32_t)i2s_dma_buffer;
    s_metrics.cpar_match = (s_metrics.snap_init.dma1_cpar == s_metrics.expected_cpar);
    s_metrics.cmar_match = (s_metrics.snap_init.dma1_cmar == s_metrics.expected_cmar);

    s_test_state = TEST_STATE_RUNNING;
}

/* =========================================================================
 * Pin Input Verification Helper for F5-D03 (Non-invasive GPIO IDR Polling)
 * Leaves PB12 (WS) and PB13 (CK) in AF mode; reads GPIOB->IDR in tight loop.
 * ========================================================================= */

static void run_pin_input_test(i2s_diag_pin_test_t *t)
{
    /* Enable DWT cycle counter for precise microsecond timing */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Target window: 10 ms (10000 us).
     * At 170 MHz, 10 ms = 1,700,000 core clock cycles. */
    uint32_t target_cycles = 1700000U;

    uint32_t init_idr = GPIOB->IDR;
    uint32_t last_b12 = (init_idr >> 12) & 1U;
    uint32_t last_b13 = (init_idr >> 13) & 1U;

    t->pb12_init_level = (uint8_t)last_b12;
    t->pb13_init_level = (uint8_t)last_b13;
    t->pb12_saw_high = (uint8_t)last_b12;
    t->pb12_saw_low = (uint8_t)(!last_b12);
    t->pb13_saw_high = (uint8_t)last_b13;
    t->pb13_saw_low = (uint8_t)(!last_b13);
    t->pb12_transitions = 0U;
    t->pb13_transitions = 0U;
    t->sample_count = 0U;

    uint32_t start_cycles = DWT->CYCCNT;
    while ((uint32_t)(DWT->CYCCNT - start_cycles) < target_cycles) {
        uint32_t idr = GPIOB->IDR;
        uint32_t b12 = (idr >> 12) & 1U;
        uint32_t b13 = (idr >> 13) & 1U;

        if (b12 != last_b12) {
            t->pb12_transitions++;
            last_b12 = b12;
        }
        if (b13 != last_b13) {
            t->pb13_transitions++;
            last_b13 = b13;
        }

        if (b12 != 0U) {
            t->pb12_saw_high = 1U;
        } else {
            t->pb12_saw_low = 1U;
        }

        if (b13 != 0U) {
            t->pb13_saw_high = 1U;
        } else {
            t->pb13_saw_low = 1U;
        }

        t->sample_count++;
    }
    uint32_t elapsed_cycles = DWT->CYCCNT - start_cycles;

    t->pb12_final_level = (uint8_t)last_b12;
    t->pb13_final_level = (uint8_t)last_b13;

    /* Calculate duration in microseconds */
    uint32_t sysclk_mhz = SystemCoreClock / 1000000U;
    if (sysclk_mhz == 0U) {
        sysclk_mhz = 170U;
    }
    t->duration_us = elapsed_cycles / sysclk_mhz;

    /* Calculate sampling rate in kSps (samples * 1000 / us) */
    if (t->duration_us > 0U) {
        t->sample_rate_ksps = (uint32_t)(((uint64_t)t->sample_count * 1000ULL) / t->duration_us);
    } else {
        t->sample_rate_ksps = 0U;
    }
}

/* =========================================================================
 * Segmented Diagnostic Report Formatter (strictly <= 1000 B per part)
 * ========================================================================= */

static int format_report_part(int part, char *buf, size_t max_len)
{
    switch (part) {
    case 1:
        return snprintf(buf, max_len,
            "==================================================\r\n"
            "TICKFORM FASE 5 - DIAGNOSTICA F5-D03 (PARTE 1/6)\r\n"
            "SPI2/I2S2 -> DMAMUX1 -> DMA1 Channel 1 + GPIO IDR\r\n"
            "==================================================\r\n"
            "Duration:     %lu ms (nominal 10000 ms)\r\n"
            "Master:       PCM1808 C (96 kHz nominal)\r\n"
            "Pins:         PB12=WS (LRCK), PB13=CK (BCK), PB15=SD (DOUT)\r\n"
            "DMA Buffer:   1024 slots (2048 halfwords = 4096 B)\r\n\r\n"
            "--- STATO DIAGNOSTICO PRINCIPALE ---\r\n"
            "CLASSIFICAZIONE: [%s]\r\n"
            "ANALISI:         %s\r\n\r\n",
            (unsigned long)s_metrics.duration_ms,
            s_classification,
            s_analysis_detail);

    case 2:
        return snprintf(buf, max_len,
            "--- PARTE 2/6: CONTATORI EVENTI, CNDTR ED INDIRIZZI DMA ---\r\n"
            "--- CONTATORI EVENTI E INTERRUPT ---\r\n"
            "DMA Half Cplt:       %lu\r\n"
            "DMA Full Cplt:       %lu\r\n"
            "DMA Total CB:        %lu (attesi ~3750 a 96 kHz)\r\n"
            "Raw IRQ DMA1_Ch1:    %lu (ingressi prima di HAL)\r\n"
            "Raw IRQ SPI2:        %lu (ingressi prima di HAL)\r\n"
            "HAL I2S Error CB:    %lu (last=0x%08lX)\r\n"
            "SPI2 OVR contati:    %lu\r\n\r\n"
            "--- MONITORAGGIO CNDTR (DMA1 Channel 1) ---\r\n"
            "CNDTR Iniziale:      %lu (nominale 2048)\r\n"
            "CNDTR Minimo:        %lu\r\n"
            "CNDTR Massimo:       %lu\r\n"
            "CNDTR Pre-Stop:      %lu\r\n"
            "CNDTR Post-Stop:     %lu\r\n"
            "CNDTR Variazioni:    %lu\r\n\r\n"
            "--- INDIRIZZI DMA1 CHANNEL 1 (CPAR / CMAR) ---\r\n"
            "CPAR letto:          0x%08lX (atteso 0x%08lX -> %s)\r\n"
            "CMAR letto:          0x%08lX (atteso 0x%08lX -> %s)\r\n\r\n",
            (unsigned long)s_metrics.half_cplt_count,
            (unsigned long)s_metrics.full_cplt_count,
            (unsigned long)s_metrics.total_callbacks,
            (unsigned long)s_metrics.raw_irq_dma_count,
            (unsigned long)s_metrics.raw_irq_spi_count,
            (unsigned long)s_metrics.error_callback_count,
            (unsigned long)s_metrics.last_hal_error,
            (unsigned long)s_metrics.ovr_count,
            (unsigned long)s_metrics.cndtr_initial,
            (unsigned long)s_metrics.cndtr_min,
            (unsigned long)s_metrics.cndtr_max,
            (unsigned long)s_metrics.cndtr_pre_stop,
            (unsigned long)s_metrics.cndtr_post_stop,
            (unsigned long)s_metrics.cndtr_changes,
            (unsigned long)s_metrics.snap_init.dma1_cpar,
            (unsigned long)s_metrics.expected_cpar,
            s_metrics.cpar_match ? "CONFORME" : "NON CONFORME",
            (unsigned long)s_metrics.snap_init.dma1_cmar,
            (unsigned long)s_metrics.expected_cmar,
            s_metrics.cmar_match ? "CONFORME" : "NON CONFORME");

    case 3:
        return snprintf(buf, max_len,
            "--- PARTE 3/6: VERIFICA CONFIGURAZIONE REGISTRI ---\r\n"
            "RCC SPI2 Clock:      %s (APB1ENR1=0x%08lX)\r\n"
            "RCC DMA1 Clock:      %s (AHB1ENR=0x%08lX)\r\n"
            "RCC DMAMUX1 Clock:   %s (AHB1ENR=0x%08lX)\r\n"
            "RCC GPIOB Clock:     %s (AHB2ENR=0x%08lX)\r\n"
            "NVIC DMA1_Ch1 En:    %s\r\n"
            "NVIC SPI2 En:        %s\r\n"
            "DMAMUX1_Ch0 ReqID:   %lu (atteso 12 = SPI2_RX) -> %s\r\n"
            "PB12 (WS):           MODER=%lu (AF), AFRH=%lu (AF5) -> %s\r\n"
            "PB13 (CK):           MODER=%lu (AF), AFRH=%lu (AF5) -> %s\r\n"
            "PB15 (SD):           MODER=%lu (AF), AFRH=%lu (AF5) -> %s\r\n\r\n",
            (s_metrics.snap_init.rcc_apb1enr1 & RCC_APB1ENR1_SPI2EN) ? "ABILITATO" : "DISABILITATO",
            (unsigned long)s_metrics.snap_init.rcc_apb1enr1,
            (s_metrics.snap_init.rcc_ahb1enr & RCC_AHB1ENR_DMA1EN) ? "ABILITATO" : "DISABILITATO",
            (unsigned long)s_metrics.snap_init.rcc_ahb1enr,
            (s_metrics.snap_init.rcc_ahb1enr & RCC_AHB1ENR_DMAMUX1EN) ? "ABILITATO" : "DISABILITATO",
            (unsigned long)s_metrics.snap_init.rcc_ahb1enr,
            (s_metrics.snap_init.rcc_ahb2enr & RCC_AHB2ENR_GPIOBEN) ? "ABILITATO" : "DISABILITATO",
            (unsigned long)s_metrics.snap_init.rcc_ahb2enr,
            s_metrics.snap_init.nvic_dma1_ch1_enabled ? "ABILITATO" : "DISABILITATO",
            s_metrics.snap_init.nvic_spi2_enabled ? "ABILITATO" : "DISABILITATO",
            (unsigned long)(s_metrics.snap_init.dmamux1_ccr & DMAMUX_CxCR_DMAREQ_ID),
            ((s_metrics.snap_init.dmamux1_ccr & DMAMUX_CxCR_DMAREQ_ID) == 12U) ? "CONFORME" : "NON CONFORME",
            (unsigned long)((s_metrics.snap_init.gpiob_moder >> 24) & 3U),
            (unsigned long)((s_metrics.snap_init.gpiob_afrh >> 16) & 0xFU),
            (((s_metrics.snap_init.gpiob_moder >> 24) & 3U) == 2U && ((s_metrics.snap_init.gpiob_afrh >> 16) & 0xFU) == 5U) ? "CONFORME" : "NON CONFORME",
            (unsigned long)((s_metrics.snap_init.gpiob_moder >> 26) & 3U),
            (unsigned long)((s_metrics.snap_init.gpiob_afrh >> 20) & 0xFU),
            (((s_metrics.snap_init.gpiob_moder >> 26) & 3U) == 2U && ((s_metrics.snap_init.gpiob_afrh >> 20) & 0xFU) == 5U) ? "CONFORME" : "NON CONFORME",
            (unsigned long)((s_metrics.snap_init.gpiob_moder >> 30) & 3U),
            (unsigned long)((s_metrics.snap_init.gpiob_afrh >> 28) & 0xFU),
            (((s_metrics.snap_init.gpiob_moder >> 30) & 3U) == 2U && ((s_metrics.snap_init.gpiob_afrh >> 28) & 0xFU) == 5U) ? "CONFORME" : "NON CONFORME");

    case 4: {
        uint32_t exp_pb12_trans = (96000ULL * 2ULL * (uint64_t)s_metrics.pin_test.duration_us) / 1000000ULL;
        uint32_t msps_int = s_metrics.pin_test.sample_rate_ksps / 1000U;
        uint32_t msps_dec = (s_metrics.pin_test.sample_rate_ksps % 1000U) / 100U;

        const char *pb12_status = "NON DETERMINATO";
        if (s_metrics.pin_test.pb12_transitions > 0U && s_metrics.pin_test.pb12_saw_high && s_metrics.pin_test.pb12_saw_low) {
            pb12_status = "TRANSIZIONI E ENTRAMBI I LIVELLI RILEVATI";
        } else if (s_metrics.pin_test.pb12_transitions == 0U) {
            if (s_metrics.pin_test.pb12_saw_high && !s_metrics.pin_test.pb12_saw_low) {
                pb12_status = "FISSO A LIVELLO HIGH (0 transizioni)";
            } else if (!s_metrics.pin_test.pb12_saw_high && s_metrics.pin_test.pb12_saw_low) {
                pb12_status = "FISSO A LIVELLO LOW (0 transizioni)";
            } else {
                pb12_status = "NESSUNA TRANSIZIONE RILEVATA";
            }
        } else {
            pb12_status = "TRANSIZIONI RILEVATE (LIVELLI PARZIALI)";
        }

        const char *pb13_status = "NON DETERMINATO";
        if (s_metrics.pin_test.pb13_transitions > 0U && s_metrics.pin_test.pb13_saw_high && s_metrics.pin_test.pb13_saw_low) {
            pb13_status = "TRANSIZIONI E ENTRAMBI I LIVELLI RILEVATI";
        } else if (s_metrics.pin_test.pb13_transitions == 0U) {
            if (s_metrics.pin_test.pb13_saw_high && !s_metrics.pin_test.pb13_saw_low) {
                pb13_status = "FISSO A LIVELLO HIGH (0 transizioni)";
            } else if (!s_metrics.pin_test.pb13_saw_high && s_metrics.pin_test.pb13_saw_low) {
                pb13_status = "FISSO A LIVELLO LOW (0 transizioni)";
            } else {
                pb13_status = "NESSUNA TRANSIZIONE RILEVATA";
            }
        } else {
            pb13_status = "TRANSIZIONI RILEVATE (LIVELLI PARZIALI)";
        }

        return snprintf(buf, max_len,
            "--- PARTE 4/6: VERIFICA DIGITALE INGRESSI PB12/PB13 (GPIO IDR) ---\r\n"
            "Finestra misura:     %lu us (campioni totali: %lu)\r\n"
            "Freq. campionamento: %lu kSps (~%lu.%01lu MSps)\r\n"
            "Modalita' pin:       AF5 SPI2 (MODER=AF, nessun cambio configurazione)\r\n\r\n"
            "PB12 (LRCK / WS - nominale 96 kHz):\r\n"
            "  Transizioni:       %lu (attese ~%lu in finestra)\r\n"
            "  Livelli visti:     HIGH=%s, LOW=%s (init=%u, final=%u)\r\n"
            "  Stato segnale:     %s\r\n\r\n"
            "PB13 (BCK / CK - nominale 6.144 MHz):\r\n"
            "  Transizioni:       %lu\r\n"
            "  Livelli visti:     HIGH=%s, LOW=%s (init=%u, final=%u)\r\n"
            "  Stato segnale:     %s\r\n"
            "  Nota PB13:         Campionamento SW a ~%lu.%01lu MSps (sotto Nyquist per 6.144 MHz).\r\n"
            "                     Aliasing atteso; transizioni > 0 indicano attivita' di toggle;\r\n"
            "                     non dichiarare assenza clock senza verifica strumentale diretta.\r\n\r\n",
            (unsigned long)s_metrics.pin_test.duration_us,
            (unsigned long)s_metrics.pin_test.sample_count,
            (unsigned long)s_metrics.pin_test.sample_rate_ksps,
            (unsigned long)msps_int, (unsigned long)msps_dec,
            (unsigned long)s_metrics.pin_test.pb12_transitions,
            (unsigned long)exp_pb12_trans,
            s_metrics.pin_test.pb12_saw_high ? "SI" : "NO",
            s_metrics.pin_test.pb12_saw_low ? "SI" : "NO",
            (unsigned int)s_metrics.pin_test.pb12_init_level,
            (unsigned int)s_metrics.pin_test.pb12_final_level,
            pb12_status,
            (unsigned long)s_metrics.pin_test.pb13_transitions,
            s_metrics.pin_test.pb13_saw_high ? "SI" : "NO",
            s_metrics.pin_test.pb13_saw_low ? "SI" : "NO",
            (unsigned int)s_metrics.pin_test.pb13_init_level,
            (unsigned int)s_metrics.pin_test.pb13_final_level,
            pb13_status,
            (unsigned long)msps_int, (unsigned long)msps_dec);
    }

    case 5:
        return snprintf(buf, max_len,
            "--- PARTE 5/6: SNAPSHOT REGISTRI (INIT E MID) ---\r\n"
            "[INIT (t=%lu ms)]\r\n"
            "  SPI2:   I2SCFGR=0x%04lX (I2SE=%lu, I2SMOD=%lu, CFG=%lu, STD=%lu, DATLEN=%lu, CHLEN=%lu)\r\n"
            "          CR2=0x%04lX (RXDMAEN=%lu, ERRIE=%lu)\r\n"
            "          SR=0x%04lX (RXNE=%lu, OVR=%lu, BSY=%lu, CHSIDE=%lu, FRLVL=%lu)\r\n"
            "  DMA1:   CCR=0x%04lX (EN=%lu, CIRC=%lu, HTIE=%lu, TCIE=%lu, TEIE=%lu, PSIZE=%lu, MSIZE=%lu)\r\n"
            "          CNDTR=%lu, CPAR=0x%08lX, CMAR=0x%08lX\r\n"
            "          ISR=0x%08lX (GIF=%lu, TCIF=%lu, HTIF=%lu, TEIF=%lu)\r\n"
            "  DMAMUX: CCR=0x%08lX (REQ_ID=%lu)\r\n"
            "  GPIOB:  IDR=0x%04lX (PB12=%lu, PB13=%lu, PB15=%lu)\r\n"
            "[MID (t=%lu ms)]\r\n"
            "  SPI2:   I2SCFGR=0x%04lX, CR2=0x%04lX, SR=0x%04lX (RXNE=%lu, OVR=%lu, BSY=%lu, FRLVL=%lu)\r\n"
            "  DMA1:   CCR=0x%04lX, CNDTR=%lu, CPAR=0x%08lX, CMAR=0x%08lX\r\n"
            "          ISR=0x%08lX (GIF=%lu, TCIF=%lu, HTIF=%lu, TEIF=%lu)\r\n"
            "  DMAMUX: CCR=0x%08lX\r\n"
            "  GPIOB:  IDR=0x%04lX (PB12=%lu, PB13=%lu, PB15=%lu)\r\n\r\n",
            (unsigned long)s_metrics.snap_init.tick_ms,
            (unsigned long)s_metrics.snap_init.spi2_i2scfgr,
            (unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 10) & 1U), /* I2SE */
            (unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 11) & 1U), /* I2SMOD */
            (unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 8) & 3U),
            (unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 4) & 3U),
            (unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 1) & 3U),
            (unsigned long)(s_metrics.snap_init.spi2_i2scfgr & 1U),
            (unsigned long)s_metrics.snap_init.spi2_cr2,
            (unsigned long)(s_metrics.snap_init.spi2_cr2 & 1U),
            (unsigned long)((s_metrics.snap_init.spi2_cr2 >> 5) & 1U),
            (unsigned long)s_metrics.snap_init.spi2_sr,
            (unsigned long)(s_metrics.snap_init.spi2_sr & 1U),
            (unsigned long)((s_metrics.snap_init.spi2_sr >> 6) & 1U),
            (unsigned long)((s_metrics.snap_init.spi2_sr >> 7) & 1U),
            (unsigned long)((s_metrics.snap_init.spi2_sr >> 2) & 1U),
            (unsigned long)((s_metrics.snap_init.spi2_sr >> 9) & 3U),
            (unsigned long)s_metrics.snap_init.dma1_ccr,
            (unsigned long)(s_metrics.snap_init.dma1_ccr & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_ccr >> 5) & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_ccr >> 2) & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_ccr >> 1) & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_ccr >> 3) & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_ccr >> 8) & 3U),
            (unsigned long)((s_metrics.snap_init.dma1_ccr >> 10) & 3U),
            (unsigned long)s_metrics.snap_init.dma1_cndtr,
            (unsigned long)s_metrics.snap_init.dma1_cpar,
            (unsigned long)s_metrics.snap_init.dma1_cmar,
            (unsigned long)s_metrics.snap_init.dma1_isr,
            (unsigned long)(s_metrics.snap_init.dma1_isr & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_isr >> 1) & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_isr >> 2) & 1U),
            (unsigned long)((s_metrics.snap_init.dma1_isr >> 3) & 1U),
            (unsigned long)s_metrics.snap_init.dmamux1_ccr,
            (unsigned long)(s_metrics.snap_init.dmamux1_ccr & DMAMUX_CxCR_DMAREQ_ID),
            (unsigned long)(s_metrics.snap_init.gpiob_idr & 0xFFFFU),
            (unsigned long)((s_metrics.snap_init.gpiob_idr >> 12) & 1U),
            (unsigned long)((s_metrics.snap_init.gpiob_idr >> 13) & 1U),
            (unsigned long)((s_metrics.snap_init.gpiob_idr >> 15) & 1U),
            /* MID */
            (unsigned long)s_metrics.snap_mid.tick_ms,
            (unsigned long)s_metrics.snap_mid.spi2_i2scfgr,
            (unsigned long)s_metrics.snap_mid.spi2_cr2,
            (unsigned long)s_metrics.snap_mid.spi2_sr,
            (unsigned long)(s_metrics.snap_mid.spi2_sr & 1U),
            (unsigned long)((s_metrics.snap_mid.spi2_sr >> 6) & 1U),
            (unsigned long)((s_metrics.snap_mid.spi2_sr >> 7) & 1U),
            (unsigned long)((s_metrics.snap_mid.spi2_sr >> 9) & 3U),
            (unsigned long)s_metrics.snap_mid.dma1_ccr,
            (unsigned long)s_metrics.snap_mid.dma1_cndtr,
            (unsigned long)s_metrics.snap_mid.dma1_cpar,
            (unsigned long)s_metrics.snap_mid.dma1_cmar,
            (unsigned long)s_metrics.snap_mid.dma1_isr,
            (unsigned long)(s_metrics.snap_mid.dma1_isr & 1U),
            (unsigned long)((s_metrics.snap_mid.dma1_isr >> 1) & 1U),
            (unsigned long)((s_metrics.snap_mid.dma1_isr >> 2) & 1U),
            (unsigned long)((s_metrics.snap_mid.dma1_isr >> 3) & 1U),
            (unsigned long)s_metrics.snap_mid.dmamux1_ccr,
            (unsigned long)(s_metrics.snap_mid.gpiob_idr & 0xFFFFU),
            (unsigned long)((s_metrics.snap_mid.gpiob_idr >> 12) & 1U),
            (unsigned long)((s_metrics.snap_mid.gpiob_idr >> 13) & 1U),
            (unsigned long)((s_metrics.snap_mid.gpiob_idr >> 15) & 1U));

    case 6:
        return snprintf(buf, max_len,
            "--- PARTE 6/6: SNAPSHOT REGISTRI (STOP) ED ESITO ---\r\n"
            "[PRE-STOP (t=%lu ms)]\r\n"
            "  SPI2:   I2SCFGR=0x%04lX, CR2=0x%04lX, SR=0x%04lX (RXNE=%lu, OVR=%lu, BSY=%lu, FRLVL=%lu)\r\n"
            "  DMA1:   CCR=0x%04lX, CNDTR=%lu, CPAR=0x%08lX, CMAR=0x%08lX\r\n"
            "          ISR=0x%08lX (GIF=%lu, TCIF=%lu, HTIF=%lu, TEIF=%lu)\r\n"
            "  DMAMUX: CCR=0x%08lX\r\n"
            "  GPIOB:  IDR=0x%04lX (PB12=%lu, PB13=%lu, PB15=%lu)\r\n"
            "[POST-STOP (t=%lu ms)]\r\n"
            "  SPI2:   I2SCFGR=0x%04lX, CR2=0x%04lX, SR=0x%04lX\r\n"
            "  DMA1:   CCR=0x%04lX (EN=%lu), CNDTR=%lu, CPAR=0x%08lX, CMAR=0x%08lX\r\n"
            "          ISR=0x%08lX\r\n"
            "  GPIOB:  IDR=0x%04lX (PB12=%lu, PB13=%lu, PB15=%lu)\r\n"
            "  Stop:   Status=%d, ErrorCode=0x%02lX, StopOK=%s\r\n"
            "==================================================\r\n\r\n",
            (unsigned long)s_metrics.snap_pre_stop.tick_ms,
            (unsigned long)s_metrics.snap_pre_stop.spi2_i2scfgr,
            (unsigned long)s_metrics.snap_pre_stop.spi2_cr2,
            (unsigned long)s_metrics.snap_pre_stop.spi2_sr,
            (unsigned long)(s_metrics.snap_pre_stop.spi2_sr & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.spi2_sr >> 6) & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.spi2_sr >> 7) & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.spi2_sr >> 9) & 3U),
            (unsigned long)s_metrics.snap_pre_stop.dma1_ccr,
            (unsigned long)s_metrics.snap_pre_stop.dma1_cndtr,
            (unsigned long)s_metrics.snap_pre_stop.dma1_cpar,
            (unsigned long)s_metrics.snap_pre_stop.dma1_cmar,
            (unsigned long)s_metrics.snap_pre_stop.dma1_isr,
            (unsigned long)(s_metrics.snap_pre_stop.dma1_isr & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.dma1_isr >> 1) & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.dma1_isr >> 2) & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.dma1_isr >> 3) & 1U),
            (unsigned long)s_metrics.snap_pre_stop.dmamux1_ccr,
            (unsigned long)(s_metrics.snap_pre_stop.gpiob_idr & 0xFFFFU),
            (unsigned long)((s_metrics.snap_pre_stop.gpiob_idr >> 12) & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.gpiob_idr >> 13) & 1U),
            (unsigned long)((s_metrics.snap_pre_stop.gpiob_idr >> 15) & 1U),
            /* POST-STOP */
            (unsigned long)s_metrics.snap_post_stop.tick_ms,
            (unsigned long)s_metrics.snap_post_stop.spi2_i2scfgr,
            (unsigned long)s_metrics.snap_post_stop.spi2_cr2,
            (unsigned long)s_metrics.snap_post_stop.spi2_sr,
            (unsigned long)s_metrics.snap_post_stop.dma1_ccr,
            (unsigned long)(s_metrics.snap_post_stop.dma1_ccr & 1U),
            (unsigned long)s_metrics.snap_post_stop.dma1_cndtr,
            (unsigned long)s_metrics.snap_post_stop.dma1_cpar,
            (unsigned long)s_metrics.snap_post_stop.dma1_cmar,
            (unsigned long)s_metrics.snap_post_stop.dma1_isr,
            (unsigned long)(s_metrics.snap_post_stop.gpiob_idr & 0xFFFFU),
            (unsigned long)((s_metrics.snap_post_stop.gpiob_idr >> 12) & 1U),
            (unsigned long)((s_metrics.snap_post_stop.gpiob_idr >> 13) & 1U),
            (unsigned long)((s_metrics.snap_post_stop.gpiob_idr >> 15) & 1U),
            (int)s_metrics.stop_status,
            (unsigned long)s_metrics.stop_error_code,
            s_metrics.stop_ok ? "SI" : "NO");

    default:
        buf[0] = '\0';
        return 0;
    }
}

/* =========================================================================
 * Polling & 10-Second Test Execution
 * ========================================================================= */

void i2s_rx_diag_poll(void)
{
    uint32_t now = HAL_GetTick();

    if (s_test_state == TEST_STATE_RUNNING) {
        uint32_t elapsed = now - s_start_tick;

        /* Minimal-load sampling of CNDTR */
        uint32_t cndtr = DMA1_Channel1->CNDTR;
        if (cndtr < s_metrics.cndtr_min) {
            s_metrics.cndtr_min = cndtr;
        }
        if (cndtr > s_metrics.cndtr_max) {
            s_metrics.cndtr_max = cndtr;
        }
        if (cndtr != s_cndtr_prev) {
            s_metrics.cndtr_changes++;
            s_cndtr_prev = cndtr;
        }

        /* Intermediate snapshot at ~5000 ms */
        if (!s_mid_snapshot_done && elapsed >= 5000U) {
            capture_snapshot(&s_metrics.snap_mid, elapsed);
            s_mid_snapshot_done = 1;
        }

        /* Toggle LED PC6 at ~2 Hz during test */
        if ((uint32_t)(now - s_last_led_tick) >= 250U) {
            GPIOC->ODR ^= (1UL << 6U);
            s_last_led_tick = now;
        }

        /* Check if 10-second test window has elapsed */
        if (elapsed >= 10000U) {
            s_test_state = TEST_STATE_COMPLETED;
            s_metrics.duration_ms = elapsed;

            /* Capture snapshot IMMEDIATELY BEFORE stop */
            capture_snapshot(&s_metrics.snap_pre_stop, elapsed);
            s_metrics.cndtr_pre_stop = s_metrics.snap_pre_stop.dma1_cndtr;

            /* Disable interrupts before stopping */
            __HAL_I2S_DISABLE_IT(&hi2s2, I2S_IT_ERR);
            HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
            HAL_NVIC_DisableIRQ(SPI2_IRQn);

            /* Freeze ISR counts */
            s_metrics.half_cplt_count = s_half_cplt_count;
            s_metrics.full_cplt_count = s_full_cplt_count;
            s_metrics.total_callbacks = s_metrics.half_cplt_count + s_metrics.full_cplt_count;
            s_metrics.error_callback_count = s_error_callback_count;
            s_metrics.ovr_count = s_ovr_count;
            s_metrics.last_hal_error = s_last_hal_error;
            s_metrics.raw_irq_dma_count = s_raw_irq_dma_count;
            s_metrics.raw_irq_spi_count = s_raw_irq_spi_count;
            s_metrics.is_stalled = (s_metrics.total_callbacks == 0U);

            /* Stop DMA and flush DR */
            s_metrics.stop_status = HAL_I2S_DMAStop(&hi2s2);
            __HAL_I2S_FLUSH_RX_DR(&hi2s2);

            /* Capture snapshot POST-STOP */
            capture_snapshot(&s_metrics.snap_post_stop, HAL_GetTick() - s_start_tick);
            s_metrics.cndtr_post_stop = s_metrics.snap_post_stop.dma1_cndtr;

            /* Verify stop status */
            s_metrics.stop_error_code = hi2s2.ErrorCode;
            s_metrics.dma_channel_enabled = ((DMA1_Channel1->CCR & DMA_CCR_EN) != 0U);
            s_metrics.rxdmaen_enabled = ((SPI2->CR2 & SPI_CR2_RXDMAEN) != 0U);

            if (s_metrics.dma_channel_enabled || s_metrics.rxdmaen_enabled) {
                s_metrics.stop_ok = 0;
            } else if (s_metrics.stop_status == HAL_OK && s_metrics.stop_error_code == HAL_I2S_ERROR_NONE) {
                s_metrics.stop_ok = 1;
            } else if (s_metrics.stop_status == HAL_ERROR && s_metrics.stop_error_code == HAL_I2S_ERROR_BUSY_LINE_RX) {
                s_metrics.stop_ok = 1;
            } else {
                s_metrics.stop_ok = 0;
            }

            /* Non-invasive GPIO IDR pin verification (PB12 & PB13 in AF mode) */
            run_pin_input_test(&s_metrics.pin_test);

            /* Diagnostic classification */
            int cfg_ok = 1;
            const char *cfg_err = "OK";
            if ((s_metrics.snap_init.rcc_apb1enr1 & RCC_APB1ENR1_SPI2EN) == 0U) {
                cfg_ok = 0; cfg_err = "Clock SPI2 non abilitato in APB1ENR1";
            } else if ((s_metrics.snap_init.rcc_ahb1enr & RCC_AHB1ENR_DMA1EN) == 0U) {
                cfg_ok = 0; cfg_err = "Clock DMA1 non abilitato in AHB1ENR";
            } else if ((s_metrics.snap_init.rcc_ahb1enr & RCC_AHB1ENR_DMAMUX1EN) == 0U) {
                cfg_ok = 0; cfg_err = "Clock DMAMUX1 non abilitato in AHB1ENR";
            } else if ((s_metrics.snap_init.rcc_ahb2enr & RCC_AHB2ENR_GPIOBEN) == 0U) {
                cfg_ok = 0; cfg_err = "Clock GPIOB non abilitato in AHB2ENR";
            } else if (((s_metrics.snap_init.gpiob_moder >> 24) & 3U) != 2U ||
                       ((s_metrics.snap_init.gpiob_moder >> 26) & 3U) != 2U ||
                       ((s_metrics.snap_init.gpiob_moder >> 30) & 3U) != 2U) {
                cfg_ok = 0; cfg_err = "PB12/13/15 non configurati in MODER AF";
            } else if (((s_metrics.snap_init.gpiob_afrh >> 16) & 0xFU) != 5U ||
                       ((s_metrics.snap_init.gpiob_afrh >> 20) & 0xFU) != 5U ||
                       ((s_metrics.snap_init.gpiob_afrh >> 28) & 0xFU) != 5U) {
                cfg_ok = 0; cfg_err = "PB12/13/15 non configurati in AFR5 (SPI2)";
            } else if ((s_metrics.snap_init.spi2_i2scfgr & SPI_I2SCFGR_I2SE) == 0U) {
                cfg_ok = 0; cfg_err = "SPI2 I2SE non abilitato";
            } else if ((s_metrics.snap_init.spi2_cr2 & SPI_CR2_RXDMAEN) == 0U) {
                cfg_ok = 0; cfg_err = "SPI2 RXDMAEN non abilitato";
            } else if ((s_metrics.snap_init.dmamux1_ccr & DMAMUX_CxCR_DMAREQ_ID) != 12U) {
                cfg_ok = 0; cfg_err = "DMAMUX1 Ch0 request ID non e' 12 (SPI2_RX)";
            } else if ((s_metrics.snap_init.dma1_ccr & DMA_CCR_EN) == 0U) {
                cfg_ok = 0; cfg_err = "DMA1 Channel 1 non abilitato (CCR.EN=0)";
            } else if (s_metrics.snap_init.nvic_dma1_ch1_enabled == 0U) {
                cfg_ok = 0; cfg_err = "NVIC DMA1 Channel 1 non abilitato";
            } else if (!s_metrics.cpar_match) {
                cfg_ok = 0; cfg_err = "DMA1 CPAR non conforme a SPI2_DR (0x4000380C)";
            } else if (!s_metrics.cmar_match) {
                cfg_ok = 0; cfg_err = "DMA1 CMAR non conforme a i2s_dma_buffer";
            }

            if (!cfg_ok) {
                s_classification = "CONFIGURAZIONE REGISTRI INCONGRUENTE";
                s_analysis_detail = cfg_err;
            } else if (s_metrics.cndtr_changes == 0U && s_metrics.cndtr_min == s_metrics.cndtr_initial) {
                s_classification = "DMA SENZA RICHIESTE/TRASFERIMENTI OSSERVATI";
                uint32_t sr = s_metrics.snap_pre_stop.spi2_sr;
                if ((sr & (SPI_SR_RXNE | SPI_SR_OVR | SPI_SR_BSY)) == 0U && ((sr >> 9) & 3U) == 0U) {
                    if (s_metrics.pin_test.pb12_transitions > 0U && s_metrics.pin_test.pb13_transitions > 0U) {
                        s_analysis_detail = "Nessuna attivita' dati in SPI2_SR (RXNE=0, BSY=0, OVR=0, FRLVL=0). Nota F5-D03: transizioni rilevate su PB12 e PB13 durante il test pin.";
                    } else if (s_metrics.pin_test.pb12_transitions == 0U || s_metrics.pin_test.pb13_transitions == 0U) {
                        s_analysis_detail = "Nessuna attivita' dati in SPI2_SR. Nota F5-D03: assenza di transizioni rilevata su PB12 e/o PB13 durante il test pin.";
                    } else {
                        s_analysis_detail = "Nessuna attivita' dati osservata negli snapshot di SPI2_SR (RXNE=0, BSY=0, OVR=0, FRLVL=0). Questo dato fotografa lo stato della periferica all'istante di campionamento e non costituisce prova diretta dell'assenza fisica dei clock esterni BCK/LRCK.";
                    }
                } else {
                    s_analysis_detail = "Attivita' dati/clock osservata in SPI2_SR (RXNE/OVR/FRLVL asseriti), ma CNDTR non e' variato. Le richieste non sono state servite dal DMA (verificare DMAMUX1 o controller DMA1).";
                }
            } else if (s_metrics.cndtr_changes > 0U && s_metrics.total_callbacks == 0U) {
                s_classification = "DMA CHE TRASFERISCE DATI SENZA CALLBACK OSSERVATE";
                s_analysis_detail = "CNDTR e' decrementato/variato (trasferimenti DMA da SPI2_DR in memoria osservati), ma nessuna callback Half/Full e' stata invocata durante la prova. Verificare IRQ handler, HTIE/TCIE e maschere NVIC.";
            } else if (s_metrics.cndtr_changes > 0U && s_metrics.total_callbacks > 0U) {
                s_classification = "DMA E CALLBACK ENTRAMBI OSSERVATI";
                s_analysis_detail = "Osservati trasferimenti DMA (CNDTR variato) e invocazione di callback Half/Full durante la prova. Non attesta automaticamente continuita' nominale per 10 secondi o assenza di errori audio.";
            } else {
                s_classification = "STATO NON DETERMINATO";
                s_analysis_detail = "Valori di CNDTR o registri non univoci.";
            }

            /* Initialize multi-part CDC transmission sequence */
            s_tx_stage = 1;
            s_tx_stage_start_tick = now;
            s_last_part_tick = 0;
            s_last_report_tick = now;
        }
    } else if (s_test_state == TEST_STATE_COMPLETED) {
        /* LED solid ON if stalled or errors, OFF if OK */
        if (s_metrics.is_stalled || s_metrics.ovr_count > 0 || s_metrics.error_callback_count > 0) {
            GPIOC->BSRR = (1UL << 6U);
        } else {
            GPIOC->BRR = (1UL << 6U);
        }

        /* Multi-part CDC transmission state machine (6 parts) */
        if (s_tx_stage == 0) {
            /* Periodically retransmit full report every 5000 ms */
            if ((uint32_t)(now - s_last_report_tick) >= 5000U) {
                s_tx_stage = 1;
                s_tx_stage_start_tick = now;
                s_last_part_tick = 0;
            }
        }

        if (s_tx_stage >= 1 && s_tx_stage <= 6) {
            /* Small inter-segment pacing (>= 10 ms since last accepted part) */
            if ((uint32_t)(now - s_last_part_tick) >= 10U) {
                format_report_part(s_tx_stage, s_seg_buffer, sizeof(s_seg_buffer));
                if (diagnostic_cdc_send(s_seg_buffer)) {
                    s_last_part_tick = now;
                    s_tx_stage++;
                    s_tx_stage_start_tick = now;
                    if (s_tx_stage > 6) {
                        /* Entire 6-part report transmitted successfully */
                        s_tx_stage = 0;
                        s_last_report_tick = now;
                    }
                } else {
                    /* Not accepted by CDC (host not ready or previous packet still on bus).
                     * If stage > 1 gets stuck for > 2000 ms (e.g. host disconnected mid-transmission),
                     * abort sequence cleanly to avoid blocking next periodic cycles. */
                    if (s_tx_stage > 1 && (uint32_t)(now - s_tx_stage_start_tick) >= 2000U) {
                        s_tx_stage = 0;
                        s_last_report_tick = now;
                    }
                }
            }
        }
    }
}
