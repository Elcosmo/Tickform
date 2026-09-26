# Tickform — FASE 1, Gate 3: clock MCU 170 MHz

Firmware diagnostico per WeAct Studio STM32G431 Core Board V1.0 / STM32G431CBU6. Gate 2 resta la baseline fisicamente validata nel commit b9fadb9. Gate 3 PASS: il 2026-09-26 la supervisione ha confermato flash, verify e software reset riusciti; LED PC6 circa 0.5 s ON/OFF osservato per 10–15 secondi, senza LED fisso/failure path né andamento circa 5 s ON/OFF. Configurazione 170 MHz nominali operativamente attiva e coerente con il test; non misura di accuratezza assoluta o prova di stabilità prolungata. Non avvia Gate 4.

## Configurazione e verifica documentale

HSI16 → PLLM /4 → 4 MHz → PLLN ×85 → VCO 340 MHz → PLLR /2 → SYSCLK 170 MHz nominali. AHB /1, APB1 /1, APB2 /1: HCLK, PCLK1 e PCLK2 nominalmente 170 MHz. Uscite PLL P/Q disabilitate; HSE e periferiche esterne non configurati.

Fonti ufficiali consultate il 2026-09-26:

- STM32CubeG4 1.6.3, `Projects/NUCLEO-G431RB/Templates_LL/Src/main.c`, SystemClock_Config: stessi HSI /4 ×85 /2, Boost, Flash 4 WS e passaggio AHB /2.
- [Datasheet STM32G431xB DS12589 Rev 6](https://www.st.com.cn/resource/en/datasheet/stm32g431cb.pdf), tabella 45: ingresso PLL ammesso 2.66–16 MHz, VCO Range 1 ammesso 96–344 MHz, uscita R fino a 170 MHz in Boost. I valori scelti 4/340/170 MHz sono supportati. Il dispositivo non richiede un selettore PLLRGE/PLLVCOSEL come altre famiglie.
- STM32CubeG4, `Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c`: massimo 170 MHz per SYSCLK/HCLK/PCLK1/PCLK2; tabella Flash Range 1 Boost, 4 wait state fino a 170 MHz.
- `stm32g4xx_hal_pwr_ex.c`, HAL_PWREx_ControlVoltageScaling, e header CMSIS STM32G431: Range 1 (VOS=01), Boost abilitato (R1MODE=0); attesa VOSF prima di accelerare.

Sequenza: conferma HSI ready e avvio da reset su HSI; Range 1 Boost; Flash 4 WS con readback; PLL spento prima della configurazione e attesa PLLRDY; AHB temporaneo /2; selezione PLL e attesa SWS; permanenza ad HCLK 85 MHz per almeno 170 NOP (>=2 µs, rispetto al minimo 1 µs del template); AHB finale /1. SystemCoreClockUpdate legge i registri; controlli finali verificano PLL, prescaler, tensione, latenza e SystemCoreClock. Nessun timer aggiuntivo o HAL compilato.

## Diagnostica fisica prevista

PC6 resta output push-pull active-high. SysTick usa direttamente HCLK, senza interrupt, con reload **169999 = 170000000 / 1000 - 1** costante, indipendente da SystemCoreClock. Blink atteso circa 500 ms ON / 500 ms OFF. Se il clock reale fosse 16 MHz pur superando erroneamente i controlli, ogni stato durerebbe circa 5.3125 s e il ciclo 10.625 s. Una configurazione fallita rilevata esplicitamente produce invece LED fisso, non un falso blink PASS.

Ogni attesa di ready/switch e SysTick è limitata a 1000000 letture: timeout di guardia ampio, non una durata metrologica, indipendente da SysTick durante il cambio clock. Al fallimento il firmware memorizza `gate3_status`, cattura i registri nei simboli `gate3_*`, disabilita SysTick e mantiene LED ON fino al reset esterno. Nessun reset automatico o ripiego silenzioso sul clock precedente.

Codici `gate3_status`: 0 configurazione in corso; 1 HSI/condizione iniziale di reset; 2 VOSF; 3 latenza Flash; 4 spegnimento PLL; 5 PLL lock; 6 switch SYSCLK; 7 readback finale o timeout SysTick; 170 configurazione completata. Il codice 170 è un esito software, non una misura indipendente della frequenza. Un arresto del clock CPU non può essere segnalato dal codice stesso.

Per eventuali ripetizioni della prova mantenere la configurazione di alimentazione validata (WeAct via USB-C separata, ST-Link solo SWD), partire da reset e osservare più cicli consecutivi. L'HSI ha tolleranza propria: 170 MHz e i tempi sono nominali; il test distingue errori grossolani, non certifica accuratezza metrologica o stabilità prolungata.

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

Il wrapper esegue configurazione pulita CMake (--fresh) e build Ninja (--clean-first), mantenendo -O0. Gli artefatti Gate 3 sono separati in build/gate3/ e non sostituiscono quelli Gate 2 in build/. HEX/ELF contengono gli indirizzi; il BIN è destinato a 0x08000000. Linker e startup del Gate 2 invariati.

Nessun comando di connessione al target, flash o reset è eseguito dal build. La validazione fisica Gate 3 è registrata nella memoria del progetto il 2026-09-26; Gate 4 resta NEXT e non implementato.
