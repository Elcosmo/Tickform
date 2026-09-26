/* Gate 4 integration of STM32CubeG4 1.6.3 CDC_Standalone.
 * Uses the installed official ST HAL/PCD and USB Device CDC middleware.
 * Enumeration diagnostic only: no UART bridge or Tickform application protocol.
 */
#include "stm32g4xx_hal.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_desc.h"

extern PCD_HandleTypeDef hpcd_USB_FS;
void Error_Handler(void);
USBD_HandleTypeDef hUsbDeviceFS;
static uint8_t rx_buffer[CDC_DATA_FS_MAX_PACKET_SIZE];
static uint8_t tx_buffer[CDC_DATA_FS_MAX_PACKET_SIZE];
/* CDC line coding is retained for host requests, without configuring a UART. */
static uint8_t line_coding[7] = {0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08};

static int8_t cdc_init(void)
{
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, tx_buffer, 0U);
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, rx_buffer);
    return USBD_OK;
}

static int8_t cdc_deinit(void) { return USBD_OK; }

static int8_t cdc_control(uint8_t cmd, uint8_t *buf, uint16_t length)
{
    switch (cmd) {
    case CDC_SET_LINE_CODING:
        if (length != sizeof(line_coding)) { return USBD_FAIL; }
        memcpy(line_coding, buf, sizeof(line_coding));
        break;
    case CDC_GET_LINE_CODING:
        if (length != sizeof(line_coding)) { return USBD_FAIL; }
        memcpy(buf, line_coding, sizeof(line_coding));
        break;
    case CDC_SET_CONTROL_LINE_STATE:
    case CDC_SEND_BREAK:
        break;
    default:
        return USBD_FAIL;
    }
    return USBD_OK;
}

static int8_t cdc_receive(uint8_t *buf, uint32_t *length)
{
    /* Discard diagnostic OUT data and re-arm; no echo/streaming protocol. */
    (void)length;
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, buf);
    return (int8_t)USBD_CDC_ReceivePacket(&hUsbDeviceFS);
}

static int8_t cdc_transmit_complete(uint8_t *buf, uint32_t *length, uint8_t epnum)
{
    (void)buf;
    (void)length;
    (void)epnum;
    return USBD_OK;
}

static USBD_CDC_ItfTypeDef cdc_interface = {
    cdc_init, cdc_deinit, cdc_control, cdc_receive, cdc_transmit_complete
};

void USB_LP_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void gate4_usb_init(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_PeriphCLKInitTypeDef peripheral = {0};
    RCC_CRSInitTypeDef crs = {0};

    /* CubeG4 CDC_Standalone clock path. Do not modify HSI16 or core PLL. */
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
    osc.HSI48State = RCC_HSI48_ON;
    osc.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) { Error_Handler(); }
    peripheral.PeriphClockSelection = RCC_PERIPHCLK_USB;
    peripheral.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&peripheral) != HAL_OK ||
        HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_USB) != 48000000U) {
        Error_Handler();
    }

    __HAL_RCC_CRS_CLK_ENABLE();
    crs.Prescaler = RCC_CRS_SYNC_DIV1;
    crs.Source = RCC_CRS_SYNC_SOURCE_USB;
    crs.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
    crs.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000U, 1000U);
    crs.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;
    crs.HSI48CalibrationValue = RCC_CRS_HSI48CALIBRATION_DEFAULT;
    HAL_RCCEx_CRSConfig(&crs);
    /* No wait for SOF before connecting: host SOF starts after attachment.
       No CRS IRQ needed: automatic trimming is hardware controlled.
       PA11/PA12 are controlled by the USB peripheral (RM0440 GPIO section).
       G431CBU6 uses VDD for USB; no separate VDDUSB/USV switch exists. */

    if (USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS) != USBD_OK ||
        USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK ||
        USBD_CDC_RegisterInterface(&hUsbDeviceFS, &cdc_interface) != USBD_OK ||
        USBD_Start(&hUsbDeviceFS) != USBD_OK) {
        Error_Handler();
    }
}
