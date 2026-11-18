/*
 * Tickform FASE 5 — I2S2 Slave Receiver + DMA Circular Test
 * Target: WeAct Studio STM32G431 Core Board V1.0 (STM32G431CBU6)
 * Master: PCM1808 C (96 kHz nominal, 64 BCK per frame, Philips I2S)
 * Wiring:
 *   PCM1808 LRCK (J2.4) -> PB12 (I2S2_WS, AF5)
 *   PCM1808 BCK  (J2.6) -> PB13 (I2S2_CK, AF5)
 *   PCM1808 DOUT (J2.5) -> PB15 (I2S2_SD, AF5)
 */

#include "i2s_rx.h"
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int diagnostic_cdc_send(const char *text);

/* Peripheral handles */
static I2S_HandleTypeDef hi2s2;
static DMA_HandleTypeDef hdma_i2s2_rx;

/* Static circular DMA buffer in standard SRAM (RAM)
 * Aligned to 4 bytes for clean access.
 * Holds 2048 halfwords (1024 32-bit slots = 512 stereo frames = 4096 bytes).
 */
static uint16_t i2s_dma_buffer[I2S_RX_BUFFER_HALFWORDS] __attribute__((aligned(4)));

/* ISR counters and snapshot (volatile) */
static volatile uint32_t s_half_cplt_count = 0;
static volatile uint32_t s_full_cplt_count = 0;
static volatile uint32_t s_error_callback_count = 0;
static volatile uint32_t s_ovr_count = 0;
static volatile uint32_t s_last_hal_error = 0;
static volatile uint16_t s_snapshot_hw[16];
static volatile int s_snapshot_ready = 0;

/* Test states */
enum {
    TEST_STATE_IDLE = 0,
    TEST_STATE_RUNNING,
    TEST_STATE_COMPLETED
};

static int s_test_state = TEST_STATE_IDLE;
static int s_test_passed = 0;
static uint32_t s_start_tick = 0;
static uint32_t s_last_report_tick = 0;
static uint32_t s_last_led_tick = 0;
static char s_report[1500];
static i2s_rx_metrics_t s_metrics;

/* =========================================================================
 * Interrupt Handlers
 * ========================================================================= */

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_i2s2_rx);
}

void SPI2_IRQHandler(void)
{
    HAL_I2S_IRQHandler(&hi2s2);
}

/* =========================================================================
 * HAL I2S Callbacks
 * ========================================================================= */

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2) {
        s_half_cplt_count++;
        /* Freeze the FIRST valid snapshot of 4 consecutive stereo frames
         * (8 slots = 16 halfwords) while DMA is operating on the second half of the buffer.
         * This avoids continuous overwrites during test execution and report formatting.
         */
        if (!s_snapshot_ready) {
            for (uint32_t i = 0; i < 16U; i++) {
                s_snapshot_hw[i] = i2s_dma_buffer[i];
            }
            s_snapshot_ready = 1;
        }
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
 * Initialization
 * ========================================================================= */

void i2s_rx_init(void)
{
    /* 1. Reset counters and state */
    s_half_cplt_count = 0;
    s_full_cplt_count = 0;
    s_error_callback_count = 0;
    s_ovr_count = 0;
    s_last_hal_error = 0;
    s_snapshot_ready = 0;
    s_test_passed = 0;
    memset((void *)s_snapshot_hw, 0, sizeof(s_snapshot_hw));
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
        GPIOC->BSRR = (1UL << 6U); /* LED ON on error */
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
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_DEFAULT; /* Slave mode bypasses prescaler */
    hi2s2.Init.CPOL = I2S_CPOL_LOW;

    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
        GPIOC->BSRR = (1UL << 6U); /* LED ON on error */
        return;
    }

    /* 7. Start circular DMA reception:
     *    In HAL_I2S_Receive_DMA with I2S_DATAFORMAT_24B, Size parameter
     *    is the number of 32-bit slots. HAL doubles Size to compute
     *    RxXferSize = (Size << 1) = 2048 halfwords, matching i2s_dma_buffer.
     */
    if (HAL_I2S_Receive_DMA(&hi2s2, (uint16_t *)i2s_dma_buffer, I2S_RX_BUFFER_SLOTS) != HAL_OK) {
        GPIOC->BSRR = (1UL << 6U); /* LED ON on error */
        return;
    }

    /* 8. Enable SPI error interrupt (OVR) */
    __HAL_I2S_ENABLE_IT(&hi2s2, I2S_IT_ERR);

    /* 9. Mark test as running */
    s_start_tick = HAL_GetTick();
    s_last_report_tick = s_start_tick;
    s_last_led_tick = s_start_tick;
    s_test_state = TEST_STATE_RUNNING;
}

/* =========================================================================
 * Polling & 10-Second Test Execution
 * ========================================================================= */

void i2s_rx_poll(void)
{
    uint32_t now = HAL_GetTick();

    if (s_test_state == TEST_STATE_RUNNING) {
        /* Check for pending errors directly in SPI2 status register */
        if (__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_OVR)) {
            s_ovr_count++;
            __HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
        }

        /* Toggle LED PC6 at ~2 Hz during test execution */
        if ((uint32_t)(now - s_last_led_tick) >= 250U) {
            GPIOC->ODR ^= (1UL << 6U);
            s_last_led_tick = now;
        }

        /* Check if 10-second test window has elapsed */
        if ((uint32_t)(now - s_start_tick) >= 10000U) {
            /* 1. Freeze state and duration */
            s_test_state = TEST_STATE_COMPLETED;
            s_metrics.duration_ms = now - s_start_tick;

            /* 2. Disable interrupts first to prevent callbacks during/after stop */
            __HAL_I2S_DISABLE_IT(&hi2s2, I2S_IT_ERR);
            HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
            HAL_NVIC_DisableIRQ(SPI2_IRQn);

            /* 3. Freeze ISR metrics */
            s_metrics.half_cplt_count = s_half_cplt_count;
            s_metrics.full_cplt_count = s_full_cplt_count;
            s_metrics.total_callbacks = s_metrics.half_cplt_count + s_metrics.full_cplt_count;
            s_metrics.total_slots_received = s_metrics.total_callbacks * (I2S_RX_BUFFER_SLOTS / 2U);
            s_metrics.total_bytes_received = s_metrics.total_slots_received * 4U;
            s_metrics.error_callback_count = s_error_callback_count;
            s_metrics.ovr_count = s_ovr_count;
            s_metrics.last_hal_error = s_last_hal_error;
            s_metrics.is_stalled = (s_metrics.total_callbacks == 0U);

            /* 4. Stop circular DMA and flush DR.
             * Note: In Slave RX mode, HAL_I2S_DMAStop returns HAL_ERROR and sets
             * HAL_I2S_ERROR_BUSY_LINE_RX because the external master clock continues running.
             * This is normal per ST HAL design and not an error.
             */
            s_metrics.stop_status = HAL_I2S_DMAStop(&hi2s2);
            __HAL_I2S_FLUSH_RX_DR(&hi2s2);

            /* Verification of DMA stop:
             * - Hardware check: DMA1 Channel 1 must be disabled (CCR_EN == 0)
             *   and SPI2 RX DMA request must be disabled (RXDMAEN == 0).
             * - Return & ErrorCode check:
             *   Must be either HAL_OK (with HAL_I2S_ERROR_NONE), or HAL_ERROR accompanied
             *   EXCLUSIVELY by HAL_I2S_ERROR_BUSY_LINE_RX.
             *   Any other error (e.g. HAL_I2S_ERROR_DMA, HAL_I2S_ERROR_TIMEOUT) or failure
             *   to disable the DMA channel / request must generate FAIL.
             */
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

            /* 5. Calculate expected callbacks based on measured duration at 96 kHz:
             * Buffer: 1024 slots = 512 stereo frames. Half-buffer = 256 frames.
             * 96000 frames/s / 256 frames/callback = 375 callbacks/s (total half + full).
             */
            s_metrics.expected_callbacks = (uint32_t)(((uint64_t)s_metrics.duration_ms * 375U + 500U) / 1000U);
            uint32_t min_callbacks = (s_metrics.expected_callbacks * 98U) / 100U;
            uint32_t max_callbacks = (s_metrics.expected_callbacks * 102U + 99U) / 100U;

            /* Check Half vs Full balance: diff must be 0 (equal) or 1 (Half fired, test stopped before Full) */
            int32_t diff = (int32_t)s_metrics.half_cplt_count - (int32_t)s_metrics.full_cplt_count;
            s_metrics.balance_ok = (diff == 0 || diff == 1);

            /* 6. Process sample snapshot */
            if (s_snapshot_ready) {
                for (uint32_t slot = 0; slot < 8U; slot++) {
                    uint16_t ms_hw = s_snapshot_hw[slot * 2U + 0U];
                    uint16_t ls_hw = s_snapshot_hw[slot * 2U + 1U];
                    s_metrics.sample_raw_hw[slot * 2U + 0U] = ms_hw;
                    s_metrics.sample_raw_hw[slot * 2U + 1U] = ls_hw;
                    uint32_t raw32 = ((uint32_t)ms_hw << 16U) | (uint32_t)ls_hw;
                    /* Arithmetic right shift sign-extends 24-bit audio to signed 32-bit int */
                    s_metrics.sample_pcm24[slot] = (int32_t)raw32 >> 8;
                }
            }

            /* 7. Calculate estimated throughput in B/s */
            uint32_t throughput_bps = 0;
            if (s_metrics.duration_ms > 0) {
                throughput_bps = (uint32_t)(((uint64_t)s_metrics.total_bytes_received * 1000U) / s_metrics.duration_ms);
            }

            /* 8. Determine overall test result */
            const char *status_str;
            if (s_metrics.is_stalled) {
                status_str = "FAIL: NO CLOCK / DMA STALLED (0 callbacks received)";
                s_test_passed = 0;
                GPIOC->BSRR = (1UL << 6U); /* Steady ON on failure */
            } else if (!s_metrics.stop_ok) {
                status_str = "FAIL: DMA STOP FAILED (channel or request not disabled, or unexpected HAL error)";
                s_test_passed = 0;
                GPIOC->BSRR = (1UL << 6U); /* Steady ON on failure */
            } else if (s_metrics.ovr_count > 0 || s_metrics.error_callback_count > 0) {
                status_str = "FAIL: I2S/DMA ERRORS DETECTED (Overrun or DMA error)";
                s_test_passed = 0;
                GPIOC->BSRR = (1UL << 6U); /* Steady ON on failure */
            } else if (!s_metrics.balance_ok) {
                status_str = "FAIL: UNBALANCED CALLBACKS (Half vs Full mismatch)";
                s_test_passed = 0;
                GPIOC->BSRR = (1UL << 6U); /* Steady ON on failure */
            } else if (s_metrics.total_callbacks < min_callbacks) {
                status_str = "FAIL: DATA UNDERFLOW / CLOCK SLOW (callbacks < -2% expected)";
                s_test_passed = 0;
                GPIOC->BSRR = (1UL << 6U); /* Steady ON on failure */
            } else if (s_metrics.total_callbacks > max_callbacks) {
                status_str = "FAIL: DATA OVERFLOW / CLOCK FAST (callbacks > +2% expected)";
                s_test_passed = 0;
                GPIOC->BSRR = (1UL << 6U); /* Steady ON on failure */
            } else {
                status_str = "PASS: DMA RX ~96 kHz FUNCTIONAL (rate within +/-2%, audio conversion NOT YET validated)";
                s_test_passed = 1;
                GPIOC->BRR = (1UL << 6U);  /* LED OFF on clean success */
            }

            /* 9. Format compact diagnostic report */
            char dma_stop_str[96];
            if (s_metrics.stop_ok) {
                if (s_metrics.stop_status == HAL_OK) {
                    snprintf(dma_stop_str, sizeof(dma_stop_str), "OK (clean stop, DMA1_Ch1=OFF, RXDMAEN=OFF)");
                } else {
                    snprintf(dma_stop_str, sizeof(dma_stop_str), "OK (BUSY_LINE_RX expected, DMA1_Ch1=OFF, RXDMAEN=OFF)");
                }
            } else {
                snprintf(dma_stop_str, sizeof(dma_stop_str), "FAIL (status=%d, err=0x%02lX, DMA_EN=%u, RXDMAEN=%u)",
                    (int)s_metrics.stop_status, (unsigned long)s_metrics.stop_error_code,
                    (unsigned int)s_metrics.dma_channel_enabled, (unsigned int)s_metrics.rxdmaen_enabled);
            }

            int len = snprintf(s_report, sizeof(s_report),
                "========================================\r\n"
                "TICKFORM FASE 5 - I2S2 RX + DMA TEST\r\n"
                "========================================\r\n"
                "Duration:     %lu ms (nominal 10000 ms)\r\n"
                "Master:       PCM1808 C (96 kHz nominal)\r\n"
                "STM32:        I2S2 Slave RX, Philips 24b/32b frame\r\n"
                "Pins:         PB12=WS (LRCK), PB13=CK (BCK), PB15=SD (DOUT)\r\n"
                "DMA:          DMA1 Ch1 circular, buffer 1024 slots (4096 B)\r\n"
                "DMA Stop:     %s\r\n\r\n"
                "--- CALLS & THROUGHPUT ---\r\n"
                "Half Cplt:    %lu (rate ~187.5/s)\r\n"
                "Full Cplt:    %lu (rate ~187.5/s, balance diff=%ld)\r\n"
                "Total DMA CB: %lu (expected: %lu, tol +/-2%%: %lu..%lu)\r\n"
                "Slots recv:   %lu\r\n"
                "Bytes recv:   %lu\r\n"
                "Throughput:   %lu B/s (nominal 768000 B/s)\r\n\r\n"
                "--- ERROR MONITORING ---\r\n"
                "Error CB:     %lu\r\n"
                "Overrun(OVR): %lu\r\n"
                "Frame Error (FRE): NON MONITORATO — sincronismo frame non verificato\r\n"
                "HAL Error:    0x%08lX\r\n"
                "DMA Stalled:  %s\r\n\r\n"
                "--- RAW SAMPLES (First 4 frames: MS, LS -> PCM24) ---\r\n"
                "F0: L=(0x%04X,0x%04X -> %ld) R=(0x%04X,0x%04X -> %ld)\r\n"
                "F1: L=(0x%04X,0x%04X -> %ld) R=(0x%04X,0x%04X -> %ld)\r\n"
                "F2: L=(0x%04X,0x%04X -> %ld) R=(0x%04X,0x%04X -> %ld)\r\n"
                "F3: L=(0x%04X,0x%04X -> %ld) R=(0x%04X,0x%04X -> %ld)\r\n\r\n"
                "--- OVERALL STATUS ---\r\n"
                "%s\r\n"
                "========================================\r\n\r\n",
                (unsigned long)s_metrics.duration_ms,
                dma_stop_str,
                (unsigned long)s_metrics.half_cplt_count,
                (unsigned long)s_metrics.full_cplt_count, (long)diff,
                (unsigned long)s_metrics.total_callbacks,
                (unsigned long)s_metrics.expected_callbacks,
                (unsigned long)min_callbacks,
                (unsigned long)max_callbacks,
                (unsigned long)s_metrics.total_slots_received,
                (unsigned long)s_metrics.total_bytes_received,
                (unsigned long)throughput_bps,
                (unsigned long)s_metrics.error_callback_count,
                (unsigned long)s_metrics.ovr_count,
                (unsigned long)s_metrics.last_hal_error,
                s_metrics.is_stalled ? "YES (NO ACTIVITY)" : "NO",
                (unsigned int)s_metrics.sample_raw_hw[0], (unsigned int)s_metrics.sample_raw_hw[1], (long)s_metrics.sample_pcm24[0],
                (unsigned int)s_metrics.sample_raw_hw[2], (unsigned int)s_metrics.sample_raw_hw[3], (long)s_metrics.sample_pcm24[1],
                (unsigned int)s_metrics.sample_raw_hw[4], (unsigned int)s_metrics.sample_raw_hw[5], (long)s_metrics.sample_pcm24[2],
                (unsigned int)s_metrics.sample_raw_hw[6], (unsigned int)s_metrics.sample_raw_hw[7], (long)s_metrics.sample_pcm24[3],
                (unsigned int)s_metrics.sample_raw_hw[8], (unsigned int)s_metrics.sample_raw_hw[9], (long)s_metrics.sample_pcm24[4],
                (unsigned int)s_metrics.sample_raw_hw[10], (unsigned int)s_metrics.sample_raw_hw[11], (long)s_metrics.sample_pcm24[5],
                (unsigned int)s_metrics.sample_raw_hw[12], (unsigned int)s_metrics.sample_raw_hw[13], (long)s_metrics.sample_pcm24[6],
                (unsigned int)s_metrics.sample_raw_hw[14], (unsigned int)s_metrics.sample_raw_hw[15], (long)s_metrics.sample_pcm24[7],
                status_str);
            (void)len;

            /* Send immediate report over USB CDC */
            diagnostic_cdc_send(s_report);
            s_last_report_tick = now;
        }
    } else if (s_test_state == TEST_STATE_COMPLETED) {
        /* Periodically retransmit report every 5000 ms so that a late serial connection receives it */
        if ((uint32_t)(now - s_last_report_tick) >= 5000U) {
            diagnostic_cdc_send(s_report);
            s_last_report_tick = now;
        }

        /* If test failed or stalled, blink LED PC6 rapidly (100 ms) */
        if (!s_test_passed) {
            if ((uint32_t)(now - s_last_led_tick) >= 100U) {
                GPIOC->ODR ^= (1UL << 6U);
                s_last_led_tick = now;
            }
        }
    }
}
