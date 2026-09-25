# Tickform — Project log

Cronologia **APPEND-ONLY**: aggiungere nuove sezioni in fondo, senza riscrivere la storia. Le decisioni architetturali sono negli ADR; questo log non li sostituisce.

## 2026-09-16 — Avvio M0

- Nome progetto stabilito: Tickform.
- Root locale ufficiale: `A:\Documenti\Tickform`.
- Repository GitHub: https://github.com/Elcosmo/Tickform.git.
- Repository GitHub creata intenzionalmente vuota, secondo la specifica di avvio; contenuto remoto non verificato in M0.
- PC locale definito sorgente primaria di verità; GitHub è il remote pubblico.
- Avvio milestone M0 — Repository Bootstrap.
- Codex utilizzato come esecutore locale; chat di supervisione per architettura, roadmap e review.
- Memoria primaria nel repository, non nelle chat: STATE per stato corrente, LOG per cronologia, HANDOFF per ripartenza e ADR per decisioni.
- Ispezione locale: cartella ufficiale vuota, `.git` assente, nessun conflitto o file preesistente.
- Preparati struttura, documentazione iniziale, sei ADR richiesti, BOM as purchased, costi indicativi, checklist e formato RAW draft v0.1.
- Inizializzato Git su `main` e configurato `origin` sul repository indicato. Nessun commit, push, tag o release; nessuna configurazione Git globale modificata.
- Bootstrap pronto per review; nessun bring-up o risultato sperimentale dichiarato.

## 2026-09-16 — Controllo finale M0

- Verificati 38 file di progetto: Markdown e collegamenti locali, 29 righe BOM, tre aggregati costo, sei ADR e JSON nel draft RAW.
- Tutte le 47 voci della checklist restano non verificate; presenti tutte le voci richieste dalla specifica.
- Verificate regole Gitignore con percorsi virtuali: RAW generici esclusi, RAW in data/examples (anche annidati) ammessi; sorgenti, CSV, JSON e documenti ammessi. Nessun file di acquisizione creato per il controllo.
- Nessun nome progetto errato o possibile secret rilevato dalla scansione dei contenuti; nessun codice applicativo o dato sperimentale aggiunto.
- Struttura mantenuta con README di perimetro, senza .gitkeep. Nessun file aggiunto allo staging, commit o push. Attesa review.

## 2026-09-16 — Correzione costo Amazon

- Il precedente valore indicativo di circa 31 EUR è sostituito dal costo effettivo noto dell'ordine Amazon tools: 32.07 EUR, comunicato dalla supervisione.
- AliExpress resta 48.73 EUR e TME 50–51 EUR; la spesa iniziale di sviluppo aggiornata è 130.80–131.80 EUR, non il costo unitario di un Tickform.

## 2026-09-16 — Baseline M0 registrata

- Review M0 superata; primo commit di baseline creato: 97f102b (chore: bootstrap Tickform repository).
- M0 — Repository Bootstrap dichiarato COMPLETE.
- Prossima fase: FASE 0 — Receiving inspection, da avviare quando i componenti saranno disponibili; nessuna ricezione o verifica hardware dichiarata.
- Memoria primaria del progetto operativa nel repository; PROJECT_STATE riferito al primo commit di baseline.

## 2026-09-25 — Receiving inspection completata

Fonte: esiti consolidati comunicati dalla supervisione il 2026-09-25, trascritti nella memoria del progetto; non nuove prove eseguite da Codex. Test ID e fotografie non forniti con questo aggiornamento. Le voci precedenti restano inalterate.

### Ordini e TME

TME, Amazon e AliExpress: ordini ricevuti completi e inspected. Ricevuto non significa funzionalmente validato.

- 2 MCP6022-I/P: marcatura coerente su entrambi, package DIP-8 corretto, nessun danno visivo evidente. VISUAL IDENTIFICATION PASS; FUNCTIONAL TEST NOT YET PERFORMED.
- RG174: ricevuto coerente con HELU 400189 / RG174/U; costruzione visivamente coerente con coassiale miniaturizzato.
- Campione 1 kΩ Vishay, P/N MBB02070C1001FCT00: misurato 0.998 kΩ, PASS.
- Campione 100 kΩ Yageo, TME MF0207FTE-100K, manufacturer P/N MF0207FTE52-100K: misurato 99.1 kΩ, PASS.
- Campione 4.7 MΩ Vishay, P/N MBA02040C4704FCT00: misurato 4.703 MΩ, PASS.
- 1N4148-DIO: forward 0.61 V, reverse OL, PASS.

Controlli a campione, non caratterizzazione completa della BOM.

### Amazon

- UNI-T UT139S: functional PASS per le sole funzioni utilizzate nella receiving inspection: resistance, continuity, diode test e capacitance.
- Logic analyzer 8 CH / 24 MHz nominali: visual inspection PASS; presenti unità, cavo USB e fascio cavi/probe. USB/acquisition functionality NOT YET TESTED.
- Treccia dissaldante e pompetta aspirastagno ricevute.
- Costo Amazon definitivo noto 32.07 EUR, già correttamente registrato. AliExpress 48.73 EUR, TME 50–51 EUR: totale INITIAL DEVELOPMENT EXPENSE 130.80–131.80 EUR, non costo del singolo Tickform.

### AliExpress — WeAct

WeAct Studio STM32G431 Core Board; PCB marking WeAct Studio V1.0, MCU marking STM32G431CBU6 verificata. Visual inspection PASS, nessun danno evidente.

Controlli passivi con board completamente scollegata:

| Controllo | Esito |
| --- | --- |
| GND ↔ SWD_GND | continuity PASS |
| GND ↔ GND | continuity PASS |
| 3V3 ↔ GND | circa 540 kΩ |
| VCC ↔ GND | circa 350 kΩ |

Nessun corto evidente sulle alimentazioni. SWD / USB / MCU functionality NOT YET TESTED.

### AliExpress — PCM1808

2 breakout ricevuti, visivamente equivalenti. PCB marking PCM1808, SKU:01325; chip marking coerente con PCM1808. Visual inspection PASS, fronte e retro ispezionati. Topologia completa NOT YET FULLY VERIFIED.

Pin esposti, preservando le diciture riportate: `+5V`, `3.3`, `GND`, `MD0`, `MD1`, `FMT`; `3.3V`, `GND`, `SCK`, `LRC`, `OUT`, `BCK`; `RIN`, `LIN`.

| Controllo passivo | PCM1808 #1 | PCM1808 #2 |
| --- | --- | --- |
| GND ↔ GND | continuity PASS | continuity PASS |
| 3.3V ↔ 3.3V | continuity PASS | continuity PASS |
| 3.3V ↔ GND | circa 6.6 MΩ | circa 9 MΩ |
| +5V ↔ GND | OL | circa 13 MΩ |

Conclusione limitata: nessun corto evidente sulle alimentazioni. ADC, clock, rail corretti sotto alimentazione e configurazione MD/FMT NON dichiarati funzionanti o validati.

### AliExpress — DS3231

2 moduli ricevuti, visivamente equivalenti. PCB HW-084, chip marking DS3231SN, EEPROM AT24C32 presente. Pin esposti: `32K`, `SQW`, `SCL`, `SDA`, `VCC`, `GND`. Visual inspection PASS; pin SQW identificato.

Visibile la rete D2 + R4 marcata 201 associata ai comuni moduli con circuito di carica batteria. Precauzione confermata: **NON inserire CR2032 non ricaricabile.** La batteria non è necessaria per V0. Questa osservazione non costituisce caratterizzazione elettrica completa del circuito di carica.

| Controllo passivo | DS3231 #1 | DS3231 #2 |
| --- | --- | --- |
| GND ↔ GND | continuity PASS | continuity PASS |
| VCC ↔ VCC | continuity PASS | continuity PASS |
| VCC ↔ GND | circa 115 kΩ | circa 140 kΩ |

Nessun corto evidente. I²C / SQW / accuracy NOT YET TESTED.

### AliExpress — XO 24.576 MHz

2 oscillatori ricevuti, entrambi marcati 24.5760 MHz. Metal can, 4 pin, geometria DIP-14; visual inspection PASS. Il package reale osservato differisce dalla precedente ipotesi tipo DIP/DIP-8; registrata la differenza senza modificare architettura o ADR.

Pinout NOT YET VERIFIED; supply voltage NOT YET VERIFIED; function NOT YET TESTED. **NON ALIMENTARE gli XO finché pinout e tensione di alimentazione non sono verificati.** Non inferire automaticamente il pinout definitivo dalla geometria del package.

### AliExpress — Piezo

10 piezo cablati 27 mm ricevuti; etichetta confezione buzzer-27mm-10pc. Disco in ottone, ceramica centrale, filo rosso sull'elettrodo ceramico, filo nero sull'ottone. Visual inspection PASS.

Tre esemplari a campione: 29 nF, 33 nF, 33.6 nF. Conclusione limitata: capacità nello stesso ordine di grandezza e nessun campione palesemente aperto/anomalo. Sensibilità uniforme, risposta meccanica uniforme e pickup NON dichiarati validati.

### Stato e memoria

FASE 0 — Receiving inspection: COMPLETE. Tutti gli ordini ricevuti, componenti principali identificati, controlli visivi e passivi a campione completati; nessun problema macroscopico rilevato. FASE 0 COMPLETE non significa hardware funzionante o validato: le verifiche funzionali appartengono alle fasi successive.

Prossima milestone: FASE 1 — WeAct standalone, con obiettivi SWD, firmware minimo, clock MCU e USB. Non avviata in questa attività. Aggiornati STATE, HANDOFF, checklist e BOM; stato received distinto da validazione funzionale. Architettura, ADR e formato RAW invariati. Aperto e prioritario nella fase appropriata: verifica pinout e supply voltage XO prima del power-up.
