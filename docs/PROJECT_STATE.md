# Tickform — Project state

Fotografia vivente dello stato corrente. Aggiornare quando cambia realmente il progetto; la cronologia resta in PROJECT_LOG e le decisioni negli ADR.

- **Last updated:** 2026-11-20
- **Reference commit:** 7d71430 — chore: add PROMPT.md entry point and consolidate C01/C02 report
- **Current milestone:** FASE 5 — PCM1808 → STM32 (ricezione I²S2, DMA circolare, diagnostica hardware): QUALIFICA NOMINALE FIRMWARE NORMALE COMPLETATA (FASE 5 NON ANCORA FORMALMENTE CHIUSA)
- **Next milestone:** Gate H01 (Ricognizione e stabilizzazione cablaggi/hardware) → Chiusura formale FASE 5 → (Subordinata) FASE 6 RAW Stream (FASE 6: NON INIZIATA — NON AUTORIZZATA)

## Gate status

- Gate 1 — SWD: PASS (400 kHz, rail 3V3 verificato)
- Gate 2 — minimal firmware: PASS (LED PC6 lampeggiante)
- Gate 3 — MCU clock 170 MHz: PASS (PLL e SysTick convalidati)
- Gate 4 — native USB enumeration: PASS (COM CDC 0483:5740 operativo)
- Gate 5 — LRCK nominale 96 kHz (PCM1808 C): PASS (oscillazione Master verificata)
- Gate 6 — Ricezione hardware I²S2/DMA STM32 (F5-D03): PASS (3.750–3.751 CB in 10s, 0 errori DMA/SPI2/HAL, clock verificati su PB12/13)
- Gate 7 — Collaudo firmware normale operativo F5-A02-R2.1 (F5-N01..N03): PASS (3 prove nominali positive a ~96 kHz: F5-N01-R1 3751 CB, F5-N02-R1 3751 CB, F5-N03 3750 CB; 0 errori; 3 log recuperabili archiviati in docs/evidenze_f5_n01_n03/)
- Gate H01 — Ricognizione e stabilizzazione hardware AS BUILT: OPEN / NEXT (propedeutico alla chiusura FASE 5 e autorizzazione FASE 6)
- Gate 8 — Conversione audio e campioni acustici: PENDING (ingressi analogici aperti; AFE non connesso; sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03; contenuto audio, formato e sincronismo non ancora validati)

## Hardware status

Tutti gli ordini TME, Amazon e AliExpress sono ricevuti completi. Esiti consolidati comunicati dalla supervisione; dettagli e misure nel PROJECT_LOG. BOM **AS PURCHASED**, non AS BUILT. **Ricevuto e FASE 0 COMPLETE non significano hardware funzionalmente validato.**

- **WeAct Studio STM32G431 Core Board (V1.0, STM32G431CBU6):** Visual inspection PASS. A board scollegata: continuità GND/SWD_GND e GND/GND PASS, 3V3/GND circa 540 kΩ, VCC/GND circa 350 kΩ. Gate 1 SWD PASS a 400 kHz; rail 3V3 circa 3.29 V. Gate 2 minimal firmware e LED PC6 validati. Clock MCU 170 MHz nominali validato operativamente nel Gate 3. USB FS CDC enumeration validata nel Gate 4 (COM5 locale, VID:PID 0483:5740 development-only). Alimentazione da cavo USB-C al PC; pin header destro (fila 1..4): 1° 3V3 (3,2 V - 3,3 V), 2° GND, 3° B11 (aperto), 4° V+ (5,0 V da VBUS a valle diodo Schottky).
- **Oscillatore Attivo XO 24.576 MHz (DIP-14 metal can 4-pin):** Visual inspection PASS. Alimentato a 3,3 V da WeAct 3V3; Pin 4 GND, Pin 5 OUT, Pin 8 VCC, Pin 1 OE aperto (pull-up interno). **Conferma operatore (20/11/2026):** XO già saldato su basetta millefori/perfboard con condensatore ceramico di disaccoppiamento da 100 nF presente. OUT collegato direttamente a SCK del breakout PCM1808 C con filo diretto, senza resistenza da 100 Ω esterna.
- **Modulo Breakout PCM1808 C (modulo commerciale generico per TI PCM1808):** La marcatura `SKU:01325` non è accertata fisicamente sull'esemplare C. Chip TSSOP-14 (datasheet ufficiale TI `SLES177B – APRIL 2006 – REVISED AUGUST 2015`).
  - **Bring-up FASE 4 e collaudo FASE 5:** Collaudato con successo in Master Mode (MD0 e MD1 a livello HIGH). **Conferma operatore (20/11/2026):** MD0 e MD1 configurati fisicamente tramite derivazione a Y con una resistenza da 10 kΩ per ciascun ramo a 3,3 V, coerente con lo schema KiCad (conferma della configurazione fisica riferita dall'operatore con livello HIGH confermato operativamente; non costituisce misurazione ohmica strumentale indipendente, demandata alla verifica del Gate H01); pin J1.3 GND del breakout PCM1808 collegato direttamente al GND dello ST-Link; FMT aperto a LOW tramite pull-down interno al chip.
  - **Evidenze sperimentali di banco (12/11/2026):**
    - VCC analogica: 5,0 V (da WeAct V+).
    - VDD digitale: 3,2 V (da WeAct 3V3).
    - Frequenza LRCK: **96,000 kHz rilevati tramite cursori PulseView** (Gate LRCK 96 kHz SUPERATO).
    - Frequenza BCK: circa 16 periodi in 2,625 µs su PulseView, compatibile con **6,144 MHz nominali** ($64 \times F_s$); nessuna misura metrologica esatta a 6,144 MHz dichiarata.
    - Master clock SCK: 24,576 MHz nominali via filo diretto da XO.
  - **Misure di continuità e topologia breakout C:**
    - Misura passiva su C: resistenza tra `AGND chip pin 2` e `GND 2 breakout (J2.2)` pari a circa **0,2 Ω** (confermata con multimetro su C; non estesa automaticamente ad altre continuità interne non misurate).
    - Resistenza serie onboard osservata: circa **100 Ω** lungo la pista interna tra `J2.3 (SCK)` e `Pin 6 (SCKI)` del chip.
    - Continuità 3,3 V (`J2.1 ↔ J1.2`): classificata come **IPOTIZZATA / PRESUNTA su C** (la misura 0 Ω di FASE 0 `PROJECT_LOG.md#L88` appartiene esclusivamente agli esemplari #1 e #2); operativamente funzionante nel collaudo a 96 kHz.
    - Connettore analogico tripolare `J3 (RIN, GND, LIN)`: aperto (NC esterno).
  - **Conversione audio A/D:** **NON ancora verificata** (nessun segnale applicato a RIN/LIN; stadio AFE non connesso). Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03. Il contenuto audio, il formato effettivo dei campioni e il sincronismo completo non sono ancora validati.
- **Vecchi moduli PCM1808 A e B:** Ricevuti in FASE 0 con marcatura SKU:01325. Modulo B impiegato nelle prove preliminari; causa dell'anomalia di mancata oscillazione/clock riscontrata sulla vecchia scheda B: **SCONOSCIUTA**.
- **DS3231 HW-084 / DS3231SN (#1):** Visual inspection PASS, pin SQW identificato. Precauzione confermata: **NON inserire CR2032 non ricaricabile** a causa della rete di carica D2 + R4 (201). Modulo #1: I2C1 (PA15/PB7) e configurazione/generazione SQW 1 Hz validati funzionalmente in FASE 2. Modulo #2 non validato funzionalmente. Metrologia BCK/TIM2 non iniziata.
- **Strumentazione di misura:**
  - Logic analyzer USB 8CH / 24 MHz nominali (PulseView/sigrok): strumento passivo ad alta impedenza. Acquisizione canali passivi: CH1/D0 su LRC (J2.4), CH2/D1 su BCK (J2.6), GND su massa comune. Segnale SCK 24,576 MHz **escluso dall'acquisizione** (oltre limite Nyquist 24 MS/s).
  - Nello schema CAD R0.1, lo strumento compare graficamente ma è configurato con `on_board no` ed è escluso dalla netlist PCB esportata.
  - Multimetro UNI-T UT139S: usato per misure di continuità e tensione.
- **CAD Schema Elettrico:** Progetto nativo KiCad 10 (`hardware/v0_perfboard/schematic/phase4_pcm1808c/`) in revisione **R0.1 APPROVATO TECNICAMENTE DA CHAT OVERVIEW** (13/11/2026). ERC 0 errori / 0 avvisi; netlist e grafica conformi.

## Firmware status

Gate 1/2/3/4 PASS; piattaforma MCU standalone validata:
- Gate 1: SWD a 400 kHz, target voltage 3.22 V.
- Gate 2: minimal firmware blink PC6 (~500 ms ON/OFF).
- Gate 3: Core HSI16 → PLL a 170 MHz nominali, AHB/APB /1, SysTick reload 169999.
- Gate 4: USB FS CDC device CDC ACM nativo (HSI48 + CRS), enumerazione stabile su Windows.
- FASE 2: I2C1 Standard Mode 100 kHz PA15/PB7, configurazione DS3231 SQW 1 Hz.
- FASE 3: TIM2 External Clock Mode 2 via PA0 per conteggio fronti XO (~24.6 M fronti/s).
- FASE 4: Nessuna modifica firmware richiesta per il collaudo del clock hardware Master PCM1808 C. La WeAct è stata utilizzata come fornitore di alimentazione USB pulita (V+ 5V e 3V3).
- FASE 5: Ricezione I2S2 slave (PB12=WS, PB13=CK, PB15=SD) con DMA circolare e diagnostica completate. Firmware normale `tickform_phase5_i2s_rx.bin` (F5-A02-R2.1, 43.756 B, SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`) qualificato operativamente con 3 prove nominali positive (F5-N01-R1, F5-N02-R1, F5-N03; 0 errori DMA/OVR). Requisiti residui aperti: continuous 64-bit sample counter e recupero attivo post-overflow/errori. Requisito futuro registrato per telemetria/report firmware: Boot ID univoco, Test ID progressivo e uptime continuo nelle comunicazioni seriali.

## Software status

Non iniziato. GUI, DSP, viewer e parser RAW da affrontare dopo acquisizione dati reali affidabili da STM32.

## Current architecture

PIEZO → MCP6022 AFE → PCM1808 stereo @ 96 kHz → I2S → STM32G431CBU6 → USB → PC.

Clock: XO attivo 24,576 MHz → PCM1808 master, MD1 = HIGH, MD0 = HIGH, 256 × Fs. Fs nominale 96 kHz; BCK nominale 64 × Fs = 6,144 MHz.

Metrologia: BCK contato da TIM2 + DS3231 INT/SQW 1 Hz in input capture. Il DS3231 è un riferimento indipendente, non genera il sample clock. Mapping previsto e limiti in [V0_BASELINE](architecture/V0_BASELINE.md); decisioni consolidate negli [ADR](decisions/).

## Completed

- Baseline M0, FASE 0, FASE 1, FASE 2, FASE 3: COMPLETE.
- Gate 1 (SWD), Gate 2 (minimal firmware), Gate 3 (MCU clock 170 MHz), Gate 4 (USB enumeration): PASS.
- FASE 4 — PCM1808 C clock bring-up: COMPLETE.
  - Generazione Master clock confermata con XO 24,576 MHz e pull-up 10 kΩ su MD0/MD1.
  - Gate LRCK 96 kHz: PASS (96,000 kHz rilevati tramite cursori PulseView).
  - Gate BCK ~6,144 MHz: PASS (~16 periodi in 2,625 µs su PulseView).
  - Schema CAD nativo KiCad 10 (Rev. R0.1): prodotto, validato con ERC (0 errori/avvisi) e APPROVATO tecnicamente da CHAT OVERVIEW.
  - Script deterministico e portabile `generate_schematic.py` e pacchetto ZIP consolidati.
- FASE 5 — Qualifica nominale firmware normale I²S2/DMA (F5-A02-R2.1): COMPLETE. Tre collaudi nominali positivi (F5-N01-R1 3751 CB, F5-N02-R1 3751 CB, F5-N03 3750 CB; 0 errori) con log archiviati in `docs/evidenze_f5_n01_n03/`.

## Open questions

- Causa dell'anomalia riscontrata sul vecchio breakout PCM1808 B: SCONOSCIUTA.
- Conversione audio A/D del PCM1808 C: non ancora verificata funzionalmente (LIN/RIN aperti; segnale reale assente).
- Gate H01: Ricognizione e stabilizzazione meccanico-elettrica dei cablaggi jumper/Dupont per prevenire disconnessioni accidentali prima dell'autorizzazione di FASE 6.
- AFE definitivo MCP6022: polarizzazione VREF, guadagno e accoppiamento dopo prime acquisizioni.
- Meccanica del pickup piezo.

## Blockers

Nessun blocco aperto. Qualifica nominale firmware normale completata; Gate H01 in preparazione.

## Next actions

1. Esecuzione del **Gate H01**: ricognizione e stabilizzazione fisica delle connessioni e cablaggi su banco.
2. Chiusura formale di FASE 5 subordinata al superamento di Gate H01 e autorizzazione CHAT OVERVIEW.
3. FASE 6 (RAW Streaming su PC via USB): rigorosamente NON INIZIATA e NON AUTORIZZATA.

## Important constraints

- Non intervenire sul banco, sul firmware o sulla topologia elettrica senza specifica autorizzazione.
- Non eseguire commit o push: rimangono soggetti a successiva autorizzazione OVERVIEW.
- Mantenere la distinzione rigorosa tra misure verificate, collegamenti presunti e ipotesi.
