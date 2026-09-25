# Tickform — Project state

Fotografia vivente dello stato corrente. Aggiornare quando cambia realmente il progetto; la cronologia resta in PROJECT_LOG e le decisioni negli ADR.

- **Last updated:** 2026-09-25
- **Reference commit:** 97f102b — M0 bootstrap baseline
- **Current milestone:** FASE 0 — Receiving inspection: COMPLETE
- **Next milestone:** FASE 1 — WeAct standalone

## Hardware status

Tutti gli ordini TME, Amazon e AliExpress sono ricevuti completi. Esiti consolidati comunicati dalla supervisione il 2026-09-25; dettagli e misure nel PROJECT_LOG alla stessa data. BOM **AS PURCHASED**, non AS BUILT. **Ricevuto e FASE 0 COMPLETE non significano hardware funzionalmente validato.**

- WeAct Studio STM32G431 Core Board: PCB WeAct Studio V1.0, marcatura STM32G431CBU6 verificata, visual inspection PASS. A board completamente scollegata: continuità GND/SWD_GND e GND/GND PASS, 3V3/GND circa 540 kΩ, VCC/GND circa 350 kΩ; nessun corto evidente. SWD, USB e MCU non ancora testati.
- 2 breakout PCM1808 visivamente equivalenti, PCB PCM1808 SKU:01325, chip marking coerente. Visual inspection PASS e nessun corto evidente nei controlli passivi. Topologia completa, rail sotto alimentazione, clock, ADC e configurazione MD/FMT non ancora verificati.
- 2 DS3231 HW-084 con chip DS3231SN ed EEPROM AT24C32: visual inspection PASS, pin SQW identificato, controlli passivi senza corti evidenti. Rete D2 + R4 marcata 201 visibile, associata al circuito di carica: **NON inserire CR2032 non ricaricabile**. Batteria non necessaria per V0. I²C, SQW e accuratezza non testati.
- 2 XO marcati 24.5760 MHz: metal can, 4 pin, geometria DIP-14; visual inspection PASS. Il package osservato differisce da quello inizialmente ipotizzato. **NON ALIMENTARE: pinout e supply voltage NOT YET VERIFIED.** Non inferire il pinout dalla geometria; funzione non testata.
- 2 MCP6022-I/P: marcatura coerente, DIP-8 corretto, nessun danno visivo evidente; VISUAL IDENTIFICATION PASS, FUNCTIONAL TEST NOT YET PERFORMED. AFE definitivo aperto.
- RG174 coerente con HELU 400189 / RG174/U e costruzione coassiale miniaturizzata. Controlli a campione PASS: 1 kΩ → 0.998 kΩ; 100 kΩ → 99.1 kΩ; 4.7 MΩ → 4.703 MΩ; 1N4148-DIO forward 0.61 V, reverse OL. Non costituiscono caratterizzazione completa della BOM.
- 10 piezo cablati 27 mm ricevuti, etichetta buzzer-27mm-10pc, visual inspection PASS. Tre campioni: 29 / 33 / 33.6 nF; nessun campione palesemente aperto/anomalo. Sensibilità, risposta meccanica e pickup non validati; meccanica sperimentale.

Strumenti disponibili: ST-Link V2, UNI-T UT139S, saldatore/stazione, stagno, flussante, millefori, Dupont, termorestringente e cavi vari; treccia dissaldante e pompetta aspirastagno ricevute. UT139S functional PASS limitatamente a resistenza, continuità, diode test e capacità usati nella receiving inspection. Logic analyzer 8 CH / 24 MHz nominali ricevuto con cavo USB e fascio cavi/probe, visual inspection PASS; USB/acquisizione non testate. Uso previsto: SQW, I²C, GPIO, LRCK e I2S; margine limitato per BCK 6,144 MHz, non adatto a caratterizzare seriamente MCLK 24,576 MHz. Oscilloscopio, alimentatore da banco e generatore di funzioni non disponibili secondo lo stato noto.

## Firmware status

Sostanzialmente non iniziato. Nessun firmware reale, progetto CubeMX o toolchain creato in M0.

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

## Open questions

- Schema effettivo, alimentazioni e configurazione dei breakout PCM1808.
- XO: pinout e tensione di alimentazione da verificare prima del power-up nella fase appropriata; funzione e prestazioni reali non testate.
- DS3231: I²C, SQW e accuratezza da testare; mantenere il divieto di CR2032 non ricaricabile con il circuito di carica presente.
- AFE definitivo, guadagno/shaping e accoppiamento all'ADC dopo misure reali.
- Meccanica del pickup; caratteristiche osservabili nei segnali reali.
- Trasporto USB, framing firmware e dettagli del formato RAW da definire e validare.
- Accuratezza reale della catena e del riferimento; strategia di calibrazione da validare.
- Licenze definitive e prezzi esatti non congelati.

## Blockers

Nessun problema macroscopico rilevato nella receiving inspection. XO: power-up bloccato finché pinout e tensione di alimentazione non saranno verificati. FASE 0 completa non attesta funzionamento o validazione del sistema.

## Next actions

1. In una successiva attività, avviare FASE 1 — WeAct standalone: SWD, firmware minimo, clock MCU e USB.
2. Aggiornare checklist e PROJECT_STATE con evidenze reali delle verifiche funzionali.
3. Seguire le fasi successive della roadmap, verificando pinout e supply voltage XO prima di alimentarlo nella fase appropriata.

## Important constraints

- Root ufficiale: `A:\Documenti\Tickform`; PC locale sorgente primaria di verità.
- Remote pubblico: https://github.com/Elcosmo/Tickform.git; non creare un'altra repository.
- Preservare RAW e tracciabilità; sample index come base temporale, USB non è clock metrologico.
- Nessun display V0; PC parte integrante della V0; secondo canale mantenuto disponibile.
- Non assumere specifiche AliExpress come certe né componenti ricevuti senza verifica.
- Non confondere ipotesi, risoluzione, accuratezza e misure reali; niente DSP avanzato prima di dati reali affidabili.
- Non modificare l'architettura senza CHANGE REQUEST documentata e decisione della supervisione.
- Nessun secret o informazione personale nel repository; nessuna modifica alle configurazioni Git globali.
- Questa attività aggiorna soltanto la memoria degli esiti consolidati della receiving inspection, con commit e push autorizzati. Non modifica architettura, ADR o formato RAW; non avvia FASE 1, firmware o bring-up; nessun branch, tag o release aggiuntivo.
