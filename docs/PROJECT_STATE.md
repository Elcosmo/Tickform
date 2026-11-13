# Tickform — Project state

Fotografia vivente dello stato corrente. Aggiornare quando cambia realmente il progetto; la cronologia resta in PROJECT_LOG e le decisioni negli ADR.

- **Last updated:** 2026-11-13
- **Reference commit:** 97f102b — M0 bootstrap baseline
- **Current milestone:** FASE 4 — PCM1808 C clock bring-up: COMPLETE
- **Next milestone:** FASE 5 — PCM1808 → STM32 (ricezione I2S, DMA, acquisizione campioni)

## Gate status

- Gate 1 — SWD: PASS
- Gate 2 — minimal firmware: PASS
- Gate 3 — MCU clock 170 MHz: PASS
- Gate 4 — native USB enumeration: PASS
- Gate 5 — LRCK nominale 96 kHz (PCM1808 C): PASS
- Gate 6 — Conversione audio PCM1808: PENDING (non ancora verificata)

## Hardware status

Tutti gli ordini TME, Amazon e AliExpress sono ricevuti completi. Esiti consolidati comunicati dalla supervisione; dettagli e misure nel PROJECT_LOG. BOM **AS PURCHASED**, non AS BUILT. **Ricevuto e FASE 0 COMPLETE non significano hardware funzionalmente validato.**

- **WeAct Studio STM32G431 Core Board (V1.0, STM32G431CBU6):** Visual inspection PASS. A board scollegata: continuità GND/SWD_GND e GND/GND PASS, 3V3/GND circa 540 kΩ, VCC/GND circa 350 kΩ. Gate 1 SWD PASS a 400 kHz; rail 3V3 circa 3.29 V. Gate 2 minimal firmware e LED PC6 validati. Clock MCU 170 MHz nominali validato operativamente nel Gate 3. USB FS CDC enumeration validata nel Gate 4 (COM5 locale, VID:PID 0483:5740 development-only). Alimentazione da cavo USB-C al PC; pin header destro (fila 1..4): 1° 3V3 (3,2 V - 3,3 V), 2° GND, 3° B11 (aperto), 4° V+ (5,0 V da VBUS a valle diodo Schottky).
- **Oscillatore Attivo XO 24.576 MHz (DIP-14 metal can 4-pin):** Visual inspection PASS. Alimentato a 3,3 V da WeAct 3V3; Pin 4 GND, Pin 5 OUT, Pin 8 VCC, Pin 1 OE aperto (pull-up interno). Bypass locale C1 (100 nF) documentato storicamente in FASE 3 (`PROJECT_LOG.md#L180`), non nuovamente verificato a banco nel collaudo C. OUT collegato direttamente a SCK del breakout PCM1808 C con filo diretto, senza resistenza da 100 Ω esterna (a differenza del cablaggio esplorativo temporaneo su PA0 in F3).
- **Modulo Breakout PCM1808 C (modulo commerciale generico per TI PCM1808):** La marcatura `SKU:01325` non è accertata fisicamente sull'esemplare C. Chip TSSOP-14 (datasheet ufficiale TI `SLES177B – APRIL 2006 – REVISED AUGUST 2015`).
  - **Bring-up FASE 4:** Collaudato con successo in Master Mode (MD0 e MD1 a livello HIGH tramite due resistenze di pull-up da 10 kΩ a 3,3 V attestate su J1.2 "3.3"; FMT aperto a LOW tramite pull-down interno al chip).
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
  - **Conversione audio A/D:** **NON ancora verificata** (nessun segnale applicato a RIN/LIN; stadio AFE non connesso).
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
- Prossima fase firmware (FASE 5): Ricezione I2S2 slave (PB12=WS, PB13=CK, PB15=SD) con DMA circolare e contatore campioni continuo a 64 bit.

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

## Open questions

- Causa dell'anomalia riscontrata sul vecchio breakout PCM1808 B: SCONOSCIUTA.
- Conversione audio A/D del PCM1808 C: non ancora verificata funzionalmente (LIN/RIN aperti; segnale reale assente).
- FASE 5: Integrazione I2S2 slave su STM32G431 (PB12 WS, PB13 CK, PB15 SD) con gestione DMA e streaming verso USB.
- AFE definitivo MCP6022: polarizzazione VREF, guadagno e accoppiamento dopo prime acquisizioni.
- Meccanica del pickup piezo.

## Blockers

Nessun blocco aperto. Gate LRCK 96 kHz superato e schema CAD R0.1 approvato.

## Next actions

1. Procedere con la pianificazione e l'avvio della **FASE 5 — PCM1808 → STM32**:
   - Cablaggio dei segnali digitali audio: LRCK → PB12 (I2S2_WS), BCK → PB13 (I2S2_CK), DOUT (J2.5) → PB15 (I2S2_SD).
   - Configurazione firmware STM32 per ricezione I2S2 in modalità Slave Receiver a 24-bit / 96 kHz con clock da PCM1808 Master.
   - Attivazione DMA circolare su buffer audio e contatore continuo di campioni.
   - Prima verifica della conversione audio reale.

## Important constraints

- Non intervenire sul banco, sul firmware o sulla topologia elettrica senza specifica autorizzazione.
- Non eseguire commit o push: rimangono soggetti a successiva autorizzazione OVERVIEW.
- Mantenere la distinzione rigorosa tra misure verificate, collegamenti presunti e ipotesi.
