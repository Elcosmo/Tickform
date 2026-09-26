# Tickform — FASE 1, Gate 2: LED blink

Firmware minimale per WeAct Studio STM32G431 Core Board V1.0, STM32G431CBU6 (UFQFPN48). Gate 2 PASS: il 2026-09-26 la supervisione ha confermato programmazione, verify e blink PC6 fisicamente osservato. Questo firmware costituisce la baseline validata Gate 2.

## Comportamento

PC6 output push-pull, no pull, low speed: HIGH acceso, LOW spento, circa 500 ms per stato. Si usa HSI 16 MHz di reset senza configurare PLL, HSE o 170 MHz. SystemInit ufficiale inizializza FPU/vector table; SystemCoreClockUpdate legge il clock corrente. SysTick è usato in polling, senza interrupt, soltanto per il ritardo visibile: non è un timer metrologico. Nessuna periferica esterna, HAL, RTOS o protocollo.

La prova validata è stata avviata con software reset dopo flash e verify. WeAct alimentata tramite USB-C separata; ST-Link usato per SWD senza alimentare la board tramite 3.3 V o 5 V. Nessuna pretesa di precisione del periodo; Gate 3 escluso.

## Fonti LED e supporto

Verifica del 2026-09-26 nel repository ufficiale WeAct, esempio 01-Blink:

- [board.h](https://github.com/WeActStudio/WeActStudio.STM32G431CoreBoard/blob/master/Examples/01-Blink/Core/Inc/board.h): variante STM32G431CxU6 → GPIOC, GPIO_PIN_6 (la variante CxT6 usa un altro pin).
- [board.c](https://github.com/WeActStudio/WeActStudio.STM32G431CoreBoard/blob/master/Examples/01-Blink/Core/Src/board.c): board_led_set(1) imposta GPIO_PIN_SET; inizializzazione con GPIO_PIN_RESET. Polarità active-high secondo il driver ufficiale.

Nessun codice WeAct copiato. main.c è specifico per questa prova.

## Struttura e dipendenze

CMake + Ninja + CMSIS: per un solo GPIO è un percorso più piccolo e trasparente della generazione HAL/CubeMX. Non esiste un .ioc e non si promette rigenerazione CubeMX; CubeMX non è richiesto per ricompilare.

Richiesti STM32CubeCLT 1.22.0 (GCC 14.3.1) e STM32CubeG4 1.6.3 già installati. I percorsi vengono passati al build, senza cambiare PATH o scaricare dipendenze. Sono compilati direttamente dal package ST:

- Drivers/CMSIS/Include e Device/ST/STM32G4xx/Include;
- Device/ST/STM32G4xx/Source/Templates/gcc/startup_stm32g431xx.s;
- Device/ST/STM32G4xx/Source/Templates/system_stm32g4xx.c.

Il linker STM32G431CBU6_FLASH.ld deriva dal template ST Projects/NUCLEO-G431KB/Templates/STM32CubeIDE/STM32G431KBTX_FLASH.ld, con la descrizione del dispositivo adattata alla variante CBU6. Conservati copyright, licenza e mappa del medesimo STM32G431xB: 128 KiB flash, 32 KiB RAM; regione CCM separata non utilizzata da questa prova e non sommata alla capacità RAM dichiarata. Heap riservato 512 B, stack riservato 1024 B; nessuna allocazione dinamica. I componenti ST/CMSIS restano soggetti alle rispettive licenze originali; il placeholder LICENSE del progetto non le sostituisce.

## Build (PowerShell)

Eseguire dalla cartella firmware/stm32g431, indicando le installazioni locali:

```powershell
.\build.ps1 -CubeCltRoot 'B:\Programmi Scaricati\stm32_clt\STM32CubeCLT_1.22.0' -CubeG4Root "$env:USERPROFILE\STM32Cube\Repository\STM32Cube_FW_G4_V1.6.3"
```

Il wrapper configura CMake/Ninja e compila con -O0. Non contiene comandi di programmazione, connessione ST-Link o reset. Le sole uscite sono nella directory ignorata build/: tickform_gate2.elf, tickform_gate2.hex, tickform_gate2.bin e tickform_gate2.map. HEX/ELF contengono gli indirizzi; il BIN è destinato a 0x08000000.

Il wrapper di build non esegue flash. La programmazione fisica è stata effettuata nella CHAT OPERATIVA con STM32CubeProgrammer CLI: tickform_gate2.hex, verify riuscito e software reset. Osservato LED PC6 regolare circa 500 ms ON / 500 ms OFF. Build validato senza warning: flash circa 1128 B, RAM allocata circa 1544 B. Il primo DEV_CONNECT_ERR era dovuto alla GUI CubeProgrammer ancora connessa ed è stato risolto disconnettendola/chiudendola. Gate 3 clock e Gate 4 USB nativa restano da eseguire.
