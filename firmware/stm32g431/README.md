# Tickform — FASE 1, Gate 4: native USB CDC diagnostic

Target: WeAct Studio STM32G431 Core Board V1.0, STM32G431CBU6 (UFQFPN48).
Baseline Gate 3 fisicamente validata: e284bd3. Gate 4 PASS e FASE 1 COMPLETE: flash, verify e software reset riusciti;
esiti fisici comunicati dalla supervisione il 2026-09-26. Nessun nuovo accesso
al target durante il consolidamento.

## Hardware e clock

Verificato lo [schema ufficiale WeAct QFN48 V1.0](https://github.com/WeActStudio/WeActStudio.STM32G431CoreBoard/blob/master/Hardware/QFN48/WeAct-STM32G431CxUxCoreBoard_V10_SchDoc.pdf):
J1 USB-C DN1/DN2 -> PA11 USB_DM, DP1/DP2 -> PA12 USB_DP. VBUS alimenta
VCC e il regolatore 3.3 V. Lo stesso cavo USB-C dati al PC alimenta la board
ed espone la USB nativa; ST-Link resta solo SWD, senza alimentazione 3.3/5 V.
Nessun bridge USB/UART necessario. Non alimentare contemporaneamente da VCC esterna
per questa prova, che assume alimentazione esclusiva dal bus USB.

La funzione clock_170mhz e i relativi controlli del Gate 3 restano invariati:
HSI16 /4 x85 /2, ingresso PLL 4 MHz, VCO 340 MHz, SYSCLK/HCLK/PCLK1/PCLK2
170 MHz nominali, bus /1, Range 1 Boost, Flash 4 WS, transizione AHB /2.
La validazione precedente è operativa, non metrologica.

USB usa HSI48 dedicato con selezione esplicita RCC_USBCLKSOURCE_HSI48.
CRS: sorgente USB SOF 1 kHz, divisore /1, polarità rising, reload 47999,
error limit e trimming iniziale ST di default (34 e 64), autotrim e contatore
abilitati da HAL_RCCEx_CRSConfig. Nessuna attesa dei SOF prima dell'attach;
il CRS lavora in hardware senza interrupt dedicato. PLL P/Q restano disabilitati.

STM32G431CBU6 alimenta il transceiver da VDD: non espone VDDUSB separata né
un bit PWR USV da abilitare. La rail circa 3.29 V già misurata è coerente con
il requisito USB di 3.0–3.6 V del [datasheet DS12589, tabella USB](https://www.st.com/resource/en/datasheet/stm32g431cb.pdf).
PA11/PA12 restano nello stato GPIO di reset: il peripheral USB ne assume il
controllo, come nell'esempio ST; non si inventa una selezione alternate-function.
Il PCD abilita il clock USB APB1 e USB_LP_IRQn (priorità 6), gestito da
HAL_PCD_IRQHandler. Endpoint single-buffer: non serve USB_HP_IRQn.
STOP, LPM, remote wakeup e battery charging detection non sono abilitati;
non serve USBWakeUp_IRQn. Il suspend/resume passa al middleware mantenendo i clock.
Questo gate non valida consumi in suspend o conformità USB completa.

## Classe e descrittori

Middleware ufficiale ST USB Device, classe CDC ACM Full Speed, due interfacce.
Stringa prodotto: **Tickform Gate 4**; seriale derivato dall'UID MCU come
nell'esempio ST. Descrittore bus-powered, massimo dichiarato 100 mA (non una misura).

**VID 0x0483 / PID 0x5740: DEVELOPMENT ONLY**, valori dell'esempio CDC ST.
Non sono l'identità USB finale di Tickform e **devono essere sostituiti prima
di qualsiasi rilascio/prodotto**. Nessuna decisione definitiva VID/PID.

CDC serve soltanto al bring-up. Non decide il protocollo finale Tickform:
lo streaming RAW sarà progettato separatamente, anche bulk/vendor-specific.
Sono gestite le richieste line coding e control-line state; nessuna UART viene
configurata. I dati OUT eventualmente ricevuti vengono scartati e la ricezione
riarmata. Nessun echo o streaming applicativo, audio, I2S, DMA audio, DSP o RTOS.

## Implementazione e provenienza

Dipendenza esterna bloccata a STM32CubeG4 1.6.3 già installato, senza download:
CMSIS Core/Device G431, startup e system ufficiali; HAL RCC/RCCEx, Cortex,
PCD/PCDEx e LL USB; USB Device Core e classe CDC compilati dal package.

USB_Device/usbd_conf.c/.h e usbd_desc.c/.h sono adattamenti dell'esempio
Projects/STM32G474E-EVAL/Applications/USB_Device/CDC_Standalone del package,
con copyright ST conservato e licenza originale in USB_Device/LICENSE-ST.txt.
Adattamenti: due interfacce, bus-powered, stringhe diagnostiche, controllo dimensione
allocazione statica, nessun percorso STOP/ripristino clock EVAL. PMA riservata:
0x00–0x3f buffer table; EP0 OUT 0x40, EP0 IN 0x80, CDC IN 0xc0,
CDC OUT 0x100, notification IN 0x140, senza sovrapposizioni.
usb_device.c integra la sequenza HSI48/CRS e le API CDC ufficiali senza bridge UART.
Core/Inc/stm32g4xx_hal_conf.h deriva dalla stessa configurazione ST, limitata
ai moduli necessari; GPIO è incluso come dipendenza di compilazione RCC.
Le licenze ST/CMSIS rimangono applicabili, indipendenti dal placeholder di progetto.

Dopo clock_170mhz, HAL_Init abilita SysTick a 1 ms, reload controllato 169999,
con SysTick_Handler/HAL_IncTick per i timeout HAL. PC6 lampeggia circa 500 ms ON/OFF
tramite HAL_Delay: gli interrupt USB restano attivi durante il ritardo.
LED fisso segnala il failure path; gate3_status 1..7 conserva la diagnostica
precedente, 8 segnala errore HAL/USB iniziale, 170 conferma solo il core clock.
**Il blink e il codice 170 non certificano enumerazione USB.**

Linker e startup invariati; RAM disponibile dichiarata 32 KiB, regione CCM
separata non sommata. Heap riservato 512 B e stack 1024 B. CDC usa allocazione
statica, nessun malloc dinamico.

## Build pulita (PowerShell)

Da firmware/stm32g431, con STM32CubeCLT 1.22.0 / GCC 14.3.1:

```powershell
.\build.ps1 -CubeCltRoot 'B:\Programmi Scaricati\stm32_clt\STM32CubeCLT_1.22.0' -CubeG4Root "$env:USERPROFILE\STM32Cube\Repository\STM32Cube_FW_G4_V1.6.3"
```

CMake --fresh, Ninja --clean-first, -O0, -Wall -Wextra. Build del 2026-09-26 PASS,
senza warning: Flash 31052 B (text 30660 + data 392), RAM allocata 4184 B
(data 392 + bss/riserva heap-stack 3792), CCM 0. L'uso effettivo dello stack
non è stato misurato. Artefatti ignorati in build/gate4/tickform_gate4.{elf,hex,bin,map}.
Gli artefatti Gate 2/3 restano separati. HEX/ELF includono gli indirizzi;
BIN destinato a 0x08000000. Lo script non connette, programma o resetta il target.

## Esito fisico Gate 4 — PASS, 2026-09-26

Flash PASS, verify Download verified successfully, software reset PASS.
Windows ha enumerato Dispositivo seriale USB, hardware ID VID_0483/PID_5740,
senza triangolo giallo o errore driver, indicato come funzionante correttamente;
nessun driver custom necessario. COM5 è stata osservata soltanto durante questo
test: non è una porta fissa né parte del protocollo Tickform.

Enumerazione stabile almeno 60 secondi. Scollegando USB-C device/COM sono
scomparsi correttamente; ricollegandola CDC ha ri-enumerato correttamente.
WeAct alimentata dalla propria USB-C dati al PC; ST-Link solo SWD, senza
alimentare la board. Nessun test dati CDC eseguito, intenzionalmente.

FASE 1 COMPLETE, tutti i quattro Gate PASS. La validazione riguarda
l'enumerazione, non streaming affidabile, throughput audio o protocollo RAW.
FASE 2 — DS3231 standalone NEXT, non iniziata in questo consolidamento.
