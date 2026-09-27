# Tickform — Project state

Fotografia vivente dello stato corrente. Aggiornare quando cambia realmente il progetto; la cronologia resta in PROJECT_LOG e le decisioni negli ADR.

- **Last updated:** 2026-09-27
- **Reference commit:** 97f102b — M0 bootstrap baseline
- **Current milestone:** FASE 2 — DS3231 standalone: COMPLETE
- **Next milestone:** FASE 3 — XO standalone

## Gate status

- Gate 1 — SWD: PASS
- Gate 2 — minimal firmware: PASS
- Gate 3 — MCU clock 170 MHz: PASS
- Gate 4 — native USB enumeration: PASS

## Hardware status

Tutti gli ordini TME, Amazon e AliExpress sono ricevuti completi. Esiti consolidati comunicati dalla supervisione il 2026-09-25; dettagli e misure nel PROJECT_LOG alla stessa data. BOM **AS PURCHASED**, non AS BUILT. **Ricevuto e FASE 0 COMPLETE non significano hardware funzionalmente validato.**

- WeAct Studio STM32G431 Core Board: PCB WeAct Studio V1.0, marcatura STM32G431CBU6 verificata, visual inspection PASS. A board completamente scollegata: continuità GND/SWD_GND e GND/GND PASS, 3V3/GND circa 540 kΩ, VCC/GND circa 350 kΩ; nessun corto evidente. Gate 1 SWD PASS a 400 kHz; rail 3V3 circa 3.29 V, nessun corto evidente dopo saldatura/collegamenti. CubeProgrammer identifica STM32G43x/G44x, Device ID 0x468, Cortex-M4, NVM 128 KB, target voltage circa 3.22 V. Gate 2 esecuzione firmware minimale e LED PC6 validati fisicamente. Clock MCU 170 MHz nominali validato operativamente nel Gate 3; USB FS CDC enumeration validata nel Gate 4. Configurazione validata: WeAct alimentata via USB-C dati collegata al PC; ST-Link solo SWD, nessuna alimentazione 3.3 V o 5 V dallo ST-Link.
- 2 breakout PCM1808 visivamente equivalenti, PCB PCM1808 SKU:01325, chip marking coerente. Visual inspection PASS e nessun corto evidente nei controlli passivi. Topologia completa, rail sotto alimentazione, clock, ADC e configurazione MD/FMT non ancora verificati.
- 2 DS3231 HW-084 con chip DS3231SN ed EEPROM AT24C32: visual inspection PASS, pin SQW identificato, controlli passivi senza corti evidenti. Rete D2 + R4 marcata 201 visibile, associata al circuito di carica: **NON inserire CR2032 non ricaricabile**. Batteria non necessaria per V0. Modulo #1: I2C e SQW 1 Hz validati funzionalmente in FASE 2; accuratezza non misurata, modulo #2 non validato funzionalmente.
- 2 XO marcati 24.5760 MHz: metal can, 4 pin, geometria DIP-14; visual inspection PASS. Il package osservato differisce da quello inizialmente ipotizzato. **NON ALIMENTARE: pinout e supply voltage NOT YET VERIFIED.** Non inferire il pinout dalla geometria; funzione non testata.
- 2 MCP6022-I/P: marcatura coerente, DIP-8 corretto, nessun danno visivo evidente; VISUAL IDENTIFICATION PASS, FUNCTIONAL TEST NOT YET PERFORMED. AFE definitivo aperto.
- RG174 coerente con HELU 400189 / RG174/U e costruzione coassiale miniaturizzata. Controlli a campione PASS: 1 kΩ → 0.998 kΩ; 100 kΩ → 99.1 kΩ; 4.7 MΩ → 4.703 MΩ; 1N4148-DIO forward 0.61 V, reverse OL. Non costituiscono caratterizzazione completa della BOM.
- 10 piezo cablati 27 mm ricevuti, etichetta buzzer-27mm-10pc, visual inspection PASS. Tre campioni: 29 / 33 / 33.6 nF; nessun campione palesemente aperto/anomalo. Sensibilità, risposta meccanica e pickup non validati; meccanica sperimentale.

Strumenti disponibili: ST-Link V2, UNI-T UT139S, saldatore/stazione, stagno, flussante, millefori, Dupont, termorestringente e cavi vari; treccia dissaldante e pompetta aspirastagno ricevute. UT139S functional PASS limitatamente a resistenza, continuità, diode test e capacità usati nella receiving inspection. Logic analyzer 8 CH / 24 MHz nominali ricevuto con cavo USB e fascio cavi/probe, visual inspection PASS; USB/acquisizione usate con successo per la verifica funzionale SQW 1 Hz in FASE 2; nessuna calibrazione metrologica attestata. Uso previsto: SQW, I²C, GPIO, LRCK e I2S; margine limitato per BCK 6,144 MHz, non adatto a caratterizzare seriamente MCLK 24,576 MHz. Oscilloscopio, alimentatore da banco e generatore di funzioni non disponibili secondo lo stato noto.

## Firmware status

Gate 1/2 PASS; baseline Gate 2 fisicamente validata nel commit b9fadb9. Gate 3 ora PASS sulla base degli esiti fisici comunicati il 2026-09-26: firmware compilato, programmato e verificato, software reset riuscito. PC6 osservato per circa 10–15 secondi: circa 0.5 s ON / 0.5 s OFF, nessun LED fisso/failure path e nessun andamento circa 5 s ON/OFF.

Configurazione validata: HSI16, PLLM=4, PLLN=85, PLLR=2; ingresso PLL 4 MHz, VCO 340 MHz, SYSCLK/HCLK/PCLK1/PCLK2 nominali 170 MHz, AHB/APB1/APB2 /1 a regime, Range 1 Boost, Flash 4 WS, transizione AHB /2 come implementata. SysTick reload fisso 169999: test diagnostico coerente con 170 MHz operativamente attivi, non misura metrologica di accuratezza assoluta né prova di stabilità prolungata.

Gate 4 — native USB enumeration PASS: build, flash, verify (Download verified successfully) e software reset riusciti. Windows ha enumerato Dispositivo seriale USB, senza errori driver o triangoli gialli, indicato come funzionante correttamente e senza driver custom. Enumerazione stabile almeno 60 secondi; disconnect/reconnect USB-C PASS. COM5 è soltanto la porta assegnata durante il test, non un identificatore stabile o parte del protocollo.

Core Gate 3 invariato; USB FS PA11=DM/PA12=DP, HSI48 dedicato con CRS sincronizzato ai SOF USB, CDC ACM del middleware ufficiale ST/STM32CubeG4. Product string Tickform Gate 4; VID/PID 0483:5740 DEVELOPMENT ONLY, non identità definitiva, da riesaminare e sostituire prima di qualsiasi release/prodotto. CDC è solo diagnostica di bring-up; protocollo e trasporto RAW restano da progettare separatamente.

Build CMake/Ninja, ARM GCC 14.3.1 tramite CubeCLT 1.22.0 e CubeG4 1.6.3, senza warning: flash 31052 B, RAM allocata 4184 B. Sorgenti in firmware/stm32g431; build/gate4 e tutti gli artefatti Gate 2/3/4 ignorati. Nel Gate 4 era stata verificata solo l’enumerazione; in FASE 2 anche la diagnostica ASCII CDC è stata osservata con successo. FASE 1 COMPLETE: piattaforma MCU standalone validata per SWD, firmware custom, clock nominale 170 MHz e USB FS CDC enumeration. Non validati streaming USB, protocollo RAW, throughput audio, I2S, DMA o metrologia BCK/DS3231.

FASE 2 — DS3231 standalone COMPLETE, esiti fisici comunicati dalla supervisione il 2026-09-27. Modulo #1 HW-084 / DS3231SN, senza batteria, alimentato da WeAct 3V3/GND; VCC e linee idle circa 3.3 V, pull-up I2C onboard circa 4.7 kΩ. I2C1 PA15 SCL / PB7 SDA, AF4 open-drain senza pull-up interne, Standard Mode 100 kHz: PASS. DS3231 7-bit 0x68 (HAL 0xD0) FOUND; letture e scrittura Control necessaria validate. CTRL 0x1C → 0x00, readback PASS; STATUS 0x88, OSF=1 conservato, compatibile con power loss senza batteria e non interpretato come guasto. Temperatura 27.00 °C, sola evidenza di comunicazione reale. Nessuna scrittura data/ora o AT24C32; 32K non utilizzato.

SQW configuration PASS: INTCN/RS2/RS1/A2IE/A1IE azzerati con read-modify-write. SQW physical 1 Hz verification PASS: pull-up SQW onboard circa 4.6 kΩ verso 3V3; logic analyzer USB 8CH/24 MHz con PulseView/sigrok, SQW su CH1/D0 e massa comune, acquisizione 100 kHz / 1 M samples, circa 10 s. Circa 10 cicli regolari, periodo ~1.0 s, HIGH/LOW ~0.5 s, duty ~50%. Verifica funzionale, nessuna accuratezza ppm attribuita; metrologia BCK/TIM2 non iniziata.

Mapping da preservare: PB8/BOOT0 escluso come SCL per questo prototipo; pull-down board circa 10 kΩ con pull-up modulo circa 4.7 kΩ forma il partitore coerente con SCL ~2.2 V osservati. PB6/PB7 scartato: PB6 non dispone di I2C SCL sullo STM32G431CBU6. PA15/PB7 è una correzione del pin mapping, non una modifica architetturale; nessun ADR/change request necessario.

Firmware FASE 2: TIMINGR 0xD0F32F38 derivato da CubeMX per PCLK1 170 MHz, analog filter ON, digital filter 0, rise/fall ipotizzati 1000/300 ns. Core 170 MHz e USB HSI48/CRS invariati. Flash/verify (Download verified successfully) e USB CDC PASS; COM5 osservata, non porta stabile né specifica Tickform. Build senza warning: flash 40792 B, RAM allocata 7384 B; artefatti build/phase2 ignorati. Timeout finiti, diagnostica registri/OSF/temperatura; nessuna scrittura EEPROM.

## Software status

Sostanzialmente non iniziato. GUI non iniziata, da affrontare dopo acquisizione RAW affidabile. Nessun DSP, viewer, synthetic generator, dipendenza Python o ambiente virtuale creato in M0.

## Current architecture

PIEZO → MCP6022 AFE → PCM1808 stereo @ 96 kHz → I2S → STM32G431CBU6 → USB → PC.

Clock: XO attivo 24,576 MHz → PCM1808 master, MD1 = HIGH, MD0 = HIGH, 256 × Fs. Fs nominale 96 kHz; BCK nominale 64 × Fs = 6,144 MHz.

Metrologia: BCK contato da TIM2 + DS3231 INT/SQW 1 Hz in input capture. Il DS3231 è un riferimento indipendente, non genera il sample clock. Mapping previsto e limiti in [V0_BASELINE](architecture/V0_BASELINE.md); decisioni consolidate negli [ADR](decisions/).

Target indicativi, non prestazioni ottenute: V0 circa ±0,5 s/day di errore strumentale sul rate o meglio; V1 avvicinamento a circa ±0,1 s/day. Risoluzione e accuratezza non sono equivalenti.

## Completed

- Ispezione iniziale: root ufficiale esistente e vuota; nessun file preesistente o `.git`.
- Struttura e documentazione M0 preparate, inclusi sei ADR accettati dalla specifica di avvio.
- BOM as purchased, costi indicativi, checklist e formato RAW draft documentati.
- Review M0 superata; baseline iniziale committata (97f102b).
- Repository locale inizializzato su main; remote origin configurato.
- Memoria documentale primaria attiva nel repository; baseline M0 pubblicata su origin/main.
- FASE 0 completa: tutti gli ordini ricevuti, componenti principali identificati, controlli visivi e passivi a campione completati; nessun problema macroscopico rilevato. Le verifiche funzionali appartengono alle fasi successive.

- Gate 1 — SWD e Gate 2 — firmware minimale: PASS; flash/verify riusciti e blink PC6 osservato fisicamente, come comunicato dalla supervisione il 2026-09-26.

- Gate 3 — MCU clock 170 MHz: PASS; flash/verify e test LED diagnostico validati fisicamente il 2026-09-26, senza failure path osservato. Non è una misura di accuratezza assoluta del clock.

- Gate 4 — native USB enumeration: PASS; stabilità almeno 60 s e disconnect/reconnect verificati. FASE 1 — WeAct standalone COMPLETE.

- FASE 2 — DS3231 standalone COMPLETE: I2C1 PA15/PB7, registri, configurazione e verifica fisica SQW ~1 Hz PASS.

## Open questions

- Schema effettivo, alimentazioni e configurazione dei breakout PCM1808.
- XO: pinout e tensione di alimentazione da verificare prima del power-up nella fase appropriata; funzione e prestazioni reali non testate.
- DS3231 #1: I2C e SQW funzionalmente validati, accuratezza e metrologia ancora da verificare; mantenere il divieto di CR2032 non ricaricabile con il circuito di carica presente.
- AFE definitivo, guadagno/shaping e accoppiamento all'ADC dopo misure reali.
- Meccanica del pickup; caratteristiche osservabili nei segnali reali.
- Trasporto USB, framing firmware e dettagli del formato RAW da definire e validare.
- Accuratezza reale della catena e del riferimento; strategia di calibrazione da validare.
- Licenze definitive e prezzi esatti non congelati.

## Blockers

Nessun problema macroscopico rilevato nella receiving inspection. XO: power-up bloccato finché pinout e tensione di alimentazione non saranno verificati. FASE 0 completa non attesta funzionamento o validazione del sistema.

## Next actions

1. FASE 3 — XO standalone NEXT: verificare pinout reale e supply voltage prima di qualsiasi alimentazione, poi relativo bring-up.
2. FASE 3 non avviata in questo consolidamento.

## Important constraints

- Root ufficiale: `A:\Documenti\Tickform`; PC locale sorgente primaria di verità.
- Remote pubblico: https://github.com/Elcosmo/Tickform.git; non creare un'altra repository.
- Preservare RAW e tracciabilità; sample index come base temporale, USB non è clock metrologico.
- Nessun display V0; PC parte integrante della V0; secondo canale mantenuto disponibile.
- Non assumere specifiche AliExpress come certe né componenti ricevuti senza verifica.
- Non confondere ipotesi, risoluzione, accuratezza e misure reali; niente DSP avanzato prima di dati reali affidabili.
- Non modificare l'architettura senza CHANGE REQUEST documentata e decisione della supervisione.
- Nessun secret o informazione personale nel repository; nessuna modifica alle configurazioni Git globali.
- Questa attività consolida firmware già validato ed evidenze della FASE 2, con un commit e push autorizzati. Nessuna modifica al firmware eseguibile, architettura o ADR; non avvia FASE 3; nessun branch, tag o release aggiuntivo.
