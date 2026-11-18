# TICKFORM — COMPREHENSIVE PROJECT HANDOFF
**Documento Unico di Passaggio di Consegne per ChatGPT / Nuovi Agenti AI**  
**Stato documento:** REVISIONE CONSOLIDATA R3.1 (18 Novembre 2026) — Consolidamento Mandato C01  
**Baseline Git:** commit [`e857849debbc54e655a8a87a88c07a2ee0386d43`](https://github.com/Elcosmo/Tickform/commit/e857849debbc54e655a8a87a88c07a2ee0386d43) (`main`, allineato a `origin/main`)  
**Root di Progetto:** `A:\Documenti\Tickform`

---

## Note di Rettifica R3.1 (18/11/2026)
La presente revisione candidata R3.1 recepisce le 5 osservazioni emerse dalla revisione indipendente di CHAT OVERVIEW (`TICKFORM_R3_REVIEW_INDIPENDENTE_20261118.md`):
1. **R3-ERR-01:** Corretto il volume di dati del Run 1: con `PSIZE=1` (16 bit / 2 byte), la variazione di 296 trasferimenti in `CNDTR` equivale a **592 byte** (non 296 byte).
2. **R3-ERR-02:** Descritti i risultati del campionamento GPIO IDR dei Run 1, 3 e 4 come osservazioni fenomenologiche relative alla finestra di test post-stop (10 ms eseguiti *dopo* `HAL_I2S_DMAStop`), senza inferenze indebite sullo stato continuo durante lo streaming.
3. **R3-ERR-03:** Allineamento con `BRINGUP_CHECKLIST.md`: la gestione degli errori/overflow rimane formalmente aperta (`[ ]`), essendo stati convalidati solo il monitoraggio e la diagnostica (F5-D03), ma non l'algoritmo di ripristino automatico a runtime; stabilità DMA provata su corse nominali da 10s.
4. **R3-ERR-04:** Chiarito che `Stop I2S DMA: Status=1 ErrorCode=0x40 StopOK=SI` è il comportamento nativo di ST HAL per Slave RX quando la linea di clock master è ancora attiva all'arresto; `i2s_rx_diag.c` gestisce questo caso verificando i registri e impostando `StopOK=SI`; zero errori rilevati durante lo streaming.
5. **R3-ERR-05:** Verificato su file fisico che `build/phase5/tickform_phase5_i2s_rx.bin` è esattamente di **43.756 byte** con SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`.

## Note di Revisione R3 (18/11/2026)
La presente revisione candidata R3 integra le correzioni definitive scaturite dalla revisione indipendente ChatGPT R2 e dal Mandato R3:
1. **Riconciliazione completa dei log F5-D03:** Censiti e archiviati in [`docs/evidenze_f5_d03/`](./evidenze_f5_d03/) i 5 file Tera Term originali con SHA-256 verificati. Dettagliati i **6 run diagnostici distinti**, separando formalmente le ritrasmissioni periodiche CDC.
2. **Rettifica del firmware dei 6 riavvii:** L'evidenza della sessione attesta che nessun nuovo flash ha avuto luogo tra i test diagnostici e i 6 riavvii (3 reset NRST + 3 cicli power-cycle USB). **Il firmware in esecuzione durante i riavvii era `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`), NON `tickform_phase5_i2s_rx.bin`**. I 6 reboot sono evidenza osservazionale di ripetibilità del boot e del LED, non serie di log con 3750 callback.
3. **Collaudo firmware normale F5-A02-R2.1:** Il binario `tickform_phase5_i2s_rx.bin` (SHA-256 `04B95777...`) è approvato e compilato, ma **NON ancora collaudato a banco con clock operativi**. Costituisce il prossimo gate aperto prima della chiusura formale di FASE 5.
4. **Campionamento GPIO IDR e Teoria Nyquist:** Corretto il range reale a **1908–2067 kSps (~1,9–2,1 MSps)**. PB12 (LRCK 96 kHz) campionato regolarmente (~1920 transizioni in 10 ms); PB13 (BCK 6,144 MHz) **sotto Nyquist**: 0 transizioni su PB13 non provano assenza di clock per effetto di aliasing. Il test IDR avviene *dopo* l'arresto DMA.
5. **CAD KiCad R0.1 vs Banco AS BUILT:** KiCad R0.1 (due resistenze 10 kΩ R1/R2) è confermato integro e approvato per FASE 4. Sul banco fisico AS BUILT il livello HIGH è confermato, ma il valore resistivo e la topologia a Y non sono stati misurati con multimetro.
6. **Portabilità Markdown:** Tutti i collegamenti interni sono espressi come percorsi relativi rispetto a `docs/`.
7. **FASE 6:** Confermato il blocco tassativo: FASE 6 è **NON INIZIATA e NON ANCORA AUTORIZZATA**.

---

## 1. Executive Summary del Progetto

**TICKFORM** è un progetto di ingegneria embedded e metrologica per la realizzazione di un **timegrapher avanzato per orologi meccanici**, concepito per la pubblicazione open-source e open-hardware.

A differenza degli strumenti commerciali di fascia economica (che riducono il segnale a impulsi tramite comparatori a soglia fissa), TICKFORM preserva integralmente la forma d'onda acustica grezza (**RAW waveform a 96 kHz / 24 bit stereo**) del ticchettio prodotto dallo scappamento ad ancora dell'orologio.

### Catena del segnale e metrologia
1. **Trasduzione:** Un disco piezoelettrico passivo da 27 mm rileva le vibrazioni meccaniche dell'orologio (urti di sblocco, impulso e caduta dell'ancora).
2. **Condizionamento (AFE):** Un doppio operazionale a basso rumore Microchip MCP6022-I/P amplifica il segnale ad alta impedenza con riferimento virtuale $V_{\text{REF}} \approx 2,5\ \text{V}$.
3. **Conversione A/D:** L'ADC Texas Instruments PCM1808 (modulo C), pilotato da un oscillatore al quarzo dedicato (XO) a 24,576 MHz in modalità Master (256 × $F_s$), genera i clock audio seriali ed esegue il campionamento stereo a $F_s = 96\ \text{kHz}$ nominali ($BCK = 6,144\ \text{MHz}$).
4. **Ricezione ed elaborazione embedded:** Una scheda WeAct Studio con microcontrollore STM32G431CBU6 (ARM Cortex-M4F @ 170 MHz) riceve i dati I²S2 in modalità Slave Receiver e li trasferisce in memoria SRAM tramite DMA circolare (DMA1 Channel 1 / DMAMUX1 req ID 12).
5. **Metrologia indipendente:** Per correggere la tolleranza e la deriva termica dell'XO economico (target V0 $\approx \pm 0,5\ \text{s/giorno}$), il timer hardware `TIM2` a 32 bit conta continuamente i fronti del bit clock audio `BCK` ed esegue la cattura hardware ad ogni fronte del segnale ad onda quadra a 1 Hz (`SQW`) generato dal modulo RTC/TCXO ad alta stabilità Maxim DS3231SN ($\pm 2\ \text{ppm}$).
6. **Streaming ed elaborazione PC:** I dati vengono trasmessi via USB nativa al PC host, dove un software dedicato archivierà i file `.raw` + `.json`, calcolando BPH, Rate (marcia giornaliera in s/d), Beat Error (errore di battuta in ms) e Amplitude (ampiezza del bilanciere in gradi), tracciando il nastro grafico del timegrapher.

---

## 2. Mappa delle Directory e dei Documenti Canonici

```text
A:\Documenti\Tickform\
├── .git/                                [Git locale, branch main, HEAD e857849]
├── README.md                            [Visione generale, indice documentale]
├── LICENSE                              [Licenza MIT placeholder]
├── .gitignore                           [Esclusione build/, raw generici]
├── bom/
│   ├── V0.0_AS_PURCHASED.md             [BOM d'acquisto: tutti i lotti RECEIVED]
│   └── cost_tracking.csv                [Tracciamento costi: ~131 € totali]
├── docs/
│   ├── TICKFORM_PROJECT_MEMORY.md       [MEMORIA CENTRALE: testo tecnico omnicomprensivo R3]
│   ├── TICKFORM_FULL_PROJECT_HANDOFF.md [Il presente documento di handoff R3]
│   ├── TICKFORM_DOCUMENTATION_AUDIT.md  [Audit documentale e inventario R3]
│   ├── TICKFORM_GAP_REGISTER_R3.md      [Registro univoco dei disallineamenti D, V, A]
│   ├── TABELLA_CORREZIONI_R3.md         [Matrice comparativa correzioni R2 vs R3]
│   ├── TICKFORM_R3_CHANGELOG.md         [Changelog di dettaglio della revisione R3]
│   ├── RAPPORTO_VERIFICA_GIT_R3.md      [Rapporto di verifica Git non distruttivo]
│   ├── RAPPORTO_VERIFICA_KICAD_R3.md    [Rapporto di verifica KiCad e matrice hardware]
│   ├── PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md [Bozza proposta per i 4 documenti canonici]
│   ├── evidenze_f5_d03/                 [I 5 log Tera Term originali + manifest SHA-256]
│   ├── PROJECT_STATE.md                 [Stato vivente: proposta di aggiornamento pronta]
│   ├── PROJECT_LOG.md                   [Diario cronologico immutabile: append-only pronto]
│   ├── CHAT_HANDOFF.md                  [Sintesi operativa chat: proposta pronta]
│   ├── decisions/                       [ADR-001 .. ADR-006 accettati]
│   └── bringup/BRINGUP_CHECKLIST.md     [Checklist prove: proposta di aggiornamento pronta]
├── hardware/v0_perfboard/schematic/phase4_pcm1808c/
│   ├── tickform_phase4_pcm1808c.kicad_pro [KiCad 10: Progetto integro]
│   ├── tickform_phase4_pcm1808c.kicad_sch [Foglio schema R0.1 approvato (FASE 4)]
│   ├── tickform_symbols.kicad_sym       [Libreria simboli autonoma]
│   └── erc_report.rpt                   [Report ERC: 0 Errors, 0 Warnings]
└── firmware/stm32g431/
    ├── CMakeLists.txt                   [3 target: phase3_xo, phase5_i2s_rx, phase5_diag]
    ├── build.ps1                        [Script build unificato con target switch]
    ├── Core/Src/i2s_rx.c / i2s_rx.h     [Modulo normale operativo FASE 5]
    ├── Core/Src/i2s_rx_diag.c / .h      [Modulo diagnostica interna F5-D03]
    └── build/                           [Output binari compilati e validati crittograficamente]
```

---

## 3. Stato Hardware AS BUILT vs Schema KiCad R0.1

### 3.1. Schema KiCad R0.1 (Approvato per FASE 4)
Lo schema risiede in [`hardware/v0_perfboard/schematic/phase4_pcm1808c/`](../hardware/v0_perfboard/schematic/phase4_pcm1808c/):
- **Stato:** Approvato da CHAT OVERVIEW (13/11/2026), ERC **0 Errors, 0 Warnings**, verificato integro su Git (SHA-256 `16AB73A3...`).
- **Limiti di perimetro:** Documenta esclusivamente la generazione dei clock a 96 kHz sul modulo **PCM1808 C**. **I pin I²S (PB12, PB13, PB15) e il modulo DS3231 NON sono collegati nello schema.** Le uscite clock LRC e BCK sono attestate solo sulle sonde dell'analizzatore logico (`INSTR1`).
- **Resistenze pull-up:** Disegnate come due resistenze distinte da 10 kΩ (`R1` e `R2`) verso 3,3 V.

### 3.2. Prototipo Fisico Reale AS BUILT (Banco Sperimentale F5-D03)
- WeAct alimentata via cavo USB-C dal PC (rail 3,3 V e V+ 5,0 V attivi).
- Oscillatore XO 24,576 MHz alimentato a 3,3 V con condensatore di bypass ceramico da 100 nF. Uscita OUT collegata a `SCK` del PCM1808 C con filo diretto (R_onboard ~100 Ω accertata passivamente).
- Modulo PCM1808 C alimentato a 5 V analogici (WeAct V+) e 3,3 V digitali (WeAct 3V3).
- **MD0 e MD1:** Collegati a 3,3 V tramite derivazione a Y; **valore resistivo e topologia NON misurati con multimetro** prima del test; livello logico HIGH confermato dall'avvio della modalità Master a 96 kHz.
- **Collegamenti I²S verso STM32:** Tre cavetti volanti Dupont maschio-femmina su breadboard collegano `LRC (J2.4)` a `PB12`, `BCK (J2.6)` a `PB13`, `OUT (J2.5)` a `PB15`.
- **Modulo DS3231:** Connesso su breadboard (I2C1 su PA15/PB7, SQW 1 Hz); batteria non installata.

---

## 4. Architettura Firmware e Sottosistemi STM32

### 4.1. Configurazione I²S2 Slave Receiver e DMA
- **Target operativo normale:** `tickform_phase5_i2s_rx` ([`firmware/stm32g431/Core/Src/i2s_rx.c`](../firmware/stm32g431/Core/Src/i2s_rx.c)).
- **Configurazione I²S2:** Slave Receiver, Philips I²S 24-bit su frame 32-bit, clock polarity LOW.
- **DMA:** DMA1 Channel 1 in modalità circolare, DMAMUX1 Request ID 12 (`SPI2_RX`). Dimensione trasferimento Halfword (16-bit), incremento memoria abilitato.
- **Buffer circolare:** 2048 halfword (1024 frame stereo = 4096 byte). Ad ogni metà buffer (Half Transfer) e fine buffer (Transfer Complete) viene generata una callback HAL. A 96 kHz stereo, si registrano **375 callback/secondo** (~3750 callback in 10 secondi).
- **Stato del binario:** Compilato e approvato crittograficamente (`firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin`, 43.756 byte, SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`). **Collaudo formale a banco con clock attivi pendente.**

### 4.2. Firmware Diagnostico F5-D03
- **Target diagnostico:** `tickform_phase5_diag` ([`firmware/stm32g431/Core/Src/i2s_rx_diag.c`](../firmware/stm32g431/Core/Src/i2s_rx_diag.c)).
- **Caratteristiche:**
  - Acquisizione di 10 secondi con monitor CNDTR a 1 kHz.
  - Verifica degli indirizzi hardware CPAR (`0x4000380C`) e CMAR (`0x2000110C`).
  - Snapshot dei registri all'avvio (INIT), a metà prova (MID), prima dello stop (PRE-STOP) e dopo lo stop (POST-STOP).
  - Test digitale ingressi GPIO IDR (`run_pin_input_test()`) eseguito nei 10 ms immediatamente successivi all'arresto del DMA.
  - Stampa CDC segmentata in 6 parti con pacing temporale per eliminare il problema del silent drop USB.
- **Stato del binario:** Programmato ed eseguito a banco il 17/11/2026 (`firmware/stm32g431/build/diag/tickform_phase5_diag.bin`, 52.308 byte, SHA-256 `29786361C8299AE5CEFDA480C97EC100A30616624D8029BAE275F164BA3A43B7`).

### 4.3. Benchmark e Limiti di Campionamento GPIO IDR
- **Frequenza reale misurata su target:** **1908–2067 kSps (~1,9–2,1 MSps)**.
- **PB12 (LRCK 96 kHz):** Campionato circa 10 volte per semiperiodo; nei test regolari ha rilevato esattamente **1920–1922 transizioni in 10 ms**, provando l'attività del clock a 96 kHz.
- **PB13 (BCK 6,144 MHz):** Sotto frequenza di Nyquist; risente di aliasing. Transizioni > 0 indicano toggle delle linee; **0 transizioni non provano l'assenza fisica del clock**.

---

## 5. Riconciliazione Integrale delle Evidenze Sperimentali (F5-D03)

I 5 file di log Tera Term archiviati in [`docs/evidenze_f5_d03/`](./evidenze_f5_d03/) documentano inequivocabilmente **6 run diagnostici distinti**:

| Run # | File Sorgente | SHA-256 File | DMA Total CB | Raw IRQ SPI2 | HAL Err CB | CNDTR Min/Fin | GPIO PB12 (LRCK) | GPIO PB13 (BCK) | Esito di Laboratorio |
| :---: | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **Run 1** | `Testo incollato(20261117-133519)(1).txt` | `4A449FCB...` | 0 | 103 | 103 | 1752 / 1752 | 0 trans (HIGH) | 0 trans (HIGH) | **Anomalo:** Stallo hardware; CNDTR decrementa di 296 trasferimenti (592 byte) poi si blocca. Finestra post-stop: PB12/PB13 stabili ad HIGH. |
| **Run 2** | `Testo incollato(20261117-220546).txt` | `6583F3ED...` | **3751** | 0 | 0 | 1 / 668 | 1920 trans | 7290 trans | **Regolare:** Trasferimento continuo nominale; 0 errori; clock 96 kHz perfetto. |
| **Run 3** | `Testo incollato(20261117-222021).txt` | `DFC1BEDC...` | 1122 | 518.924 | 518.924 | 1 / 1479 | 0 trans (LOW) | 0 trans (LOW) | **Anomalo:** Trasferimento interrotto dopo 1122 CB; 518k IRQ errore SPI2. Finestra post-stop: PB12/PB13 stabili a LOW. |
| **Run 4** | `Testo incollato(20261117-222542).txt` | `0A8C70DA...` | 3714 | 3.567.483 | 3.567.483 | 1 / 1853 | 1921 trans | 7552 trans | **Anomalo:** DMA quasi completo (3714 CB) ma flood di 3,56M IRQ errore SPI2. Finestra post-stop: clock presenti e oscillanti. |
| **Run 5A**| `Testo incollato(20261117-224318).txt` (Rep 1) | `88F48EDF...` | **3750** | 0 | 0 | 1 / 1462 | 1921 trans | 8152 trans | **Regolare:** Esattamente 3750 CB; 0 errori; clock 96 kHz nominali. |
| **Run 5B**| `Testo incollato(20261117-224318).txt` (Rep 2-3)| `88F48EDF...` | **3751** | 0 | 0 | 1 / 391 | 1922 trans | 8139 trans | **Regolare:** 3751 CB; 0 errori; clock 96 kHz nominali. (Rep 3 identica per ritrasmissione CDC). |
| **Reboot**| Dichiarazione operatore (nessun log) | N/A | Non mis. | Non mis. | Non mis. | Non mis. | Non mis. | Non mis. | **Osservazionale:** 3 reset NRST + 3 cicli USB senza LED blu acceso. Firmware: `tickform_phase5_diag`. |

### Analisi dell'Intermittenza
La transizione tra i test anomali (Run 1, 3, 4) e i test regolari (Run 2, 5A, 5B) è avvenuta a seguito dell'intervento meccanico dell'operatore, che ha riposizionato e ripremuto l'insieme dei cavetti Dupont e dei morsetti sulla breadboard.  
**Il singolo contatto difettoso non è stato isolato con certezza.** Il cablaggio intermittente su linee di clock o pull-up costituisce un'ipotesi tecnica altamente plausibile e supportata dalla correlazione, non una diagnosi certa.

### Riconciliazione dello Stop DMA (Status=1, ErrorCode=0x40, StopOK=SI)
In tutti i report F5-D03 (compresi i run nominali Run 2, 5A e 5B) compare la riga:
`Stop I2S DMA: Status=1 ErrorCode=0x00000040 StopOK=SI`
- **Comportamento nativo ST HAL (C01-B-ERR-01):** In modalità `I2S_MODE_SLAVE_RX`, `HAL_I2S_DMAStop` (`stm32g4xx_hal_i2s.c:1450-1456`) imposta `HAL_I2S_ERROR_BUSY_LINE_RX` (`0x40`) e ritorna `HAL_ERROR` (`1`) incondizionatamente in quel ramo, senza verificare esplicitamente il flag BSY. È l'esito predeterminato di quel ramo del driver ST HAL, non una misura del clock esterno né un'anomalia di ricezione.
- **Criterio software StopOK (C01-B-ERR-02):** Il modulo diagnostico (`i2s_rx_diag.c:704-712`) convalida `StopOK=SI` controllando direttamente la disabilitazione del canale DMA (`DMA1_Channel1->CCR.EN == 0`) e della richiesta RXDMAEN (`SPI2->CR2.RXDMAEN == 0`). Altre letture registri sono evidenze diagnostiche post-stop separate.
- **Assenza di errori durante lo streaming:** Durante l'intera finestra di streaming di 10 secondi non vi è stato alcun errore (`Raw IRQ SPI2 = 0`, `HAL Err CB = 0`, `OVR = 0`).

---

## 6. Cronologia Crittografica dei Firmware STM32

*Principio vincolante: Firmware compilato $
eq$ approvato $
eq$ programmato $
eq$ collaudato.*

| Target Firmware | File Binario | SHA-256 Ufficiale | Dimensione | Stato di Validazione | Note Operative |
| :--- | :--- | :--- | :---: | :--- | :--- |
| `tickform_phase3_xo` | `build/phase3/tickform_phase3_xo.bin` | `E0B8F70661F62EC362DAB0EA53B8E7D3E06B02381FF284CB41865C8C9B1DC4BA` | 37.004 B | **COLLAUDATO CON SUCCESSO** | Baseline FASE 3 (contatore XO PA0/TIM2_ETR). |
| `tickform_phase5_i2s_rx` | `build/phase5/tickform_phase5_i2s_rx.bin` | `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` | 43.756 B | **APPROVATO / COLLAUDO PENDENTE** | Modulo normale F5-A02-R2.1. Flash fallito in F5-A02-T01 (0 CB per HW); non ritestato. |
| `tickform_phase5_diag` (D02) | `build/diag/tickform_phase5_diag.bin` | `D42E1CBC484CD5D1E0F100302BE18DAEC3571F3CE5E2A1CE9CED0D23AA71A11C` | 47.404 B | **COLLAUDATO (Storico)** | Diagnostica F5-D02-R2. CNDTR 2048 fisso, CDC segmentato ok. |
| `tickform_phase5_diag` (D03) | `build/diag/tickform_phase5_diag.bin` | `29786361C8299AE5CEFDA480C97EC100A30616624D8029BAE275F164BA3A43B7` | 52.308 B | **COLLAUDATO CON SUCCESSO** | Diagnostica F5-D03. Eseguito a banco il 17/11/2026 (Run 1..5B e 6 reboot). |

---

## 7. Stato del Repository Git

- **Root locale:** `A:\Documenti\Tickform`
- **Branch attivo:** `main` (allineato a `origin/main` su `https://github.com/Elcosmo/Tickform.git`).
- **Commit HEAD:** `e857849debbc54e655a8a87a88c07a2ee0386d43` (docs(hardware): checkpoint PCM1808 C 96kHz bring-up and KiCad schematic, 13/11/2026).
- **Working Tree:**
  - 4 file tracciati modificati: `CMakeLists.txt`, `stm32g4xx_hal_conf.h`, `main.c`, `build.ps1`.
  - Sorgenti I²S untracked protetti nel filesystem locale: `i2s_rx.c`, `i2s_rx.h`, `i2s_rx_diag.c`, `i2s_rx_diag.h`.
  - Documentazione candidata R3 e pacchetti di review untracked.
- **Divieti operativi:** Nessun commit, push, reset o rebase eseguito.

---

## 8. Prossime Attività e Decisioni Aperte per CHAT OVERVIEW

Per procedere formalmente dopo la consegna del pacchetto R3, CHAT OVERVIEW dovrà valutare:
1. **Mandato Operativo di Collaudo F5-A02-R2.1 (GAP-V-01 / GAP-A-01):** Autorizzare il flash di `tickform_phase5_i2s_rx.bin` (SHA-256 `04B95777...`) ed eseguire il collaudo di ricezione DMA di 10 secondi sul banco funzionante, salvando il log ufficiale.
2. **Documentazione AS BUILT (GAP-V-02):** Fotografare il banco e misurare le resistenze di pull-up MD0/MD1 con multimetro prima di qualsiasi saldatura.
3. **Approvazione della Sincronizzazione Documenti Canonici (GAP-A-04):** Autorizzare l'applicazione delle modifiche proposte in `docs/PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md` a `PROJECT_STATE.md`, `PROJECT_LOG.md`, `CHAT_HANDOFF.md` e `BRINGUP_CHECKLIST.md`.
4. **FASE 6 (RAW Streaming su PC):** Rimane rigorosamente **NON INIZIATA e NON AUTORIZZATA** fino alla chiusura formale di FASE 5.
