# Tickform — Chat handoff

Do not redesign the project from scratch. Treat accepted ADRs and PROJECT_STATE as the current baseline.

Tickform è un progetto DIY destinato a essere sviluppato e pubblicato come progetto open-source/open-hardware per un timegrapher avanzato per orologi meccanici. La strategia di licenza è ancora in review; le licenze non sono state formalmente adottate. Obiettivo: preservare waveform RAW e costruire misure riproducibili di BPH, rate, beat error e amplitude, con timegrapher plot e analisi successive basate su dati reali.

## Baseline e decisioni consolidate

WATCH → PIEZO → MCP6022 AFE → PCM1808 stereo 96 kHz → I2S → STM32G431CBU6 / WeAct Core Board → USB → PC.

XO attivo 24,576 MHz, PCM1808 master MD1/MD0 HIGH, 256 × Fs; BCK nominale 64 × Fs = 6,144 MHz. BCK → TIM2 external clock; DS3231 INT/SQW 1 Hz → TIM2 input capture. DS3231 non genera il sample clock. Il sample index è la base temporale; USB non è un clock metrologico. Nessun display V0. RAW sempre conservabile e secondo canale utile per sviluppi futuri.

Non riaprire senza evidenza le scelte dei sei ADR: ADC/rate, MCU, RAW, metrologia BCK/DS3231, PC senza display e XO V0 con possibile riferimento migliore V1. PCB V1 dovrà considerare EXT REF/PPS. In caso di problema tecnico serio, proporre una [CHANGE REQUEST](change_requests/README.md), senza cambiare silenziosamente la baseline.

## Stato corrente

Aggiornamento 2026-09-25: FASE 0 — Receiving inspection: COMPLETE. Prossima milestone FASE 1 — WeAct standalone (SWD, firmware minimo, clock MCU, USB), non avviata. Root ufficiale `A:\Documenti\Tickform`, PC sorgente primaria; remote https://github.com/Elcosmo/Tickform.git. Git su `main`; baseline M0 97f102b e aggiornamento memoria 3e96ee2 già pubblicati.

Tutti gli ordini TME, Amazon e AliExpress ricevuti completi; componenti principali identificati, controlli visivi e passivi a campione completati, nessun problema macroscopico rilevato. Ricevuto e FASE 0 COMPLETE non significano hardware funzionalmente validato. Firmware, software e GUI non iniziati; nessuna acquisizione RAW di orologio disponibile. Dettagli e misure della receiving inspection nel PROJECT_LOG del 2026-09-25.

## Open questions e prossime azioni

In una successiva attività procedere con FASE 1. WeAct STM32G431CBU6 identificata, ma SWD/USB/MCU non testati. PCM1808 SKU:01325: topologia completa, rail sotto alimentazione, ADC, clock e MD/FMT ancora da verificare. DS3231 HW-084 / DS3231SN con AT24C32 e rete di carica D2 + R4 (201): NON inserire CR2032 non ricaricabile; batteria non necessaria per V0, I²C/SQW/accuratezza non testati. XO marcati 24.5760 MHz, metal can 4 pin con geometria DIP-14: NON ALIMENTARE finché pinout e supply voltage non sono verificati; non inferire il pinout dal package.

AFE definitivo e meccanica pickup restano sperimentali; protocollo USB, framing e formato RAW non congelati. Licenze under review; accuratezza e costi unitari non misurati. Spesa iniziale di sviluppo 130.80–131.80 EUR, incluso Amazon 32.07 EUR; non costo unitario.

Seguire le fasi 0–10 della checklist. Ordine DSP: RAW acquisition → waveform viewer → event detection → BPH → rate → beat error → amplitude → timegrapher plot → analisi avanzata. Niente DSP sofisticato prima di dati reali affidabili. Target V0 ±0,5 s/day o meglio e futuro V1 circa ±0,1 s/day sono obiettivi indicativi, non risultati.

## Ruoli e memoria

- **Overview / supervisione:** architettura, roadmap, decisioni e review.
- **Operativa:** coordinamento del bring-up, raccolta di evidenze e condizioni dei test da riportare nel repository.
- **Codex:** esecuzione locale delle specifiche, organizzazione file, controlli, diff/stato Git e segnalazione di problemi; non decide autonomamente cambi architetturali.

[PROJECT_STATE](PROJECT_STATE.md) contiene lo stato attuale, [PROJECT_LOG](PROJECT_LOG.md) la storia append-only, gli [ADR](decisions/) le decisioni accettate e [V0_BASELINE](architecture/V0_BASELINE.md) i dettagli tecnici e il mapping previsto. Questa pagina è una sintesi di passaggio, non la cronologia completa.
