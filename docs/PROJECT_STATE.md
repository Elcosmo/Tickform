# Tickform — Project state

Fotografia vivente dello stato corrente. Aggiornare quando cambia realmente il progetto; la cronologia resta in PROJECT_LOG e le decisioni negli ADR.

- **Last updated:** 2026-09-16
- **Reference commit:** 97f102b — M0 bootstrap baseline
- **Current milestone:** M0 — Repository Bootstrap: COMPLETE
- **Next milestone:** FASE 0 — Receiving inspection

## Hardware status

Componenti acquistati / in fase di ricezione; non è verificato che siano tutti arrivati. BOM **AS PURCHASED**, non AS BUILT.

- WeAct Studio STM32G431 Core Board con MCU previsto STM32G431CBU6: identificazione fisica da verificare.
- 2 breakout PCM1808 da ispezionare: schema reale non noto.
- 2 moduli DS3231 tipo ZS-042/equivalente: verificare topologia batteria/ricarica.
- 2 XO attivi 24,576 MHz, package tipo DIP/DIP-8 footprint: identificare marcatura, pinout e alimentazione prima di alimentarli.
- 2 MCP6022-I/P; AFE definitivo aperto.
- Circa 10 piezo passivi cablati da 27 mm; meccanica del pickup sperimentale.

Strumenti disponibili dichiarati: ST-Link V2, multimetro, saldatore/stazione, stagno, flussante, millefori, Dupont, termorestringente e cavi vari. Ordinato: Binghe logic analyzer, 8 canali, circa 24 MS/s, per SQW, I²C, GPIO, LRCK e I2S; margine limitato per BCK 6,144 MHz. Non adatto a caratterizzare seriamente MCLK 24,576 MHz. Oscilloscopio, alimentatore da banco e generatore di funzioni non disponibili; non bloccano M0.

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
- Memoria documentale primaria attiva nel repository.

## Open questions

- Schema effettivo, alimentazioni e configurazione dei breakout PCM1808.
- Identità, pinout, tensione e prestazioni reali degli XO.
- Stato di ricezione dei componenti e topologia DS3231.
- AFE definitivo, guadagno/shaping e accoppiamento all'ADC dopo misure reali.
- Meccanica del pickup; caratteristiche osservabili nei segnali reali.
- Trasporto USB, framing firmware e dettagli del formato RAW da definire e validare.
- Accuratezza reale della catena e del riferimento; strategia di calibrazione da validare.
- Licenze definitive e prezzi esatti non congelati.

## Blockers

Nessun blocco rilevato per M0, revisionato e completo. Pubblicazione/sincronizzazione della baseline sul remote come prossima azione. Ricezione e identificazione dell'hardware sono prerequisiti delle rispettive fasi di bring-up; non sono verifiche già completate.

## Next actions

1. Pubblicare/sincronizzare la baseline sul remote.
2. Receiving inspection dei componenti quando disponibili.
3. Aggiornare checklist e PROJECT_STATE con evidenze reali.
4. Successivamente procedere con FASE 1 e seguenti secondo roadmap.

## Important constraints

- Root ufficiale: `A:\Documenti\Tickform`; PC locale sorgente primaria di verità.
- Remote pubblico: https://github.com/Elcosmo/Tickform.git; non creare un'altra repository.
- Preservare RAW e tracciabilità; sample index come base temporale, USB non è clock metrologico.
- Nessun display V0; PC parte integrante della V0; secondo canale mantenuto disponibile.
- Non assumere specifiche AliExpress come certe né componenti ricevuti senza verifica.
- Non confondere ipotesi, risoluzione, accuratezza e misure reali; niente DSP avanzato prima di dati reali affidabili.
- Non modificare l'architettura senza CHANGE REQUEST documentata e decisione della supervisione.
- Nessun secret o informazione personale nel repository; nessuna modifica alle configurazioni Git globali.
- M0 limitato a repository e memoria documentale: niente installazioni, codice applicativo o dati inventati. Dopo review positiva sono autorizzati i due commit di baseline e il push; nessun tag, release, branch aggiuntivo o avvio della FASE 0 in questa attività.
