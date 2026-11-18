#ifndef TICKFORM_I2S_RX_DIAG_H
#define TICKFORM_I2S_RX_DIAG_H

#include "stm32g4xx_hal.h"
#include <stdint.h>

/* Number of 32-bit audio slots in circular buffer.
 * At 96 kHz stereo, 2 slots = 1 stereo frame (Left + Right).
 * 1024 slots = 512 frames = 5.333 ms of audio.
 * Total memory: 1024 * 2 halfwords = 2048 uint16_t = 4096 bytes (4 KB).
 */
#define I2S_RX_BUFFER_SLOTS       1024U
#define I2S_RX_BUFFER_HALFWORDS   (I2S_RX_BUFFER_SLOTS * 2U)

/* Hardware register snapshot for in-depth diagnostic analysis */
typedef struct {
    uint32_t tick_ms;
    /* SPI2 / I2S2 registers */
    uint32_t spi2_cr1;
    uint32_t spi2_cr2;
    uint32_t spi2_sr;
    uint32_t spi2_i2scfgr;
    uint32_t spi2_i2spr;
    /* DMA1 Channel 1 registers */
    uint32_t dma1_isr;
    uint32_t dma1_ccr;
    uint32_t dma1_cndtr;
    uint32_t dma1_cpar;
    uint32_t dma1_cmar;
    /* DMAMUX1 Channel 0 register */
    uint32_t dmamux1_ccr;
    /* GPIO Port B registers */
    uint32_t gpiob_moder;
    uint32_t gpiob_afrh;
    uint32_t gpiob_idr;
    /* Clocks (RCC) */
    uint32_t rcc_ahb1enr;
    uint32_t rcc_ahb2enr;
    uint32_t rcc_apb1enr1;
    /* NVIC enable status */
    uint32_t nvic_dma1_ch1_enabled;
    uint32_t nvic_spi2_enabled;
    /* Raw IRQ handler entry counts */
    uint32_t raw_irq_dma_count;
    uint32_t raw_irq_spi_count;
} i2s_diag_snapshot_t;

/* Pin input verification structure for F5-D03 */
typedef struct {
    uint32_t sample_count;       /* Total iterations of IDR read loop */
    uint32_t duration_us;        /* Measured duration in microseconds (via DWT) */
    uint32_t sample_rate_ksps;   /* Estimated sampling rate in kSps (samples * 1000 / us) */
    uint32_t pb12_transitions;   /* Number of logic transitions detected on PB12 (LRCK) */
    uint32_t pb13_transitions;   /* Number of logic transitions detected on PB13 (BCK) */
    uint8_t pb12_init_level;     /* Initial logic level of PB12 (0 or 1) */
    uint8_t pb12_final_level;    /* Final logic level of PB12 (0 or 1) */
    uint8_t pb12_saw_high;       /* 1 if logic level 1 was observed at least once */
    uint8_t pb12_saw_low;        /* 1 if logic level 0 was observed at least once */
    uint8_t pb13_init_level;     /* Initial logic level of PB13 (0 or 1) */
    uint8_t pb13_final_level;    /* Final logic level of PB13 (0 or 1) */
    uint8_t pb13_saw_high;       /* 1 if logic level 1 was observed at least once */
    uint8_t pb13_saw_low;        /* 1 if logic level 0 was observed at least once */
} i2s_diag_pin_test_t;

/* Diagnostic metrics structure for F5-D03 */
typedef struct {
    uint32_t duration_ms;
    /* Callback counts */
    uint32_t half_cplt_count;
    uint32_t full_cplt_count;
    uint32_t total_callbacks;
    uint32_t error_callback_count;
    uint32_t ovr_count;
    uint32_t last_hal_error;
    /* Raw ISR entry counts */
    uint32_t raw_irq_dma_count;
    uint32_t raw_irq_spi_count;
    /* CNDTR tracking */
    uint32_t cndtr_initial;
    uint32_t cndtr_min;
    uint32_t cndtr_max;
    uint32_t cndtr_pre_stop;
    uint32_t cndtr_post_stop;
    uint32_t cndtr_changes;
    /* DMA Address verification (F5-D03) */
    uint32_t expected_cpar;
    uint32_t expected_cmar;
    int cpar_match;
    int cmar_match;
    /* Digital pin verification (F5-D03) */
    i2s_diag_pin_test_t pin_test;
    /* Timed snapshots */
    i2s_diag_snapshot_t snap_init;
    i2s_diag_snapshot_t snap_mid;
    i2s_diag_snapshot_t snap_pre_stop;
    i2s_diag_snapshot_t snap_post_stop;
    /* Stop verification */
    HAL_StatusTypeDef stop_status;
    uint32_t stop_error_code;
    uint8_t dma_channel_enabled;
    uint8_t rxdmaen_enabled;
    int stop_ok;
    int is_stalled;
} i2s_diag_metrics_t;

void i2s_rx_diag_init(void);
void i2s_rx_diag_poll(void);

#endif /* TICKFORM_I2S_RX_DIAG_H */
