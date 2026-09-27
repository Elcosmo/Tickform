/* Phase 2 diagnostic; DS3231 datasheet Rev 10, control/status/temp registers. */
#include "ds3231.h"
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <stdarg.h>

#define DS_ADDRESS (0x68U << 1U) /* HAL expects shifted 7-bit address: 0xD0. */
#define IO_TIMEOUT_MS 100U
#define SQW_MASK 0x1FU /* RS2, RS1, INTCN, A2IE, A1IE -> 0. */
/* STM32CubeMX 6.18.1 official I2cTimingTraitement calculator:
   Standard 100 kHz, kernel 170 MHz, analog filter ON, DNF=0,
   assumed rise=1000 ns / fall=300 ns (not measured). */
#define I2C_TIMING 0xD0F32F38U
static I2C_HandleTypeDef bus;
static char report[1536];
static size_t used;

static void append(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(report + used, sizeof(report) - used, fmt, args);
    va_end(args);
    if (n > 0) {
        size_t available = sizeof(report) - used;
        used += (size_t)n < available ? (size_t)n : available - 1U;
    }
}

static int failed(const char *step, HAL_StatusTypeDef status)
{
    append("FAIL: %s; HAL=%u I2C_error=0x%08lX\r\n"
           "SQW 1 Hz config: FAIL / not confirmed\r\n",
           step, (unsigned)status, (unsigned long)HAL_I2C_GetError(&bus));
    return 0;
}

static HAL_StatusTypeDef read_register(uint8_t reg, uint8_t *data, uint16_t n)
{
    return HAL_I2C_Mem_Read(&bus, DS_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT,
                            data, n, IO_TIMEOUT_MS);
}

int ds3231_bringup(void)
{
    used = 0U;
    report[0] = 0;
    append("Tickform FASE 2 - DS3231\r\nBoot snapshot (repeated every 5 s)\r\n");
    RCC_PeriphCLKInitTypeDef clock = {0};
    clock.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    clock.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    HAL_StatusTypeDef rc = HAL_RCCEx_PeriphCLKConfig(&clock);
    if (rc != HAL_OK) { return failed("I2C1 clock", rc); }
    if (HAL_RCC_GetPCLK1Freq() != 170000000U) {
        return failed("PCLK1 must be 170 MHz", HAL_ERROR);
    }
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_AF_OD;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF4_I2C1;
    gpio.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA, &gpio);
    gpio.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &gpio);
    __HAL_RCC_I2C1_CLK_ENABLE();
    bus.Instance = I2C1;
    bus.Init.Timing = I2C_TIMING;
    bus.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    bus.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    bus.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    bus.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    bus.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    rc = HAL_I2C_Init(&bus);
    if (rc != HAL_OK) { return failed("I2C1 init", rc); }
    rc = HAL_I2CEx_ConfigAnalogFilter(&bus, I2C_ANALOGFILTER_ENABLE);
    if (rc != HAL_OK) { return failed("analog filter", rc); }
    rc = HAL_I2CEx_ConfigDigitalFilter(&bus, 0U);
    if (rc != HAL_OK) { return failed("digital filter", rc); }
    append("I2C1 PA15/PB7 AF4 init: PASS; Standard 100 kHz\r\n");
    rc = HAL_I2C_IsDeviceReady(&bus, DS_ADDRESS, 2U, IO_TIMEOUT_MS);
    if (rc != HAL_OK) { return failed("DS3231 0x68 NOT ACKNOWLEDGED", rc); }
    append("DS3231 0x68: FOUND (address ACK, no silicon ID register)\r\n");
    uint8_t date[7], control, status, temperature[2], after;
    rc = read_register(0x00U, date, sizeof(date));
    if (rc != HAL_OK) { return failed("time/date read", rc); }
    append("TIME/DATE raw 00..06: %02X %02X %02X %02X %02X %02X %02X\r\n"
           "Time validity NOT assumed; no date/time written\r\n",
           date[0], date[1], date[2], date[3], date[4], date[5], date[6]);
    rc = read_register(0x0EU, &control, 1U);
    if (rc != HAL_OK) { return failed("CTRL read", rc); }
    append("CTRL before: 0x%02X\r\n", control);
    rc = read_register(0x0FU, &status, 1U);
    if (rc != HAL_OK) { return failed("STATUS read", rc); }
    append("STATUS: 0x%02X; OSF: %u (not cleared)\r\n", status,
           (unsigned)((status >> 7) & 1U));
    if (status & 0x80U) {
        append("OSF may follow power loss/no battery; not automatically a fault\r\n");
    }
    rc = read_register(0x11U, temperature, sizeof(temperature));
    if (rc != HAL_OK) { return failed("TEMP read", rc); }
    int msb = temperature[0] < 128U ? temperature[0] : (int)temperature[0] - 256;
    int quarters = msb * 4 + (temperature[1] >> 6);
    unsigned magnitude = (unsigned)(quarters < 0 ? -quarters : quarters);
    append("TEMP raw: %02X %02X; TEMP: %s%u.%02u C (last conversion)\r\n",
           temperature[0], temperature[1], quarters < 0 ? "-" : "",
           magnitude / 4U, (magnitude % 4U) * 25U);
    uint8_t desired = control & (uint8_t)~SQW_MASK;
    rc = HAL_I2C_Mem_Write(&bus, DS_ADDRESS, 0x0EU, I2C_MEMADD_SIZE_8BIT,
                           &desired, 1U, IO_TIMEOUT_MS);
    if (rc != HAL_OK) { return failed("CTRL write", rc); }
    rc = read_register(0x0EU, &after, 1U);
    if (rc != HAL_OK) { return failed("CTRL readback", rc); }
    append("CTRL after: 0x%02X\r\n", after);
    /* CONV can self-clear; EOSC/BBSQW must remain as read. */
    if ((after & SQW_MASK) != 0U || (after & 0xC0U) != (control & 0xC0U)) {
        return failed("CTRL readback mismatch", HAL_ERROR);
    }
    append("SQW 1 Hz config: PASS (register readback only)\r\n"
           "Physical SQW measurement: PENDING; SQW/32K not connected to MCU\r\n");
    return 1;
}

const char *ds3231_report(void) { return report; }
