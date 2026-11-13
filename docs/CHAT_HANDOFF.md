# Tickform — Chat handoff

Do not redesign the project from scratch. Treat accepted ADRs and PROJECT_STATE as the current baseline.

Tickform è un progetto DIY destinato a essere sviluppato e pubblicato come progetto open-source/open-hardware per un timegrapher avanzato per orologi meccanici. La strategia di licenza è ancora in review; le licenze non sono state formalmente adottate. Obiettivo: preservare waveform RAW e costruire misure riproducibili di BPH, rate, beat error e amplitude, con timegrapher plot e analisi successive basate su dati reali.

## Baseline e decisioni consolidate

WATCH → PIEZO → MCP6022 AFE → PCM1808 stereo 96 kHz → I2S → STM32G431CBU6 / WeAct Core Board → USB → PC.

XO attivo 24,576 MHz, PCM1808 master MD1/MD0 HIGH, 256 × Fs; BCK nominale 64 × Fs = 6,144 MHz. BCK → TIM2 external clock; DS3231 INT/SQW 1 Hz → TIM2 input capture. DS3231 non genera il sample clock. Il sample index è la base temporale; USB non è un clock metrologico. Nessun display V0. RAW sempre conservabile e secondo canale utile per sviluppi futuri.

Non riaprire senza evidenza le scelte dei sei ADR: ADC/rate, MCU, RAW, metrologia BCK/DS3231, PC senza display e XO V0 con possibile riferimento migliore V1. PCB V1 dovrà considerare EXT REF/PPS. In caso di problema tecnico serio, proporre una [CHANGE REQUEST](change_requests/README.md), senza cambiare silenziosamente la baseline.

## Stato corrente

Aggiornamento 2026-11-13: M0 COMPLETE. FASE 0 — Receiving inspection: COMPLETE. FASE 1 — WeAct standalone: COMPLETE. FASE 2 — DS3231 standalone: COMPLETE. FASE 3 — XO standalone: COMPLETE. **FASE 4 — PCM1808 C clock bring-up: COMPLETE.** Gate 1 SWD PASS; Gate 2 minimal firmware execution PASS; Gate 3 MCU clock 170 MHz PASS; Gate 4 native USB enumeration PASS; **Gate 5 LRCK nominale 96 kHz: PASS**. Root ufficiale `A:\Documenti\Tickform`, PC sorgente primaria; remote https://github.com/Elcosmo/Tickform.git. Git su `main`.

Tutti gli ordini TME, Amazon e AliExpress ricevuti completi; componenti principali identificati, controlli visivi e passivi a campione completati.

FASE 2: I2C1 PA15=SCL / PB7=SDA, AF4, mapping finale validato. PB8/BOOT0 escluso. DS3231 0x68 FOUND, Control 0x1C→0x00/readback PASS, SQW ~1 Hz osservato con logic analyzer; OSF=1 preservato.

FASE 3: XO a 3.3 V, pin 4 GND / pin 5 OUT / pin 8 VCC / pin 1 OPEN, bypass locale 100 nF. OUT -> 100 ohm serie temporanea -> PA0 / AF14 / TIM2_ETR, External Clock Mode 2. Conteggio ~24.6 MHz e plausibility ±5% PASS.

FASE 4: Collaudo clock hardware su **breakout PCM1808 C** (modulo commerciale generico, TI TSSOP-14, datasheet SLES177B Rev. Agosto 2015).
- Alimentazioni da WeAct: V+ ~5,0 V su J1.1 (+5V), 3V3 ~3,2 V su J2.1 (3.3V), GND su J2.2 (GND_2); J1.3 aperto all'esterno.
- Clock XO 24,576 MHz via filo diretto su J2.3 (SCK), nessuna R esterna da 100 Ω; resistenza interna onboard ~100 Ω osservata su pista SCK → Pin 6 (SCKI).
- Configurazione Master 256 × Fs: MD0 e MD1 a livello HIGH con due resistenze pull-up da 10 kΩ a 3,3 V su J1.2; FMT aperto (pull-down interno a LOW).
- Risultati fisici: **LRCK = 96,000 kHz rilevati tramite cursori PulseView**; **BCK con circa 16 periodi in 2,625 µs su PulseView**, compatibile con 6,144 MHz nominali ($64 \times F_s$); nessuna misura metrologica esatta a 6,144 MHz dichiarata.
- Misure interne: AGND chip pin 2 → GND 2 breakout circa **0,2 Ω** su scheda C. Continuità 3,3 V (J2.1 ↔ J1.2) classificata come presunta su C (misura 0 Ω in FASE 0 appartenente solo a #1 e #2).
- Anomalia della vecchia scheda B: causa **SCONOSCIUTA**.
- Conversione audio A/D: **NON ancora verificata** (RIN/LIN aperti, AFE non connesso).
- Schema CAD KiCad 10 nativo (`hardware/v0_perfboard/schematic/phase4_pcm1808c/`): revisione **R0.1 APPROVATA TECNICAMENTE DA CHAT OVERVIEW** (13/11/2026), ERC 0 errori/avvisi, script deterministico portabile.

## Open questions e prossime azioni

Prossimo passo: **FASE 5 — PCM1808 → STM32**:
1. Connessione fisica linee I2S: LRCK (J2.4) → PB12 (I2S2_WS), BCK (J2.6) → PB13 (I2S2_CK), DOUT (J2.5) → PB15 (I2S2_SD).
2. Firmware STM32: periferica SPI2/I2S2 configurata in Slave Receiver a 24-bit / 96 kHz, DMA circolare, gestione contatore campioni continuo a 64 bit.
3. Prima verifica funzionale della conversione audio reale da PCM1808 C.

DS3231 HW-084 / DS3231SN con AT24C32 e rete di carica D2 + R4 (201): NON inserire CR2032 non ricaricabile; batteria non necessaria per V0. XO validato funzionalmente a 3.3 V; accuratezza assoluta e secondo esemplare ancora da verificare. AFE definitivo MCP6022 e meccanica pickup restano sperimentali.

Seguire le fasi 0–10 della checklist. Ordine DSP: RAW acquisition → waveform viewer → event detection → BPH → rate → beat error → amplitude → timegrapher plot → analisi avanzata. Niente DSP sofisticato prima di dati reali affidabili. Target V0 ±0,5 s/day o meglio e futuro V1 circa ±0,1 s/day sono obiettivi indicativi, non risultati.

## Ruoli e memoria

- **Overview / supervisione:** architettura, roadmap, decisioni e review.
- **Operativa:** coordinamento del bring-up, raccolta di evidenze e condizioni dei test da riportare nel repository.
- **Codex:** esecuzione locale delle specifiche, organizzazione file, controlli, diff/stato Git e segnalazione di problemi; non decide autonomamente cambi architetturali.

[PROJECT_STATE](PROJECT_STATE.md) contiene lo stato attuale, [PROJECT_LOG](PROJECT_LOG.md) la storia append-only, gli [ADR](decisions/) le decisioni accettate e [V0_BASELINE](architecture/V0_BASELINE.md) i dettagli tecnici e il mapping previsto. Questa pagina è una sintesi di passaggio, non la cronologia completa.
