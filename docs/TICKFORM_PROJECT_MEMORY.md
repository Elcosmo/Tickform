# TICKFORM — MEMORIA CENTRALE PERSISTENTE DEL PROGETTO
**Master Technical Reference & System Memory Document**  
**Stato documento:** REVISIONE CONSOLIDATA AGGIORNATA (20 Novembre 2026) — Mandato F5-CLOSEOUT-DOC / H01
**Autore:** Antigravity (Archivista di Progetto e Ingegnere di Sistema)  
**Supervisione:** CHAT OVERVIEW  
**Repository Locale:** `A:\Documenti\Tickform`  
**Baseline Git:** commit [`7d714302867f93a5351c12164b412b08d0ae2e98`](https://github.com/Elcosmo/Tickform/commit/7d714302867f93a5351c12164b412b08d0ae2e98) (`main`, allineato a `origin/main`)
**Ultimo aggiornamento documentale:** 20/11/2026

---

## Note di Aggiornamento — Mandato F5-CLOSEOUT-DOC / H01 (20/11/2026)
La presente revisione recepisce gli esiti operativi del collaudo del firmware normale I²S/DMA e le prescrizioni del mandato esecutivo della supervisione:
1. **Qualifica nominale firmware normale completata con successo:** Flash controllato di `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` (F5-A02-R2.1, 43.756 B, SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`). Tre prove nominali da 10s positive (F5-N01-R1 3751 CB, F5-N02-R1 3751 CB, F5-N03 3750 CB; 0 errori DMA/OVR). Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03; il contenuto audio, il formato effettivo dei campioni e il sincronismo completo non sono ancora validati.
2. **Archiviazione log ed evidenze sperimentali:** I tre file di log originali recuperabili (`F5_N02_RESET.txt`, `F5_N02_R1_RESET.txt`, `F5_N03_POWER_CYCLE.txt`) sono archiviati in [`docs/evidenze_f5_n01_n03/`](./evidenze_f5_n01_n03/) con manifest verificato `MANIFEST_EVIDENZE_F5N01_N03_20261120.txt`. Per F5-N01 (FAIL al primo boot per connettore Dupont scollegato; collegamento esatto non identificato nella documentazione) e F5-N01-R1 non esistono file di log originali recuperabili sul PC; i relativi esiti provengono direttamente dai rapporti operativi trasmessi dall'operatore alla CHAT OVERVIEW. Preservato anche F5-N02 (eseguito ma non validabile per campioni azzerati). Riconosciuto formalmente che i report duplicati nella USB CDC ogni 5s sono ritrasmissioni automatiche e non test indipendenti.
3. **Hardware AS BUILT confermato dall'operatore:** 1) Pin J1.3 GND del PCM1808 collegato direttamente a GND ST-Link; 2) XO già saldato su basetta millefori con condensatore ceramico da 100 nF; 3) MD0 e MD1 con derivazione a Y con una resistenza da 10 kΩ per ciascun ramo verso 3,3 V; 4) DS3231 conforme (nessuna batteria CR2032 non ricaricabile inserita).
4. **Inserimento Gate H01 nella roadmap:** Ricognizione e stabilizzazione hardware delle connessioni (prevenzione distacchi accidentali Dupont) tra FASE 5 e FASE 6.
5. **Requisiti residui aperti e stato di FASE 5:** FASE 5 NON ANCORA FORMALMENTE CHIUSA. Continuous 64-bit sample counter e recovery attiva overflow/errori rimangono aperti e non completati. FASE 6 rigorosamente NON INIZIATA e NON AUTORIZZATA.
6. **Requisito futuro di telemetria firmware registrato:** Inserimento nelle future versioni firmware di Boot ID univoco, Test ID progressivo e contatore di uptime (ms/s) per distinguere formalmente ritrasmissioni e riavvii.

## Note di Rettifica R3.1 (18/11/2026)
La presente revisione candidata R3.1 recepisce integralmente e puntualmente le 5 osservazioni emerse dalla revisione indipendente di CHAT OVERVIEW (`TICKFORM_R3_REVIEW_INDIPENDENTE_20261118.md`):
1. **R3-ERR-01 (Unità di misura DMA Run 1):** Rettificato il calcolo del volume di dati trasferito nel Run 1: con `PSIZE=1` (16 bit / half-word) e `MSIZE=1`, il decremento di `CNDTR` da 2048 a 1752 corrisponde esattamente a **296 trasferimenti da 16 bit, ovvero 592 byte** (non 296 byte).
2. **R3-ERR-02 (Natura fenomenologica delle transizioni GPIO post-stop):** La descrizione dei Run 1, 3 e 4 è stata formulata in termini rigorosamente fenomenologici riferiti alla specifica finestra di campionamento IDR post-stop (10 ms eseguiti *dopo* `HAL_I2S_DMAStop`), evitando inferenze indebite sullo stato continuo del segnale durante i 10 secondi di streaming.
3. **R3-ERR-03 (Allineamento BRINGUP_CHECKLIST ed Error Handling):** Chiarito che l'assenza di interrupt di errore (0 err) nei test nominali Run 2, 5A e 5B documenta la stabilità del trasporto dati e il funzionamento dei meccanismi di monitoraggio, ma la voce di checklist per il recupero automatico runtime post-errore rimane aperta (`[ ]`), non essendo stato testato un algoritmo di ripristino automatico.
4. **R3-ERR-04 (Riconciliazione Stop DMA Status=1 / ErrorCode=0x40 / StopOK=SI):** Chiarito formalmente che `Status=1` (`HAL_ERROR`) ed `ErrorCode=0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) sono il comportamento sistematico e documentato del driver ST HAL (`stm32g4xx_hal_i2s.c`) in modalità Slave RX quando il clock esterno continua ad oscillare durante lo stop; il firmware diagnostico gestisce esplicitamente tale condizione convalidando l'arresto (`StopOK=SI`) dopo verifica dello stato dei registri; durante i 10 secondi di streaming non si è verificato alcun errore.
5. **R3-ERR-05 (Conferma dimensione binario normale):** Confermato su disco che `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` misura esattamente **43.756 byte** con SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` (il valore 43.748 era un refuso di battitura isolato nella tabella di `RAPPORTO_F5_D03.md`).

## Note di Revisione R3 (18/11/2026)
La presente revisione candidata R3 recepisce integralmente le prescrizioni del Mandato R3 e i rilievi della revisione indipendente ChatGPT (`TICKFORM_REVIEW_R2_20261118.md`):
1. **Integrazione completa dei log F5-D03:** Censiti e descritti tutti i 5 file di log Tera Term archiviati in [`docs/evidenze_f5_d03/`](./evidenze_f5_d03/), con hash SHA-256 verificati contro il manifest. Identificati esattamente **6 run diagnostici distinti**, distinguendo formalmente le ritrasmissioni periodiche CDC (aventi contatori identici) dai test indipendenti.
2. **Rettifica categorica del firmware dei 6 riavvii:** L'evidenza della sessione e la conferma dell'operatore attestano che sulla STM32 era programmato `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`) e che nessun re-flash ha avuto luogo. Durante i 6 riavvii (3 reset NRST + 3 cicli power-cycle USB) **il firmware in esecuzione era `tickform_phase5_diag`, NON `tickform_phase5_i2s_rx`**. I 6 reboot rappresentano un'evidenza osservazionale di ripetibilità del boot e del LED (nessun LED fisso), non una serie di log numerici certificati.
3. **Firmware normale operativo F5-A02-R2.1:** È classificato formalmente come **approvato e compilato** (`tickform_phase5_i2s_rx.bin`, SHA-256 `04B95777...`), ma **NON ancora collaudato sul banco con clock operativi**. Il collaudo formale con log salvato costituisce il prossimo gate aperto obbligatorio prima della chiusura formale di FASE 5.
4. **Riconciliazione campionamento GPIO IDR e Teoria Nyquist:** Corretto il range reale misurato a **1908–2067 kSps (~1,9–2,1 MSps)**. PB12 (LRCK 96 kHz) è campionato a ~10 campioni per semiperiodo (~1920 transizioni in 10 ms), mentre PB13 (BCK 6,144 MHz) è **sotto Nyquist**: 0 transizioni su PB13 non provano l'assenza fisica del clock per effetto di aliasing o sincronismo. Precisato che `run_pin_input_test()` opera **dopo** lo stop DMA.
5. **Rigorosa distinzione CAD vs Banco AS BUILT:** Separata la prescrizione dello schema KiCad R0.1 (due resistenze distinte da 10 kΩ R1/R2) dalla realtà fisica del banco AS BUILT (livello HIGH accertato operativamente, ma valore ohmico e topologia a Y non misurati con multimetro e non confermati). KiCad R0.1 è preservato intatto.
6. **Portabilità e Backup Indipendente (C01-B-ERR-03):** Tutti i collegamenti nei Markdown utilizzano percorsi relativi rispetto a `docs/`. Si distingue formalmente l'archivio documentale R3.1 dallo snapshot completo del repository (`tickform_full_repository_snapshot_C01.zip`), dalle copie fisiche indipendenti (`B:`, `D:`) e dalla cronologia Git (`git bundle`).
7. **Limiti tecnici sul DMA:** ~3750 callback in 10s dimostrano la conformità del trasporto dati I²S→DMA, ma non attestano la qualità del segnale analogico audio, l'assenza di campioni persi o l'affidabilità dello streaming USB continuo.
8. **Stato della Roadmap e FASE 6:** FASE 6 rimane rigorosamente **NON INIZIATA e NON ANCORA AUTORIZZATA**.

---

## 1. Identità e Scopo del Progetto

### 1.1. Cos'è TICKFORM
**TICKFORM** è uno strumento di misura embedded e una suite software open-source e open-hardware per la diagnosi metrologica, la calibrazione e l'analisi acustica avanzata degli **orologi meccanici**.  
Il sistema è progettato per colmare il divario tra i cronocomparatori commerciali economici (es. serie Weishi 1000/1900), basati su hardware proprietario a soglia fissa e algoritmi opachi, e la strumentazione scientifica di laboratorio.

### 1.2. Problema che risolve
Gli strumenti commerciali economici trasformano il segnale del sensore piezoelettrico in impulsi digitali tramite circuiti di comparazione analogica a soglia fissa prima di qualsiasi elaborazione. Questa riduzione distrugge l'informazione acustica sottesa al ticchettio (urto del dente della ruota di scappamento sulla leva d'entrata, sblocco dell'ancora, caduta del dente sulla leva d'uscita), impedendo l'analisi della salute dello scappamento, della lubrificazione e delle anomalie del treno d'ingranaggi.  
Inoltre, tali strumenti soffrono di derive termiche del riferimento di tempo locale, limitando la precisione della misura della marcia giornaliera (Rate).

TICKFORM risolve radicalmente il problema applicando il principio cardine di **preservazione integrale della forma d'onda grezza (RAW audio preservation)**:
1. Il segnale del pickup piezoelettrico viene amplificato da uno stadio analogico (AFE) a basso rumore.
2. Viene digitalizzato a **96 kHz / 24 bit stereo** tramite un ADC audio dedicato (Texas Instruments PCM1808) in modalità Master.
3. Lo stream audio PCM viene acquisito via I²S2 da un microcontrollore a 170 MHz (STMicroelectronics STM32G431CBU6) tramite DMA circolare e trasferito via USB nativa al PC host.
4. L'accuratezza temporale dell'ADC viene tracciata e calibrata in continuo misurando l'oscillatore audio locale rispetto a un RTC/TCXO ad altissima stabilità (Maxim DS3231SN a $\pm 2\ 	ext{ppm}$).
5. Tutti i dati grezzi vengono archiviati su PC host in formato standard aperto (`.raw` + metadati `.json`) per consentire elaborazioni DSP riproducibili.

### 1.3. Funzionalità previste per il prodotto finale
- **Misura dei parametri orologieri fondamentali:**
  - **Frequenza di oscillazione (BPH - Beats Per Hour):** Riconoscimento automatico delle alternanze orarie standard (18.000, 19.800, 21.600, 25.200, 28.800, 36.000 BPH) e non standard.
  - **Marcia giornaliera (Rate):** Scarto in secondi/giorno (s/d) con risoluzione di $0,1\ 	ext{s/d}$ e incertezza target $\le \pm 0,5\ 	ext{s/d}$ nella V0 (target $\le \pm 0,1\ 	ext{s/d}$ nella V1 con PCB dedicato).
  - **Errore di battuta (Beat Error):** Asimmetria del semi-periodo di oscillazione espressa in millisecondi (risoluzione $0,1\ 	ext{ms}$).
  - **Ampiezza di oscillazione (Amplitude):** Angolo di rotazione del bilanciere espresso in gradi ($^{\circ}$), calcolato dall'intervallo temporale tra il primo urto (sblocco) e il terzo urto (caduta) in funzione dell'angolo di levata (Lift Angle) impostabile dall'utente.
- **Visualizzazione avanzata:**
  - Tracciamento in tempo reale del tradizionale "nastro grafico" del cronocomparatore (doppia traccia dei colpi dell'ancora).
  - Oscillogramma ad alta risoluzione del singolo battito con marcatura dei tre eventi acustici dello scappamento svizzero.
  - Spettrogramma FFT e densità spettrale di potenza per l'identificazione di risonanze, rumori di strisciamento e difetti del treno del tempo.
- **Archiviazione e Metrologia:**
  - Esportazione dei dataset in formato RAW non compresso per analisi retrospettive, certificazioni e benchmark di algoritmi DSP.
  - Tracciamento della deriva dell'oscillatore audio tramite time-stamping hardware.

### 1.4. Requisiti di precisione e target metrologici
| Parametro | Risoluzione Target | Incertezza Target V0 (Perfboard) | Incertezza Target V1 (PCB Pro) |
| :--- | :---: | :---: | :---: |
| **Marcia (Rate)** | $0,1\ 	ext{s/giorno}$ | $\le \pm 0,5\ 	ext{s/giorno}$ | $\le \pm 0,1\ 	ext{s/giorno}$ |
| **Errore di Battuta (Beat Error)** | $0,1\ 	ext{ms}$ | $\le \pm 0,1\ 	ext{ms}$ | $\le \pm 0,05\ 	ext{ms}$ |
| **Ampiezza (Amplitude)** | $1^{\circ}$ | $\le \pm 3^{\circ}$ ($150^{\circ} \div 330^{\circ}$) | $\le \pm 1^{\circ}$ ($150^{\circ} \div 330^{\circ}$) |
| **Base dei Tempi (Frequenza Audio)** | $1\ 	ext{Hz}$ | $\pm 2\ 	ext{ppm}$ correlata a TCXO DS3231 | $\pm 1\ 	ext{ppm}$ TCXO onboard |

---

## 2. Architettura Generale di Sistema

L'architettura hardware e firmware di TICKFORM è strutturata in stadi funzionali disaccoppiati:

```text
[Orologio Meccanico]
         │ (vibrazione acustica)
         ▼
[Sensore Piezo 27mm]
         │ (tensione ad alta impedenza)
         ▼
[AFE: MCP6022-I/P] ── (Gain ~50-100, Vref 2.5V)
         │ (segnale analogico condizionato)
         ▼
[ADC: PCM1808 C] ◀── [Oscillatore XO 24.576 MHz] (Master Clock SCK)
         │ (I2S Master: BCK 6.144 MHz, LRCK 96 kHz, DOUT)
         ▼
[MCU: WeAct STM32G431] ◀── [TCXO: DS3231SN 1Hz SQW] (Calibrazione Metrologica TIM2)
   ├── I2S2 Slave Receiver
   ├── DMA1 Channel 1 (Circular Buffer)
   └── USB Full Speed CDC (Native USB)
         │ (USB FS Packet Streaming)
         ▼
[PC Host: Python/DSP Suite]
   ├── Archiviazione RAW (.raw + .json)
   ├── Calcolo Metrologico (BPH, Rate, Beat Error, Amplitude)
   └── UI Nastro Grafico & Visualizzatore Forma d'Onda
```

### 2.1. Compromessi e decisioni consolidate
1. **Separazione ADC e MCU:** L'ADC opera come **I²S Master** pilotato dal proprio oscillatore locale a 24,576 MHz, sgravando l'albero dei clock della MCU da jitter e consentendo una frequenza di campionamento audio indipendente dal PLL di sistema (170 MHz).
2. **Architettura V0 vs V1:** La versione V0 impiega breakout commerciali cablati su perfboard per accelerare il bring-up e validare il concetto metrologico a costi minimi. La versione V1 integrerà l'intera catena su PCB custom a 4 layer con piani di massa separati (AGND/DGND) e TCXO integrato.
3. **Preservazione dei dati su PC:** Nessuna elaborazione distruttiva o estrazione di parametri avviene sulla MCU; la STM32 funge da convertitore di protocollo, interfaccia DMA e contatore metrologico. L'elaborazione pesante è delegata al PC host.

---

## 3. Componenti Hardware e Distinta Base (BOM)

### 3.1. Distinzione fondamentale: AS PURCHASED vs AS BUILT
- **BOM AS PURCHASED:** Elenco dei componenti acquistati per il progetto (ordini TME, Amazon, AliExpress). Tutti i lotti risultano formalmente consegnati e ispezionati visivamente in FASE 0 ([`bom/V0.0_AS_PURCHASED.md`](../bom/V0.0_AS_PURCHASED.md)). La disponibilità d'acquisto non attesta automaticamente la funzionalità del componente nel circuito.
- **Hardware AS BUILT (Banco Sperimentale Reale — Aggiornato alle conferme dell'operatore del 20/11/2026):** Il circuito effettivamente assemblato e operante sul banco sperimentale di FASE 5, che comprende:
  - WeAct Studio STM32G431 Core Board alimentata da porta USB-C del PC.
  - Oscillatore attivo XO DIP-14 a 24,576 MHz alimentato a 3,3 V: **confermato già saldato su basetta millefori/perfboard con condensatore di disaccoppiamento ceramico da 100 nF presente**.
  - Modulo breakout PCM1808 (esemplare C) alimentato a 5 V (analogico) e 3,3 V (digitale). **Confermato pin J1.3 GND del breakout PCM1808 collegato direttamente al GND dello ST-Link V2**.
  - Resistenze di pull-up su MD0 e MD1 per la modalità Master 96 kHz: **l'operatore ha confermato la configurazione fisica della derivazione a Y con una resistenza da 10 kΩ per ciascun ramo verso 3,3 V**, coerente con lo schema KiCad (conferma della configurazione fisica riferita dall'operatore con livello logico HIGH confermato operativamente; non costituisce misurazione ohmica strumentale indipendente, demandata alla verifica del Gate H01).
  - Connessioni I²S volanti tramite cavetti jumper Dupont verso i pin PB12 (WS), PB13 (CK) e PB15 (SD) della WeAct.
  - Modulo RTC Maxim DS3231 con batteria non inserita (pericolo di sovraccarica della rete D2+R4 per batterie CR2032 non ricaricabili), collegato via I2C1 (PA15/PB7) e pin SQW su PA1.

### 3.2. Riepilogo componenti chiave e stato di validazione
| Sottosistema | Componente Specifico | Ruolo | Stato di Validazione a Banco |
| :--- | :--- | :--- | :--- |
| **MCU Board** | WeAct Studio STM32G431CBU6 (V1.0) | Controller embedded, I2S Slave, DMA, USB | **VALIDATO** (SWD, 170 MHz, USB CDC, I2S DMA) |
| **Oscillatore**| XO DIP-14 24.576 MHz metal can | Master Clock per ADC (256 × Fs) | **VALIDATO** (FASE 3 contatore fronti; FASE 4 clock C) |
| **ADC Audio** | Breakout TI PCM1808 (Esemplare C) | Digitalizzazione 96 kHz / 24 bit stereo | **VALIDATO TRASPORTO** (Clock 96 kHz e DMA ok; AFE non connesso) |
| **Vecchio ADC**| Breakout TI PCM1808 (Esemplare B) | Precedente esemplare di test | **GUASTO** (Causa: TOTALMENTE SCONOSCIUTA) |
| **RTC/TCXO** | Modulo Maxim DS3231SN | Riferimento metrologico $\pm 2\ 	ext{ppm}$ (1 Hz SQW) | **VALIDATO STANDALONE** (I2C1 e SQW 1 Hz ok in FASE 2) |
| **Op-Amp AFE** | Microchip MCP6022-I/P (DIP-8) | Amplificatore a basso rumore per piezo | **ISPEZIONATO F0** (Non ancora collegato a banco) |
| **Sensore** | Disco piezoelettrico passivo 27 mm | Trasduttore acustico da contatto | **ISPEZIONATO F0** (Non ancora collegato a banco) |

### 3.3. Tracciamento dei costi di sviluppo
I costi complessivi sostenuti per i lotti V0 ammontano a circa **131,31 €** (inclusi componenti di scorta, spedizioni e IVA), integralmente tracciati in [`bom/cost_tracking.csv`](../bom/cost_tracking.csv).

---

## 4. Schema KiCad Canonico e Stato di Maturità

### 4.1. File e collocazione
I sorgenti CAD nativi approvati risiedono nella cartella:  
[`hardware/v0_perfboard/schematic/phase4_pcm1808c/`](../hardware/v0_perfboard/schematic/phase4_pcm1808c/)
- Progetto KiCad 10: `tickform_phase4_pcm1808c.kicad_pro`
- Foglio schema (A3 Landscape): `tickform_phase4_pcm1808c.kicad_sch`
- Libreria simboli autonoma: `tickform_symbols.kicad_sym`
- Rapporto ERC nativo KiCad: `erc_report.rpt` (**0 Errors, 0 Warnings**)
- Netlist: `tickform_phase4_pcm1808c.net`
- Esportazioni grafiche: `tickform_phase4_pcm1808c.pdf`, `tickform_phase4_pcm1808c.svg`
- Generatore deterministico: `generate_schematic.py`

### 4.2. Perimetro e limiti dello schema KiCad R0.1
1. **Riferito a FASE 4:** Lo schema documenta il circuito impiegato per convalidare la generazione del clock a 96 kHz sul modulo **PCM1808 C**.
2. **I²S e STM32 non connessi nel CAD:** I pin audio del breakout (`LRC` J2.4, `BCK` J2.6) sono collegati esclusivamente ai canali di misura passiva dell'analizzatore logico (`INSTR1`). Il pin dati `OUT` (J2.5) e l'ingresso analogico `J3` sono lasciati aperti (NC). **Non vi è alcun collegamento disegnato verso i pin I²S della STM32 (PB12, PB13, PB15).**
3. **DS3231 assente:** Il modulo RTC Maxim DS3231 non compare nello schema R0.1.
4. **Resistenze di pull-up:** Lo schema prescrive due resistenze da 10 kΩ (`R1` e `R2`). Sul banco fisico AS BUILT il valore ohmico effettivo e la topologia a Y non sono stati misurati con multimetro.
5. **Integrità:** Lo schema KiCad R0.1 è confermato **integro e non modificato** (SHA-256 `16AB73A3...`). L'aggiornamento a Revisione R0.2 (per includere PB12/13/15 e DS3231) è demandato a formale autorizzazione della supervisione.

---

## 5. Collegamenti Sperimentali Attuali e Matrice di Confronto

### 5.1. Matrice comparativa a tre colonne
| Linea di Segnale / Alimentazione | (A) Schema KiCad R0.1 Approvato | (B) Prototipo Fisico Costruito (FASE 4) | (C) Configurazione Sperimentale Banco F5-D03 |
| :--- | :--- | :--- | :--- |
| **Alimentazione +5V Analogica** | WeAct Pin 4 (`V+`) → MOD1 Pin `J1.1 (+5V)` | Cavo volante diretto WeAct V+ → J1.1. Misurati 5,0 V. | Identica (WeAct V+ → J1.1). |
| **Alimentazione +3.3V Digitale** | WeAct Pin 1 (`3V3`) → MOD1 Pin `J2.1 (3.3V)` | Cavo volante diretto WeAct 3V3 → J2.1. Misurati 3,29 V. | Identica (WeAct 3V3 → J2.1). |
| **Massa Comune GND** | WeAct Pin 2 (`GND`) → MOD1 Pin `J2.2 (GND_2)` | Cavo volante diretto WeAct GND → J2.2. Massa comune. | Identica (WeAct GND → J2.2). |
| **Terminale J1.3 (GND)** | Aperto all'esterno (NC) con nota presunzione interna. | Nessun cavetto esterno installato su J1.3. | Lasciato aperto all'esterno. |
| **Oscillatore XO Supply & Bypass**| VCC 3.3V da WeAct; GND a massa; C1 100 nF bypass. | C1 (100 nF) documentato in F3, non riesaminato su C. | Stesso montaggio breadboard con C1 bypass. |
| **Master Clock SCK** | XO Pin 5 (`OUT`) → MOD1 Pin `J2.3 (SCK)`. | Filo diretto senza resistenze esterne. R_onboard ~100 Ω. | Identica (XO OUT → J2.3). |
| **Pull-Up MD0 / MD1** | **Due resistenze distinte da 10 kΩ (`R1`, `R2`)**. | Livello HIGH presente. Valori e topologia non verificati. | **Derivazione a Y; resistenze NON misurate con multimetro; livello HIGH accertato.** |
| **Formato Dati FMT (J1.6)** | Lasciato OPEN (pull-down interno chip a LOW per I2S). | Aperto all'esterno. Livello logico LOW confermato. | Lasciato aperto all'esterno. |
| **Word Clock (LRCK / WS)** | `J2.4 (LRC)` → Sonda Logic Analyzer `INSTR1 CH1/D0`. | Sonda LA collegata. Misurati 96,000 kHz su PulseView. | **Cavetto Dupont volante da `J2.4` a `PB12` della WeAct.** |
| **Bit Clock (BCK)** | `J2.6 (BCK)` → Sonda Logic Analyzer `INSTR1 CH2/D1`. | Sonda LA collegata. Osservati 16 periodi in 2,625 µs. | **Cavetto Dupont volante da `J2.6` a `PB13` della WeAct.** |
| **Data Output (DOUT / SD)** | `J2.5 (OUT)` lasciato **OPEN / NC**. | Aperto all'esterno (ricezione non attiva in FASE 4). | **Cavetto Dupont volante da `J2.5` a `PB15` della WeAct.** |
| **Modulo DS3231 RTC** | **Non presente** nello schema KiCad R0.1. | Standalone in FASE 2 (I2C1 su PA15/PB7; SQW 1 Hz). | Connesso a banco su I2C1 + SQW per metrologia. |
| **Ingressi Analogici AFE (J3)** | Ingressi `J3.1..J3.3` lasciati **OPEN / NC**. | Connettore tripolare aperto. Nessun cavo collegato. | Lasciato aperto (non connesso). |

---

## 6. Architettura Firmware e Sottosistemi STM32

### 6.1. Target di compilazione disponibili
Il progetto risiede in [`firmware/stm32g431/`](../firmware/stm32g431/). Il sistema CMake definisce tre target distinti:
1. `tickform_phase3_xo`: Target baseline di FASE 3. Sorgente: `Core/Src/xo_counter.c`. Monitora PA0/TIM2_ETR. Genera `build/phase3/tickform_phase3_xo.bin` (37.004 byte, SHA-256 `E0B8F706...`).
2. `tickform_phase5_i2s_rx`: Target operativo normale di FASE 5. Sorgente: `Core/Src/i2s_rx.c`. Riceve I²S2 Slave DMA circolare e gestisce streaming USB CDC. Genera `build/phase5/tickform_phase5_i2s_rx.bin` (43.756 byte, SHA-256 `04B95777...`). **Stato: compilato e approvato crittograficamente, collaudo a banco con clock attivi PENDENTE.**
3. `tickform_phase5_diag`: Target di diagnostica interna hardware di FASE 5 (F5-D02 / F5-D03). Sorgente: `Core/Src/i2s_rx_diag.c`. Esegue snapshot registri, monitor CNDTR a 1 kHz, verifica indirizzi CPAR/CMAR e campionamento GPIO IDR. Genera `build/diag/tickform_phase5_diag.bin` (52.308 byte, SHA-256 `29786361...`). **Stato: programmato e collaudato con successo a banco il 17/11/2026.**

Script di build: [`firmware/stm32g431/build.ps1`](../firmware/stm32g431/build.ps1) (default target: `phase3`). Toolchain: STM32CubeCLT 1.22.0 (`arm-none-eabi-gcc` 14.3.1), CMake/Ninja, firmware package STM32CubeG4 V1.6.3.

### 6.2. Albero dei clock (Clock Tree) e configurazione MCU
- **Sorgente di sistema:** Oscillatore HSI interno a 16 MHz moltiplicato tramite PLL a **170 MHz** (Core, AHB, APB1, APB2).
- **USB Device Clock:** Derivato da PLL 'Q' a 48 MHz dedicati.
- **Interfaccia I²S2:** Configurato come Slave Receiver; i clock di campionamento provengono interamente dall'esterno (PCM1808 Master).

### 6.3. Sottosistema I²S2 Slave Receiver e DMA
- **Periferica:** `SPI2` in modalità I²S Slave Receiver (`I2S_MODE_SLAVE_RX`), formato Philips I²S standard 24 bit su frame 32 bit (`I2S_DATAFORMAT_24B`), clock polarità LOW.
- **Pinout STM32 (AF5):**
  - `PB12`: I2S2_WS (LRCK, Word Select, 96 kHz).
  - `PB13`: I2S2_CK (BCK, Bit Clock, 6,144 MHz).
  - `PB15`: I2S2_SD (DOUT, Serial Data, campioni PCM).
- **Canale DMA:** DMA1 Channel 1, mappato su DMAMUX1 Request ID 12 (`SPI2_RX`). Modalità circolare, incremento memoria abilitato, dimensione trasferimento 16 bit (Halfword), priorità HIGH.
- **Buffer circolare:** Buffer in SRAM da 2048 halfwords (1024 slot a 32 bit = 4096 byte).
- **Interrupt e Callback:** `HAL_I2S_RxHalfCpltCallback()` ad ogni mezzo buffer (1024 halfword); `HAL_I2S_RxCpltCallback()` a fine buffer. A 96 kHz stereo, la frequenza attesa delle callback totali è di **375 callback al secondo** (~3750 callback in una finestra di 10 secondi).
- **Limiti interpretativi:** Il conteggio regolare di ~3750 callback dimostra che il trasporto dati hardware I²S→DMAMUX→DMA1 opera nominalmente. **Non dimostra automaticamente la corretta conversione analogica del segnale audio, l'assenza assoluta di drop a livello di campione o l'affidabilità dello streaming continuo su PC.**

### 6.4. Riconciliazione prestazionale del campionamento GPIO IDR (F5-D03)
Nel modulo diagnostico F5-D03, la funzione `run_pin_input_test()` esegue un loop intensivo di lettura del registro `GPIOB->IDR` per 10.000 µs (10 ms) mantenendo la configurazione pin AF5:
- **Frequenza di campionamento reale misurata su target:** Compresa tra **1908 kSps e 2067 kSps (~1,9–2,1 MSps)** (la stima preliminare di 11–12 MSps formulata in F5-D02 era teorica e non rispecchiava il carico di fetch/loop su flash/SRAM).
- **Efficacia su PB12 (LRCK, 96 kHz nominali):** Il campionamento a ~2 MSps acquisisce circa 10 campioni per semiperiodo, risultando pienamente idoneo a verificare le commutazioni del segnale. Nei test regolari sono state contate esattamente **1920–1922 transizioni in 10 ms**, perfettamente coincidenti con un'onda quadra a 96 kHz.
- **Limiti teorici su PB13 (BCK, 6,144 MHz nominali):** La frequenza di campionamento di ~2 MSps è **sotto la frequenza di Nyquist** (che richiederebbe > 12,3 MSps). Il campionamento produce aliasing: transizioni maggiori di zero indicano attività di toggle (rilevate da 7290 a 8152 transizioni nei run regolari), ma **zero transizioni campionate non dimostrano in modo definitivo l'assenza fisica del clock**, potendo derivare da sincronismo di fase o battimento. Una verifica definitiva richiede strumentazione esterna dedicata (analizzatore logico a 24 MHz).
- **Timing del test:** La verifica IDR viene eseguita **dopo** l'arresto della ricezione DMA (`HAL_I2S_DMAStop`), fotografando lo stato dei pin nei 10 ms immediatamente successivi al test, non durante i 10 secondi di ricezione I2S.

### 6.5. Riconciliazione dell'arresto DMA (Status=1, ErrorCode=0x40, StopOK=SI)
Nei log diagnostici F5-D03 (inclusi i run nominali Run 2, 5A e 5B), la riga di chiusura della prova riporta:
```text
Stop I2S DMA: Status=1 ErrorCode=0x00000040 StopOK=SI
```
L'analisi combinata del codice applicativo e del driver ST HAL documenta:
1. **Comportamento nativo del driver ST HAL (C01-B-ERR-01):** Nel file `Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.c` (linee 1450–1456), la funzione `HAL_I2S_DMAStop` in modalità `I2S_MODE_SLAVE_RX` imposta incondizionatamente `HAL_I2S_ERROR_BUSY_LINE_RX` (`0x00000040`) e ritorna `HAL_ERROR` (`1`) senza verificare esplicitamente il flag BSY nel ramo esaminato. Si tratta del comportamento predeterminato di quel ramo del driver ST HAL per Slave RX, non di una misura attiva del clock da parte del driver, né di un'anomalia di ricezione.
2. **Criterio software per StopOK=SI (C01-B-ERR-02):** In `firmware/stm32g431/Core/Src/i2s_rx_diag.c` (linee 704–712), il firmware diagnostico verifica direttamente che:
   - `(DMA1_Channel1->CCR & DMA_CCR_EN) == 0` (canale DMA spento).
   - `(SPI2->CR2 & SPI_CR2_RXDMAEN) == 0` (richiesta DMA I2S disabilitata).
   - il codice di ritorno coincida con `HAL_I2S_ERROR_BUSY_LINE_RX` (oppure `HAL_OK` / `ERROR_NONE`).
   In tal caso imposta `stop_ok = 1` (stampato come `StopOK=SI`). Gli ulteriori registri (quali `SPI2->CR1.SPE` o `I2SCFGR.I2SE`) fanno parte dello snapshot diagnostico post-stop separato, non della condizione software di `stop_ok`.
3. **Assenza di errori durante lo streaming:** Durante l'intera finestra di acquisizione di 10 secondi, i contatori registrano `Raw IRQ SPI2 = 0`, `HAL I2S Error CB = 0`, `SPI2 OVR contati = 0`. Il valore `ErrorCode=0x40` si manifesta esclusivamente durante la procedura di arresto software e non indica alcun errore o perdita di campioni durante l'acquisizione.

---

## 7. Roadmap Originale e Stato di Avanzamento Attuale

La roadmap originaria del progetto TICKFORM si articola in 11 milestone sequenziali:

| Fase | Denominazione e Obiettivo | Stato Formale | Deliverable / Criteri di Chiusura |
| :---: | :--- | :---: | :--- |
| **M0** | **Bootstrap & Baseline:** Repository, documentazione iniziale, regole e ADR. | **COMPLETA** | Commit `97f102b`, `3e96ee2`. |
| **FASE 0** | **Receiving Inspection:** Ricezione componenti, ispezione visiva, sicurezza. | **COMPLETA** | Commit `5af3861`, checklist F0 compilata. |
| **FASE 1** | **WeAct Standalone Bring-up:** SWD, flash minimale, clock 170 MHz, USB CDC. | **COMPLETA** | Commit `b39939e`, `e284bd3`, `b9fadb9`. |
| **FASE 2** | **DS3231 Standalone Bring-up:** Comunicazione I2C1 (PA15/PB7) e onda quadra SQW 1 Hz. | **COMPLETA** | Commit `0b45ffa`. |
| **FASE 3** | **XO Standalone Bring-up:** Alimentazione 3,3 V e conteggio fronti 24,576 MHz via TIM2_ETR (PA0). | **COMPLETA** | Commit `d95845e`. |
| **FASE 4** | **PCM1808 C Clock Bring-up:** Clock Master 96 kHz convalidato strumentalmente, CAD KiCad R0.1. | **COMPLETA** | Commit `e857849`. (Modulo B guasto; causa SCONOSCIUTA). |
| **FASE 5** | **PCM1808 → STM32 (I²S2 Slave DMA):** Ricezione hardware I²S2 su PB12/13/15, DMA circolare, collaudo firmware normale F5-A02-R2.1 completato. | **QUALIFICA NOMINALE COMPLETATA** | 3 prove nominali positive (F5-N01-R1 3751 CB, F5-N02-R1 3751 CB, F5-N03 3750 CB; 0 err DMA/OVR; log archiviati in `docs/evidenze_f5_n01_n03/`). **Fase formalmente non chiusa** (requisiti 64-bit e recovery aperti). |
| **GATE H01** | **Ricognizione e Stabilizzazione Hardware:** Ricognizione AS BUILT e fissaggio meccanico cablaggi (prevenzione distacchi Dupont tipo F5-N01). | **APERTO / NEXT** | Pre-requisito tassativo di stabilità prima dell'autorizzazione a FASE 6. |
| **FASE 6** | **RAW Streaming → PC Host:** Trasferimento continuo campioni grezzi su USB e archiviazione loss-less. | **NON INIZIATA — NON AUTORIZZATA** | Bloccata in attesa del superamento di Gate H01 e chiusura formale di FASE 5. |
| **FASE 7** | **Metrologia di Frequenza (Clock Tracking):** Misura continua fronti BCK tra impulsi 1 Hz del DS3231 (TIM2 capture). | **NON INIZIATA** | Subordinata a FASE 6. |
| **FASE 8** | **Stadio Analogico AFE:** Caratterizzazione guadagno, rumore e clipping dell'MCP6022. | **NON INIZIATA** | Subordinata a FASE 7. |
| **FASE 9** | **Piezo + Orologio Meccanico:** Acquisizione del primo segnale reale di ticchettio da orologio. | **NON INIZIATA** | Subordinata a FASE 8. |
| **FASE 10** | **Elaborazione DSP su PC Host:** Algoritmi BPH, Rate, Beat Error, Amplitude e interfaccia grafica. | **NON INIZIATA** | Subordinata a dataset reali convalidati. |

---

## 8. Ultimo Traguardo Verificato ed Evidenze di Laboratorio

In questa sezione si applica la rigorosa tripartizione metodologica tra evidenze strumentali, dichiarazioni dell'operatore e ipotesi tecniche.

### A. Dati Misurati Strumentalmente con Log ed Evidenze Fisiche
Tutti i log originali scambiati nella conversazione sono stati archiviati byte-for-byte in [`docs/evidenze_f5_d03/`](./evidenze_f5_d03/). La disamina dei file individua esattamente **6 run diagnostici distinti**, separati dalle ritrasmissioni periodiche CDC:

1. **Run 1 — `Testo incollato(20261117-133519)(1).txt` (45.557 byte, SHA-256 `4A449FCB...`):**
   - Eseguito con `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`).
   - Contiene 1 frammento iniziale e 10 ritrasmissioni identiche di un unico test anomalo.
   - DMA Total Callbacks: `0` (Half=0, Full=0). Raw IRQ DMA1_Ch1: `0`.
   - Interrupt grezzi e anomalie: `Raw IRQ SPI2 = 103`, `HAL I2S Error CB = 103` (last=0x00000000), `SPI2 OVR contati = 0`.
   - Monitoraggio CNDTR: CNDTR iniziale 2048, minimo 1752, variazioni contate `1`. Poiché `PSIZE=1` (16 bit / half-word) e `MSIZE=1`, il decremento di 296 trasferimenti corrisponde esattamente a 296 campioni a 16 bit, ovvero **592 byte** trasferiti prima dell'arresto del flusso.
   - Indirizzi DMA: `CPAR = 0x4000380C` (**CONFORME**), `CMAR = 0x2000110C` (**CONFORME**).
   - Verifica GPIO IDR (finestra post-stop da 10 ms a 2067 kSps): PB12 0 transizioni (stato rilevato: HIGH); PB13 0 transizioni (stato rilevato: HIGH).
   - *Osservazione fenomenologica:* Nella finestra di test GPIO successiva all'arresto del DMA, entrambi i pin sono stati campionati costantemente a livello logico 1 (nessuna transizione in 10 ms). Ciò documenta che nei 10 ms post-stop i segnali di clock non oscillavano sui pin PB12/PB13 della MCU.

2. **Run 2 — `Testo incollato(20261117-220546).txt` (13.373 byte, SHA-256 `6583F3ED...`):**
   - Contiene 3 ritrasmissioni identiche di un **test completamente regolare**.
   - DMA Total Callbacks: **`3751`** (Half=1876, Full=1875; attese ~3750 a 96 kHz).
   - Interrupt e anomalie: `Raw IRQ DMA1_Ch1 = 3751`, `Raw IRQ SPI2 = 0`, `HAL I2S Error CB = 0`, `SPI2 OVR contati = 0`.
   - Monitoraggio CNDTR: iniziale 2048, minimo 1, massimo 2048, variazioni contate 5001. CPAR e CMAR conformi.
   - Verifica GPIO IDR (1921 kSps):
     - PB12 (LRCK 96 kHz): **1920 transizioni** (HIGH=SI, LOW=SI, init=1, final=1) -> esattamente coerente con 96 kHz.
     - PB13 (BCK 6,144 MHz): **7290 transizioni** (HIGH=SI, LOW=SI).
   - *Esito:* Trasporto I²S2→DMA perfettamente operante a frequenza nominale.

3. **Run 3 — `Testo incollato(20261117-222021).txt` (18.596 byte, SHA-256 `DFC1BEDC...`):**
   - Contiene 1 frammento e 4 ritrasmissioni di un test anomalo con perdita parziale di contatto.
   - DMA Total Callbacks: `1122` (Half=561, Full=561) -> trasferimento parziale interrotto.
   - Interrupt e anomalie: `Raw IRQ DMA1_Ch1 = 1122`, **`Raw IRQ SPI2 = 518.924`**, **`HAL I2S Error CB = 518.924`**, OVR = 0.
   - Monitoraggio CNDTR: iniziale 2048, pre/post-stop 1479, variazioni contate 1500.
   - Verifica GPIO IDR (finestra post-stop da 10 ms a 2067 kSps): PB12 0 transizioni (stato rilevato: LOW); PB13 0 transizioni (stato rilevato: LOW).
   - *Osservazione fenomenologica:* Durante la finestra di streaming DMA si sono verificati 1122 callback con 518.924 interrupt di errore SPI2; nella successiva finestra di test GPIO post-stop di 10 ms, entrambi i pin sono rimasti stabili a livello logico 0 (nessuna transizione osservata).

4. **Run 4 — `Testo incollato(20261117-222542).txt` (17.906 byte, SHA-256 `0A8C70DA...`):**
   - Contiene 4 ritrasmissioni di un test anomalo con flood massivo di interrupt.
   - DMA Total Callbacks: `3714` (Half=1857, Full=1857) -> quasi l'intero volume di campioni trasferito.
   - Interrupt e anomalie: `Raw IRQ DMA1_Ch1 = 3714`, **`Raw IRQ SPI2 = 3.567.483`**, **`HAL I2S Error CB = 3.567.483`**, OVR = 0.
   - Verifica GPIO IDR (finestra post-stop da 10 ms a 1917 kSps): PB12 1921 transizioni (HIGH=SI, LOW=SI); PB13 7552 transizioni (HIGH=SI, LOW=SI).
   - *Osservazione fenomenologica:* Durante i 10 secondi di streaming sono stati completati 3714 callback DMA accompagnati da 3.567.483 interrupt di errore SPI2; nella finestra post-stop di 10 ms, entrambi i pin presentano transizioni logiche coerenti con clock oscillanti (LRCK ~96 kHz, BCK presente). L'evidenza documenta una grave instabilità di linea durante lo streaming, mentre i clock risultavano attivi nella finestra di test successiva.

5. **Run 5A — `Testo incollato(20261117-224318).txt` (Report 1) (14.025 byte, SHA-256 `88F48EDF...`):**
   - **Primo test regolare della sessione conclusiva:**
   - DMA Total Callbacks: **`3750`** (Half=1875, Full=1875).
   - Interrupt e anomalie: `Raw IRQ DMA1_Ch1 = 3750`, `Raw IRQ SPI2 = 0`, `HAL I2S Error CB = 0`, `SPI2 OVR contati = 0`.
   - Monitoraggio CNDTR: iniziale 2048, variazioni contate 5000.
   - Verifica GPIO IDR (1908 kSps): PB12 **1921 transizioni**; PB13 **8152 transizioni**.
   - *Esito:* Test nominale perfetto, zero errori, perfetta rispondenza temporale.

6. **Run 5B — `Testo incollato(20261117-224318).txt` (Report 2 e 3) (SHA-256 `88F48EDF...`):**
   - **Secondo test regolare della sessione conclusiva (con una ritrasmissione identica):**
   - DMA Total Callbacks: **`3751`** (Half=1876, Full=1875).
   - Interrupt e anomalie: `Raw IRQ DMA1_Ch1 = 3751`, `Raw IRQ SPI2 = 0`, `HAL I2S Error CB = 0`, `SPI2 OVR contati = 0`.
   - Monitoraggio CNDTR: iniziale 2048, variazioni contate 5000.
   - Verifica GPIO IDR (1908 kSps): PB12 **1922 transizioni**; PB13 **8139 transizioni**.
   - *Esito:* Secondo test nominale consecutivo perfetto.

### A.1. Collaudo Firmware Normale F5-A02-R2.1 (20 Novembre 2026)
In data 20/11/2026 è stato eseguito il mandato CHAT OVERVIEW per il collaudo formale del firmware normale operativo `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` (F5-A02-R2.1, 43.756 B, SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`).

I tre file di log originali recuperabili scambiati sono archiviati byte-for-byte in [`docs/evidenze_f5_n01_n03/`](./evidenze_f5_n01_n03/) con manifest verificato `MANIFEST_EVIDENZE_F5N01_N03_20261120.txt`:

1. **F5-N01-R1 — Primo Avvio Nominale:**
   - Eseguito a seguito del ripristino di un connettore Dupont scollegato a banco (il collegamento/pin esatto non è stato identificato nella documentazione trasmessa alla OVERVIEW). Per F5-N01 (FAIL originale al primo boot per tale distacco) e F5-N01-R1 non esistono file di log originali recuperabili sul PC; i relativi esiti provengono direttamente dai rapporti operativi trasmessi dall'operatore alla CHAT OVERVIEW.
   - Risultato: **PASS**, 3.751 callback DMA (Half=1876, Full=1875), 0 Error CB, 0 OVR, 0 HAL Error.
2. **F5-N02-R1 — `F5_N02_R1_RESET.txt` (46.725 byte, SHA-256 `17E4C6CF...`):**
   - Eseguito dopo pressione del tasto RESET NRST sulla WeAct Core Board.
   - Durata: 10000 ms.
   - DMA Total Callbacks: **3751** (Half=1876, Full=1875, diff=1).
   - Throughput: 768.204 B/s (nominale 768.000 B/s).
   - Error CB = 0, Overrun(OVR) = 0, HAL Error = 0x00000000, DMA Stalled = NO.
   - Campioni RAW: F0: L=501, R=445; F1: L=277, R=533; F2: L=661, R=549; F3: L=237, R=396.
   - Esito e validazione: **PASS: DMA RX ~96 kHz FUNCTIONAL (rate entro +/-2%)**. Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03 (e nel precedente run F5-N02, `F5_N02_RESET.txt`, 10.552 byte, SHA-256 `AFF52837...`). Il contenuto audio, il formato effettivo dei campioni e il sincronismo completo non sono ancora validati.
3. **F5-N03 — `F5_N03_POWER_CYCLE.txt` (15.828 byte, SHA-256 `CED1DC13...`):**
   - Eseguito dopo disconnessione e riconnessione del cavo USB-C di alimentazione WeAct.
   - Durata: 10000 ms.
   - DMA Total Callbacks: **3750** (Half=1875, Full=1875, bilanciamento perfetto diff=0).
   - Throughput: 768.000 B/s (nominale 768.000 B/s).
   - Error CB = 0, Overrun(OVR) = 0, HAL Error = 0x00000000, DMA Stalled = NO.
   - Esito: **PASS: DMA RX ~96 kHz FUNCTIONAL (rate within +/-2%, audio conversion NOT YET validated)**.

**Nota di metodo sulle ritrasmissioni USB CDC:**
Le molteplici sezioni ripetute nei file (8 in N02, 35 in N02_R1, 12 in N03) sono re-trasmissioni automatiche generate dal firmware ogni 5000 ms nello stato `TEST_STATE_COMPLETED` per consentire l'aggancio del terminale seriale; non rappresentano sessioni di test distinte o ripetute.

### B. Dichiarazioni dell'Operatore Non Accompagnate da Log Archiviati
A seguito delle prove sopra documentate, l'operatore di banco ha eseguito un intervento manuale di riposizionamento dei collegamenti fisici:
1. **Intervento fisico:** Riposizionamento e pressione meccanica complessiva dei contatti breadboard, dei cavetti Dupont e dei morsetti Wago.
2. **Sei riavvii consecutivi (3 Reset NRST + 3 cicli power-cycle USB):**
   - Eseguiti 3 riavvii tramite pressione del pulsante NRST della WeAct.
   - Eseguiti 3 cicli di disconnessione e riconnessione del cavo USB-C.
   - In tutti e 6 i casi, l'operatore ha osservato che la procedura si è conclusa senza accensione fissa del LED blu di errore PC6.
   - **Rettifica fondamentale:** Durante questi 6 riavvii il firmware programmato sulla STM32 era `tickform_phase5_diag.bin` (F5-D03), in quanto nessun re-flash è stato eseguito.
   - **Stato archivistico:** Per questi 6 riavvii non sono stati salvati log seriali individuali. Tale evento costituisce **evidenza osservazionale di ripetibilità del boot e del comportamento del LED**, non certificazione strumentale di 6 acquisizioni da 3750 callback.

### C. Ipotesi e Presunzioni Tecniche Non Verificate
1. **Causa dello stallo iniziale e dei flood di interrupt del 17/11/2026:** È altamente plausibile che la causa fosse un falso contatto su uno o più collegamenti volanti della breadboard (linee clock I²S o resistenze pull-up MD0/MD1). Tuttavia, **il singolo contatto difettoso non è stato isolato con certezza**. La ripresa del funzionamento è avvenuta a seguito della pressione dell'insieme del banco.
2. **Causa del guasto della scheda PCM1808 B:** Registrata formalmente come **TOTALMENTE SCONOSCIUTA**.
3. **Continuità 3.3V (J2.1 ↔ J1.2) su esemplare C:** Classificata come **presunzione di conformità di layout**, non misurata con multimetro prima del test (la misura 0 Ω di FASE 0 appartiene solo agli esemplari #1 e #2).
4. **Qualità del segnale audio reale:** Non ancora verificabile (ingressi analogici J3 aperti all'esterno; AFE non connesso).

---

## 9. Prossima Attività Prevista e Criteri di Accettazione

### 9.1. Prossimi passi formalmente approvati
1. **Collaudo formale del firmware normale operativo F5-A02-R2.1 (GAP-V-01):**
   - **COMPLETATO con esito positivo (20/11/2026):** Programmazione di `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` (SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`). Convalidate 3 acquisizioni nominali da 10s (F5-N01-R1, F5-N02-R1, F5-N03; 3750-3751 CB, 0 errori). Log archiviati in `docs/evidenze_f5_n01_n03/`.
2. **Rilievo e documentazione hardware AS BUILT (GAP-V-02):**
   - **CONFERMATO dall'operatore (20/11/2026):** Masse (J1.3 su GND ST-Link), XO saldato su basetta con C bypass 100 nF, Y-split MD0/MD1 con resistenze da 10 kΩ per ramo, modulo DS3231.
3. **GATE H01 — Ricognizione e Stabilizzazione Hardware (NEXT):**
   - Ispezione e fissaggio meccanico-elettrico di tutti i cavetti jumper Dupont e morsetti Wago a banco prima di procedere a qualsiasi prova continuativa di lunga durata (prevenzione distacchi accidentali come avvenuto in F5-N01).
4. **Valutazione e Chiusura Formale di FASE 5:**
   - La qualifica nominale del firmware normale è completata, ma FASE 5 **non è ancora formalmente chiusa**: restano aperti i requisiti originali di continuous 64-bit sample counter e recovery attiva post-overflow/errori, da trattare formalmente con CHAT OVERVIEW.
5. **FASE 6 (RAW Streaming su PC):**
   - Rimane rigorosamente **NON INIZIATA e NON AUTORIZZATA** fino al superamento formale del Gate H01 e alla chiusura di FASE 5.

---

## 10. Registro delle Decisioni Architetturali (ADR)

| ADR ID | Titolo | Scelta Consolidata | Motivazione Tecnica | Compromesso Accettato |
| :---: | :--- | :--- | :--- | :--- |
| **ADR-001** | ADC PCM1808 a 96 kHz | Texas Instruments PCM1808 in modalità Master 24 bit a 96 kHz ($256 	imes F_s$). | Risoluzione temporale adeguata ($10,4\ \mu	ext{s}$) per isolare i tre urti acustici dello scappamento; chip economico e reperibile. | Consumo energetico non ottimizzato per alimentazione a batteria (accettabile per V0 USB). |
| **ADR-002** | Microcontrollore STM32G431 | WeAct Studio STM32G431CBU6 (ARM Cortex-M4F @ 170 MHz, 128 KB Flash, 32 KB SRAM). | Timer ad alta risoluzione a 32 bit, supporto nativo USB Full-Speed, periferica I2S/SPI avanzata con DMAMUX. | Package QFN saldato; uso di breakout core board per prototipazione rapida. |
| **ADR-003** | Preservazione Forma d'Onda RAW | Nessun filtraggio distruttivo o trigger hardware sulla MCU; streaming continuo su PC. | Permette di ricalcolare e perfezionare gli algoritmi DSP a posteriori senza perdere la dinamica del ticchettio. | Elevato throughput dati via USB (~576 KB/s raw non compresso); necessità di storage su PC host. |
| **ADR-004** | Metrologia BCK vs DS3231 SQW | Conteggio hardware in continuo dei fronti BCK tramite TIM2; cattura ad ogni fronte del segnale 1 Hz dal TCXO DS3231. | Rende la misura della marcia indipendente dalla deriva dell'oscillatore audio locale (XO economico da 24,576 MHz). | Complessità dell'elaborazione di correlazione temporale delegata al software host. |
| **ADR-005** | Nessun Display Locale nella V0 | Esclusione di display LCD/OLED o controlli onboard sul prototipo V0; interfaccia interamente su PC via USB. | Riduzione della complessità hardware, azzeramento del rumore EMI da display e riduzione dei costi. | Dipendenza obbligatoria da un computer host connesso via USB per visualizzare i risultati. |
| **ADR-006** | Oscillatore XO V0 e TCXO V1 | Uso di oscillatore attivo metallico economico (XO 24,576 MHz) corretto da DS3231 nella V0; TCXO nativo nella V1. | Collaudo immediato con componenti reperibili a basso costo, mantenendo la scalabilità metrologica. | Target V0 limitato a ~$\pm 0,5\ 	ext{s/giorno}$; target V1 di $\pm 0,1\ 	ext{s/giorno}$ demandato al PCB definitivo. |

---

## 11. Problemi Risolti, Cause Accertate e Lezioni Apprese

1. **Riconnessione fallita STM32CubeProgrammer CLI (`DEV_CONNECT_ERR` in FASE 1):** Causa: GUI di STM32CubeProgrammer rimasta connessa al target. Risolto disconnettendo la GUI.
2. **Conflitto di pull-down su PB8 / BOOT0 in FASE 2:** Causa: Pull-down onboard da 10 kΩ su PB8 creava partitore a ~2,2 V con il pull-up del modulo. Risolto spostando I2C1 su PA15 e PB7.
3. **Falso contatto su PA0 ad alta frequenza in FASE 3:** Causa: Pin header A0 non saldato dava continuità DC ma 0 fronti a 24,6 MHz. Risolto saldando il pin. *Lezione:* La continuità DC con multimetro non garantisce l'integrità del segnale RF.
4. **Anomalia del modulo PCM1808 B in FASE 4:** Causa del guasto: **TOTALMENTE SCONOSCIUTA e NON DETERMINATA**. Risolto sostituendo con modulo C.
5. **Riferimenti errati a terminali inesistenti nel rapporto F5-A02:** Causa: Riferimenti spuri a pin J2.7/J2.8 (J2 ha solo 6 pin). Risolto correggendo relazioni e librerie KiCad.
6. **Silent Drop USB CDC in F5-D02:** Causa: Buffer `tx_buffer[1536]` rifiutava report da ~2900 byte. Risolto segmentando il report in parti < 1000 byte con pacing di 10 ms.
7. **Inversione etichette I2SE e I2SMOD in F5-D02-R2:** Causa: Bit 10 etichettato come MOD e bit 11 come I2SE. Risolto in F5-D03 (bit 10 = I2SE, bit 11 = I2SMOD).
8. **Stallo hardware del banco durante i primi test di FASE 5 (17/11/2026):** Causa: Mancato contatto elettrico su linee volatili della breadboard (identificato da PB12/13 fissi a HIGH nel run 1 e a LOW nel run 3). Risolto riposizionando e ripremendo i collegamenti fisici a banco. *Nota:* Il singolo contatto specifico non è stato isolato.
9. **Ritrovamento connettore DuPont scollegato durante F5-N01 (20/11/2026):** Causa: distacco accidentale di un connettore DuPont a banco prima del primo run (0 callback). L'operatore ha confermato il ritrovamento di un connettore Dupont scollegato e il successivo ripristino del funzionamento; il collegamento/pin esatto non è stato identificato nella documentazione trasmessa alla OVERVIEW. Risolto: ripristino del contatto a banco; i successivi test F5-N01-R1, F5-N02-R1 e F5-N03 hanno confermato il ripristino del funzionamento (3750–3751 CB, 0 err). *Lezione:* I collegamenti volanti a banco sono vulnerabili e richiedono ricognizione e stabilizzazione formale prima di FASE 6 (Gate H01).

---

## 12. Problemi Aperti, Blocchi Noti e Rischi Tecnici

1. **Gate H01 — Ricognizione e Stabilizzazione Hardware a Banco (PRIORITÀ IMMEDIATA):** Dopo il successo della qualifica nominale del firmware normale F5-A02-R2.1 (F5-N01..N03), la FASE 5 non è ancora formalmente chiusa e la FASE 6 NON è autorizzata. Prima di procedere, è necessario verificare e consolidare tutti i collegamenti a banco (jumper DuPont, alimentazioni, masse, partitori MD0/MD1, fissaggi meccanici) per eliminare la vulnerabilità riscontrata in F5-N01.
2. **Certificazione della conversione audio reale da PCM1808 C:** Non ancora verificata funzionalmente (LIN/RIN lasciati flottanti; AFE non connesso). Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03. Il contenuto audio, il formato effettivo dei campioni e il sincronismo completo non sono ancora validati.
3. **Causa del guasto della scheda PCM1808 B:** Permane non accertata (SCONOSCIUTA).
4. **Circuito di carica DS3231:** Divieto tassativo di inserire batterie CR2032 non ricaricabili per la presenza della rete D2 + R4 (200 Ω).
5. **Vulnerabilità meccanica dei cablaggi jumper:** I collegamenti volanti su breadboard rimangono sensibili a vibrazioni e falsi contatti.
6. **Requisiti firmware residui FASE 5 (Contatore continuo 64-bit e Gestione Overflow/Errori):** Come stabilito dai criteri di accettazione storici (`BRINGUP_CHECKLIST.md` e `RAW_FORMAT_V0_1.md`), sono previsti un contatore continuo a 64 bit dei campioni e una politica attiva di recupero/resincronizzazione da overflow/errori. Attualmente il firmware implementa conteggio callback su finestra temporale e rilevamento/conteggio degli errori OVR/UDR/DMA, ma senza recovery automatico. Tali requisiti rimangono aperti e motivano il mantenimento della FASE 5 non formalmente chiusa.
7. **Requisito futuro di telemetria (Boot ID, Test ID progressivo, Uptime):** La telemetria USB CDC ripete periodicamente l'ultimo report a test completato (ogni 5 s). Per distinguere con certezza nei log le ripetizioni passive dai riavvii hardware o esecuzioni successive, è necessario integrare nei futuri report di telemetria: Boot ID (identificativo univoco del ciclo di accensione), Test ID progressivo (contatore incrementale dell'acquisizione) e Uptime (tempo di accensione in millisecondi o secondi).

---

## 13. Baseline di Riferimento, Versioning e Identificativi Git

### 13.1. Cronologia dei commit Git consolidati
```text
* 7d71430 (HEAD -> main, origin/main) docs: prepare repository for GitHub publication (18/11/2026)
* ee89c29 docs: consolidate documentation R3.1 and evidence (18/11/2026)
* 2cb3de5 chore: complete project safety backup and pre-consolidation tag (18/11/2026)
* e857849 docs(hardware): checkpoint PCM1808 C 96kHz bring-up and KiCad schematic (13/11/2026)
* d95845e feat: validate XO standalone bringup (05/10/2026)
* 0b45ffa feat: validate DS3231 standalone bringup (27/09/2026)
* b39939e feat: complete STM32 standalone bringup (26/09/2026)
* e284bd3 feat: validate STM32 170MHz clock (26/09/2026)
* b9fadb9 feat: validate STM32 minimal firmware (26/09/2026)
* 5af3861 docs: complete receiving inspection (25/09/2026)
* 3e96ee2 docs: record M0 baseline (16/09/2026)
* 97f102b chore: bootstrap Tickform repository (16/09/2026)
```

### 13.2. Cronologia Crittografica delle Programmazioni Firmware STM32
La seguente tabella applica la distinzione fondamentale:  
*Firmware compilato $
eq$ firmware approvato $
eq$ firmware programmato $
eq$ firmware collaudato.*

| Data Evento | Target CMake / Denominazione | File Binario Eseguibile | Dimensione | SHA-256 Ufficiale Verificato | Autorizzazione / Mandato | Evidenza Flash / Programmazione | Esito della Prova Eseguita su Target |
| :---: | :--- | :--- | :---: | :--- | :--- | :--- | :--- |
| **05/10/2026** | `tickform_phase3_xo` (FASE 3) | `firmware/stm32g431/build/phase3/tickform_phase3_xo.bin` | 37.004 B | `E0B8F70661F62EC362DAB0EA53B8E7D3E06B02381FF284CB41865C8C9B1DC4BA` | Mandato FASE 3 | ST-Link CLI log (Gate 3/F3) | **COLLAUDATO CON SUCCESSO:** 16 finestre TIM2_ETR su PA0 convalidate. Baseline preservata. |
| **15/11/2026** | `tickform_phase5_i2s_rx` (F5-A02-R2.1) | `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` | 43.756 B | `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` | Mandato F5-A02-R2 | ST-Link CLI log F5-A02-T01 | **COLLAUDO ANOMALO (0 CB):** Flash eseguito a `0x08000000`, 0 callback in 10s per stallo hardware dei clock a banco. |
| **16/11/2026** | `tickform_phase5_diag` (F5-D02-R2) | `firmware/stm32g431/build/diag/tickform_phase5_diag.bin` (D02) | 47.404 B | `D42E1CBC484CD5D1E0F100302BE18DAEC3571F3CE5E2A1CE9CED0D23AA71A11C` | Mandato F5-D02-R2 | Report F5-D02-R2 | **COLLAUDATO:** CNDTR fermo a 2048, CDC segmentato funzionante. Riscontrata inversione bit etichette. |
| **17/11/2026** | `tickform_phase5_diag` (F5-D03) | `firmware/stm32g431/build/diag/tickform_phase5_diag.bin` (D03) | 52.308 B | `29786361C8299AE5CEFDA480C97EC100A30616624D8029BAE275F164BA3A43B7` | Mandato F5-D03 | ST-Link log F5-D03-T01; 5 file log Tera Term in `docs/evidenze_f5_d03/` | **COLLAUDATO CON SUCCESSO:** Eseguiti 6 run diagnostici. Run 2, 5A, 5B nominali (3750–3751 CB, 0 err). Presente nei 6 reboot successivi. |
| **20/11/2026** | `tickform_phase5_i2s_rx` (F5-A02-R2.1) | `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` | 43.756 B | `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` | Mandato F5-N01/N02/N03 | ST-Link CLI flash; 3 log Tera Term in `docs/evidenze_f5_n01_n03/` | **COLLAUDATO CON SUCCESSO:** Eseguiti F5-N01-R1 (PASS, 3751 CB, 0 err), F5-N02-R1 (PASS post-RESET, 3751 CB), F5-N03 (PASS post-power cycle, 3750 CB, 0 err). Osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03; contenuto audio, formato e sincronismo non ancora validati. Qualifica nominale superata. |

---

## 14. Metrologia e Precisione Temporale

### 14.1. Principio metrologico di TICKFORM (ADR-004)
Un oscillatore economico XO a 24,576 MHz presenta tolleranze tipiche di $\pm 20\ 	ext{ppm} \div \pm 50\ 	ext{ppm}$ e derive termiche (~$0,4\ 	ext{ppm/}^\circ	ext{C}$), corrispondenti a errori potenziali da $\pm 1,7$ a $\pm 4,3\ 	ext{s/giorno}$.  
Per raggiungere il target metrologico di $\le \pm 0,5\ 	ext{s/giorno}$ nella V0 senza impiegare un costoso TCXO audio dedicato, TICKFORM adotta il **tracciamento hardware indipendente**:
1. Il timer a 32 bit `TIM2` della STM32 viene configurato con ingresso di clock esterno pilotato dal bit clock audio `BCK` (6,144 MHz nominali, $64 	imes F_s$).
2. Il modulo RTC Maxim DS3231SN integra un sensore di temperatura e un TCXO compensato a $\pm 2\ 	ext{ppm}$ ($\le 0,17\ 	ext{s/giorno}$ di errore massimo tra 0°C e 40°C).
3. L'uscita a collettore aperto `SQW` del DS3231 viene configurata a **1 Hz** e collegata a un canale di cattura hardware (Input Capture) di `TIM2`.
4. Ad ogni fronte di salita dell'impulso a 1 Hz, il valore del contatore `TIM2` viene congelato in un registro di cattura senza latenza software.
5. La differenza di conteggio tra due secondi consecutivi esprime il numero esatto di cicli di clock BCK generati nel secondo campione:
   $$\Delta N_{	ext{BCK}} = N_{	ext{BCK}}(t) - N_{	ext{BCK}}(t-1)$$
6. La frequenza di campionamento effettiva $F_{s,	ext{eff}}$ dell'ADC viene calcolata esattamente dal PC host come:
   $$F_{s,	ext{eff}} = rac{\Delta N_{	ext{BCK}}}{64}$$
7. Gli algoritmi di calcolo del Rate correggono matematicamente la marcia dell'orologio utilizzando la base dei tempi calibrata al TCXO.

---

## 15. Formato Dati RAW (Draft v0.1)

I file di acquisizione vengono archiviati sul filesystem del PC host come coppie di file con lo stesso nome base:
- `YYYYMMDD_HHMMSS_<identificativo_orologio>.raw`: File binario contenente i campioni audio grezzi non compressi a 96 kHz / 24 bit stereo (4 byte per canale, 8 byte per frame stereo, little-endian).
- `YYYYMMDD_HHMMSS_<identificativo_orologio>.json`: File di testo contenente i metadati metrologici, l'identificativo del banco hardware, i timestamp di inizio e fine, i valori di cattura metrologica $\Delta N_{	ext{BCK}}$ e le note operative dell'utente.

Specifica completa in [`docs/data-format/RAW_FORMAT_V0_1.md`](data-format/RAW_FORMAT_V0_1.md).

---

## 16. Procedura Operativa per Nuove Sessioni di Lavoro / Agenti AI

Quando un nuovo assistente AI o un nuovo ingegnere apre il progetto, deve attenersi alle seguenti regole:
1. **Verificare la cartella locale:** Assicurarsi che la working directory sia `A:\Documenti\Tickform`.
2. **Consultare la memoria centrale:** Leggere prioritariamente `docs/TICKFORM_PROJECT_MEMORY.md` e `docs/TICKFORM_GAP_REGISTER_R3.md`.
3. **Ispezione in sola lettura:** Eseguire `git status` e `git log -n 5` per verificare lo stato di `main` senza mai invocare `git reset`, `git clean` o `git checkout` distruttivi.
4. **Non dare per scontati i file generati:** Verificare sempre gli hash SHA-256 dei binari in `firmware/stm32g431/build/` prima di proporre flash o test.
5. **Rispettare le autorizzazioni:** Nessuna modifica a KiCad, nessun commit Git e nessuna transizione a FASE 6 può essere eseguita senza mandato esplicito di CHAT OVERVIEW.

---

## 17. Regole di Aggiornamento della Memoria Persistente e Protocollo di Sincronizzazione

Per evitare divergenze future, ogni modifica significativa deve seguire il presente protocollo di sincronizzazione:

| Evento Tecnico | Documento Primario da Aggiornare | Azione Richiesta | Documenti Secondari Coinvolti |
| :--- | :--- | :--- | :--- |
| **Nuova decisione architetturale** | `docs/decisions/ADR-xxx.md` | Creare nuovo ADR numerato | Sintesi in `PROJECT_MEMORY` (§10) e nota in `PROJECT_LOG`. |
| **Collaudo firmware eseguito** | `docs/PROJECT_LOG.md` | Nuova voce append-only con data, hash BIN e log | Aggiornare tabella §13.2 in `PROJECT_MEMORY` e Gate in `PROJECT_STATE`. |
| **Modifica cablaggio / hardware** | Scheda AS BUILT in `PROJECT_MEMORY` | Aggiornare matrice §5.1 e nota in `PROJECT_LOG` | Se autorizzato, aggiornare KiCad e generare nuovo review pack. |
| **Chiusura formale di Milestone** | `docs/bringup/BRINGUP_CHECKLIST.md` | Spuntare deliverable con riferimento | Aggiornare `PROJECT_STATE.md` (Current/Next milestone) e `CHAT_HANDOFF.md`. |
| **Commit di consolidamento Git** | Repository Git | Commit atomico con messaggio formale | Aggiornare hash HEAD in `PROJECT_STATE.md` e `PROJECT_MEMORY.md`. |
