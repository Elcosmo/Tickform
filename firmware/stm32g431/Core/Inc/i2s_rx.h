#ifndef TICKFORM_I2S_RX_H
#define TICKFORM_I2S_RX_H

#include "stm32g4xx_hal.h"
#include <stdint.h>

/* Number of 32-bit audio slots in circular buffer.
 * At 96 kHz stereo, 2 slots = 1 stereo frame (Left + Right).
 * 1024 slots = 512 frames = 5.333 ms of audio.
 * Total memory: 1024 * 2 halfwords = 2048 uint16_t = 4096 bytes (4 KB).
 */
#define I2S_RX_BUFFER_SLOTS       1024U
#define I2S_RX_BUFFER_HALFWORDS   (I2S_RX_BUFFER_SLOTS * 2U)

/* Diagnostic metrics snapshot */
typedef struct {
    uint32_t duration_ms;
    uint32_t half_cplt_count;
    uint32_t full_cplt_count;
    uint32_t total_callbacks;
    uint32_t expected_callbacks;
    uint32_t total_slots_received;
    uint32_t total_bytes_received;
    uint32_t error_callback_count;
    uint32_t ovr_count;
    uint32_t last_hal_error;
    HAL_StatusTypeDef stop_status;
    uint32_t stop_error_code;
    uint8_t dma_channel_enabled;
    uint8_t rxdmaen_enabled;
    int is_stalled;
    int balance_ok;
    int stop_ok;
    /* Snapshot of 4 consecutive stereo frames (8 slots = 16 halfwords) */
    uint16_t sample_raw_hw[16];
    int32_t  sample_pcm24[8];
} i2s_rx_metrics_t;

void i2s_rx_init(void);
void i2s_rx_poll(void);

#endif /* TICKFORM_I2S_RX_H */

