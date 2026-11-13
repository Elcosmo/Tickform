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

## 2026-09-26 — FASE 1: Gate 1 e Gate 2 PASS

Esiti fisici consolidati comunicati dalla supervisione; nessun nuovo test hardware eseguito da Codex in questa attività.

- Gate 1 — SWD PASS: WeAct Studio STM32G431 Core Board V1.0 / STM32G431CBU6, nessun corto evidente dopo saldatura/collegamenti, rail 3V3 circa 3.29 V. ST-Link comunica a 400 kHz; STM32CubeProgrammer identifica STM32G43x/G44x, Device ID 0x468, Cortex-M4, NVM 128 KB, target voltage circa 3.22 V.
- Gate 2 — firmware minimale PASS: build CMake/Ninja, ARM GCC 14.3.1 tramite STM32CubeCLT 1.22.0, CMSIS STM32CubeG4 1.6.3; CubeMX 6.18.1 disponibile, nessun .ioc. Build riuscito senza warning; flash circa 1128 B, RAM allocata circa 1544 B. Sorgenti validati conservati senza rifattorizzazioni.
- Artefatto programmato: firmware/stm32g431/build/tickform_gate2.hex. STM32CubeProgrammer CLI: connessione, programmazione e verify riusciti (Download verified successfully), seguito da software reset.
- Primo tentativo DEV_CONNECT_ERR: GUI CubeProgrammer ancora connessa al target. Risolto con Disconnect/chiusura della GUI prima della connessione CLI.
- Osservazione fisica: LED onboard PC6 active-high lampeggia regolarmente circa 500 ms ON / 500 ms OFF. Il firmware custom compilato dal progetto viene realmente eseguito sulla MCU.
- Configurazione validata: WeAct alimentata tramite USB-C separata; ST-Link usato per SWD, senza alimentazione 3.3 V o 5 V proveniente dallo ST-Link.
- Clock HSI/default semplice, nessun PLL a 170 MHz; SysTick in polling per il solo ritardo visibile. Nessuna USB, I2S, DMA, UART, RTOS o periferica esterna introdotta. Il blink non costituisce validazione metrologica del clock.
- FASE 0 resta COMPLETE; FASE 1 resta IN PROGRESS. Gate 3 — MCU clock diventa NEXT; Gate 4 — native USB PENDING. FASE 2 DS3231 seguirà solo dopo completamento FASE 1. Architettura e ADR invariati; output build esclusi da Git.

## 2026-09-26 — Gate 3 MCU clock 170 MHz PASS

Esiti fisici comunicati dalla supervisione; nessuna nuova connessione o programmazione hardware eseguita da Codex durante questo consolidamento.

- Build Gate 3 pulita riuscita senza warning con CMake/Ninja, ARM GCC 14.3.1, CubeCLT 1.22.0 e CMSIS CubeG4 1.6.3; flash 1820 B, RAM allocata 1568 B. Artefatto: firmware/stm32g431/build/gate3/tickform_gate3.hex, escluso da Git come gli artefatti Gate 2.
- Configurazione validata: HSI16 → PLL, M=4/N=85/R=2; ingresso 4 MHz, VCO 340 MHz; SYSCLK/HCLK/PCLK1/PCLK2 nominali 170 MHz, AHB/APB1/APB2 /1 a regime. Range 1 Boost, Flash latency 4 WS, transizione AHB /2 come implementata. SysTick reload fisso 169999.
- Target rilevato: Device ID 0x468, STM32G43x/G44x, Cortex-M4, NVM 128 KB, target voltage circa 3.22 V. Programmazione PASS, verify Download verified successfully, software reset riuscito.
- PC6 osservato fisicamente per circa 10–15 secondi: circa 0.5 s ON e 0.5 s OFF. Nessun LED fisso/failure path e nessun comportamento circa 5 s ON / 5 s OFF.
- Gate 3 PASS: configurazione 170 MHz nominali operativamente attiva e coerente con la diagnostica. Non è una misura metrologica di accuratezza assoluta del clock né una prova di stabilità prolungata.
- Checklist: MCU clock verified completata; 3V3 verified completata sulla base della misura reale circa 3.29 V già documentata nel Gate 1. USB enumeration verified resta non completata.
- FASE 1 resta IN PROGRESS; Gate 1/2/3 PASS, Gate 4 — native USB diventa NEXT, non avviato. Firmware Gate 3 validato conservato senza rifattorizzazioni; architettura e ADR invariati.

## 2026-09-26 — Gate 4 PASS e FASE 1 COMPLETE

Esiti fisici comunicati dalla supervisione; nessun nuovo accesso al target durante questo consolidamento. Firmware validato conservato senza rifattorizzazioni.

- Build Gate 4 pulita PASS, senza warning: flash 31052 B, RAM allocata 4184 B, CubeCLT 1.22.0 / ARM GCC 14.3.1 e CubeG4 1.6.3. Artefatto firmware/stm32g431/build/gate4/tickform_gate4.hex escluso da Git.
- Flash PASS, verify Download verified successfully, software reset PASS sulla WeAct STM32G431CBU6.
- Core HSI16 → PLL → 170 MHz nominali, configurazione Gate 3 invariata. USB FS nativa PA11=DM/PA12=DP, HSI48 dedicato e CRS su USB SOF, CDC ACM ufficiale ST; product string Tickform Gate 4.
- Windows: Dispositivo seriale USB, COM5 osservata nel test (assegnazione locale, non porta fissa né identificatore del protocollo). Hardware ID VID_0483 / PID_5740 verificato: DEVELOPMENT ONLY, non identità USB definitiva, da riesaminare e sostituire prima di qualsiasi release/prodotto.
- Nessun triangolo giallo o errore driver; dispositivo indicato come funzionante correttamente, nessun driver custom richiesto. Enumerazione mantenuta almeno 60 s.
- Disconnect/reconnect PASS: scollegando USB-C device/COM scompaiono, ricollegandola CDC ri-enumera correttamente. WeAct alimentata dalla USB-C al PC, ST-Link solo SWD senza alimentare la board.
- Nessun test dati CDC eseguito, intenzionalmente. CDC resta diagnostica di bring-up: protocollo/trasporto RAW da progettare separatamente; streaming USB, throughput audio, I2S, DMA e metrologia BCK/DS3231 non validati.
- Gate 4 PASS; checklist USB enumeration verified completata. Gate 1/2/3/4 PASS: FASE 1 — WeAct standalone COMPLETE.
- FASE 2 — DS3231 standalone NEXT: collegamento, I2C, identificazione/configurazione, INT/SQW esplicitamente a 1 Hz e verifica SQW. Nessuna attività FASE 2 avviata; architettura e ADR invariati.

## 2026-09-27 — FASE 2 DS3231 standalone COMPLETE

Esiti reali comunicati dalla supervisione; firmware validato conservato senza rifattorizzazioni, nessun nuovo accesso al target in questo consolidamento.

- Modulo #1 HW-084 / DS3231SN senza batteria, AT24C32 presente ma non testata/scritta; 32K non usato. VCC e I2C idle ~3.3 V, pull-up I2C onboard ~4.7 kΩ. Mapping definitivo I2C1 PA15=SCL / PB7=SDA, AF4 open-drain; 100 kHz PASS, TIMINGR 0xD0F32F38 (CubeMX, PCLK1 170 MHz, filtro analogico ON/digitale 0, rise/fall ipotizzati 1000/300 ns).
- PB8/BOOT0 escluso: pull-down board ~10 kΩ e pull-up modulo ~4.7 kΩ spiegano SCL ~2.2 V osservata. PB6/PB7 scartato perché PB6 non dispone di I2C SCL. Correzione di mapping del prototipo, nessuna modifica architetturale o ADR/change request.
- Build FASE 2 PASS senza warning, flash 40792 B / RAM allocata 7384 B. Flash PASS, verify Download verified successfully, USB CDC PASS. COM5 osservata nel test, non identificatore stabile o specifica Tickform; CDC rimane diagnostica development-only.
- DS3231 0x68 FOUND (HAL 0xD0), letture registri riuscite: CTRL 0x1C → 0x00, readback PASS; STATUS 0x88, OSF=1 preservato, compatibile con power loss senza batteria e non interpretato come guasto; temperatura 27.00 °C, prova di comunicazione e non metrologia. Nessuna scrittura data/ora.
- SQW configuration PASS: read-modify-write Control, INTCN/RS2/RS1/A2IE/A1IE=0. Pull-up SQW misurata ~4.6 kΩ verso 3V3. SQW physical verification PASS: logic analyzer USB 8CH/24 MHz, PulseView/sigrok, SQW→CH1/D0 e massa comune; 100 kHz / 1 M samples (~10 s). Circa 10 cicli regolari, periodo ~1.0 s, frequenza ~1 Hz, HIGH/LOW ~0.5 s, duty ~50%. Verifica funzionale, nessuna accuratezza ppm attribuita; metrologia BCK/TIM2 non iniziata.
- Checklist I2C communication verified, SQW configured to 1 Hz, SQW 1 Hz verified completate. FASE 2 COMPLETE; FASE 3 — XO standalone NEXT, non avviata: verificare pinout reale e supply voltage prima dell'alimentazione.

## 2026-10-05 — FASE 3 XO standalone COMPLETE

Esiti fisici comunicati dalla supervisione; nessun nuovo accesso al target durante il consolidamento. Firmware XO validato preservato; variante diagnostica MCO precedentemente preparata rimossa perché non necessaria, mai usata come evidenza fisica.

- XO 24.5760 MHz metal can, 4 pin, geometria DIP-14: power-up a 3.3 V PASS, circa 3.3 V misurati (precedentemente 3.29 V). Pin 4 GND, pin 5 OUT, pin 8 VCC, pin 1 OPEN; bypass 100 nF vicino al componente. Nessun surriscaldamento, fumo o comportamento anomalo; nessun test a 5 V.
- OUT -> 100 ohm serie -> WeAct PA0, TIM2_ETR AF14. Resistenza prudenziale per protezione/damping del collegamento temporaneo, non decisione architetturale definitiva. TIM2 32 bit, External Clock Mode 2, rising edge, ETR /1, filtro 0, PSC=0, ARR=0xFFFFFFFF; delta con wrap, normalizzazione sulla finestra HAL tick ~1000 ms, CDC e min/max dall'avvio.
- Flash PASS, verify Download verified successfully, USB CDC PASS. Dopo saldatura del pin A0/PA0, TIM2 counting PASS: 16 finestre consecutive, tutte Signal detected >=1000 Hz PASS e plausibility 24.576 MHz ±5% PASS (23.347200–25.804800 MHz). Circa 24.6 milioni di fronti per finestra; range approssimativo osservato 24.589627–24.602978 MHz.
- FUNCTIONAL / APPROXIMATE FREQUENCY CHECK, non ppm metrology: finestra basata sul clock interno STM32, nessuna misura dell'accuratezza assoluta XO né conformità alla presunta specifica ±20 ppm. La differenza dal nominale non va interpretata come errore reale dell'XO. BCK + DS3231 metrology NOT STARTED.
- Edges=0 iniziale in oltre 16 finestre: pin A0/PA0 non saldato correttamente. Continuità DC presente, flash/USB e alimentazione funzionanti; rimozione ST-Link senza effetto. Dopo saldatura conteggio immediatamente passato a ~24.6 milioni di fronti/s. Continuità DC non garantisce integrità del collegamento per segnali ad alta frequenza.
- Checklist pinout e supply XO completate per il componente usato, con verifica operativa a 3.3 V. FASE 3 COMPLETE; FASE 4 PCM1808 no AFE NEXT: alimentazioni reali, SCK/MCLK, MD0/MD1/FMT, LRCK e BCK da verificare prima di I2S. FASE 4 non iniziata; architettura e ADR invariati, artefatti build ignorati.


## 2026-11-13 — FASE 4 PCM1808 C clock bring-up: Gate LRCK 96 kHz PASS e CAD KiCad R0.1 APPROVATO

Esiti fisici comunicati dalla supervisione e revisione CAD consolidata; nessun intervento hardware o firmware eseguito in questa sessione.

- **Oggetto del collaudo:** Breakout stereo ADC commerciale per Texas Instruments PCM1808, identificato come **esemplare C** (modulo generico, marcatura SKU:01325 non accertata su C). Datasheet di riferimento: TI SLES177B (APRIL 2006 – REVISED AUGUST 2015), package TSSOP-14.
- **Configurazione fisica del banco:**
  - Alimentazione analogica: WeAct Header Pin 4 (V+, ~4,7 V – 5,0 V da USB-C del PC) → Breakout J1.1 (+5V).
  - Alimentazione digitale: WeAct Header Pin 1 (3V3, ~3,2 V – 3,3 V) → Breakout J2.1 (3.3V).
  - Massa comune: WeAct Header Pin 2 (GND) → Breakout J2.2 (GND_2).
  - Terminale J1.3 (GND): lasciato OPEN all'esterno (nessun cavetto esterno installato; massa collegata internamente).
  - Master clock SCK: XO 24.576 MHz DIP-14 (alimentato a 3,3 V con condensatore di bypass 100 nF documentato storicamente in FASE 3, non nuovamente verificato su C) → Pin 5 OUT collegato a Breakout J2.3 (SCK) con **filo diretto** (nessuna resistenza da 100 Ω esterna interposta).
  - Configurazione Master Mode: due resistenze assiali da 10 kΩ attestate fisicamente sul terminale J1.2 ("3.3"), collegate rispettivamente a J1.4 (MD0) e J1.5 (MD1) → Livello logico HIGH (Master 256 × Fs).
  - Formato dati: Pin J1.6 (FMT) lasciato OPEN → Livello logico LOW garantito da pull-down interno al chip (formato I2S standard 24 bit).
  - Segnali non connessi: Uscita J2.5 (OUT) lasciata aperta (nessun collegamento I2S a STM32); connettore analogico tripolare J3 (RIN, GND, LIN) lasciato aperto (AFE non connesso).
- **Misure strumentali e verifiche fisiche:**
  - Tensioni operative: VCC analogica = 5,0 V; VDD digitale = 3,2 V.
  - Word Clock: **LRCK = 96,000 kHz rilevati tramite cursori PulseView**. Gate LRCK 96 kHz SUPERATO.
  - Bit Clock: **BCK rilevato con circa 16 periodi in 2,625 µs su PulseView**, compatibile con i 6,144 MHz nominali ($64 \times F_s$); nessuna misura metrologica esatta a 6,144 MHz dichiarata.
  - Misura passiva di continuità su C: misurati con multimetro tra `AGND chip pin 2` e `GND 2 breakout (J2.2)` circa **0,2 Ω** (misura confermata su C; non estesa automaticamente ad altre continuità interne non misurate).
  - Proprietà passiva osservata del modulo: resistenza serie onboard di circa **100 Ω** lungo la pista interna tra J2.3 (SCK) e il Pin 6 (SCKI) del chip TSSOP-14.
  - Chiarimento storico continuità 3,3 V (J2.1 ↔ J1.2): la misura 0 Ω documentata in FASE 0 (`PROJECT_LOG.md#L88`) appartiene esclusivamente agli esemplari #1 e #2. Sulla scheda C la continuità interna tra i due pin 3.3V è **IPOTIZZATA / PRESUNTA** (non misurata con multimetro prima del test), ed è risultata operativamente attiva nel collaudo a 96 kHz.
- **Stato schede precedenti:** La causa dell'anomalia di mancata oscillazione/clock riscontrata sulla vecchia scheda PCM1808 B è formalmente registrata come **SCONOSCIUTA**.
- **Stato conversione audio:** **NON ancora verificata** (nessun segnale audio applicato a RIN/LIN; AFE non connesso).
- **Schema elettrico CAD:** Prodotto progetto nativo KiCad 10 (`hardware/v0_perfboard/schematic/phase4_pcm1808c/tickform_phase4_pcm1808c.kicad_sch`) con libreria simboli autonoma, script deterministico portabile `generate_schematic.py` e pacchetto ZIP completo.
  - Verificato con KiCad ERC nativo: **0 violazioni, 0 errori, 0 avvisi**.
  - Netlist PCB esportata (`tickform_phase4_pcm1808c.net`): verificata; lo strumento esterno INSTR1 è escluso dalla netlist PCB (`on_board no`).
  - **Revisione R0.1 APPROVATA TECNICAMENTE DA CHAT OVERVIEW** (13/11/2026).
- **Checklist e stato:** FASE 4 — PCM1808 C clock bring-up: COMPLETE. Prossima milestone: FASE 5 — PCM1808 → STM32 (connessione I2S2 slave, DMA circolare, streaming e prima validazione conversione audio).
