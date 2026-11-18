# TICKFORM — AUDIT INTEGRALE DELLA DOCUMENTAZIONE
**Inventario, Analisi di Coerenza e Raccomandazioni di Governance**  
**Stato documento:** REVISIONE CONSOLIDATA R3.1 (18 Novembre 2026) — Consolidamento Mandato C01  
**Revisore:** Revisore Tecnico Indipendente e Archivista di Progetto  
**Ambito:** Repository `A:\Documenti\Tickform`

---

## Note di Rettifica R3.1 (18/11/2026)
La presente revisione candidata R3.1 formalizza le 5 rettifiche definitive scaturite dalla revisione indipendente CHAT OVERVIEW (`TICKFORM_R3_REVIEW_INDIPENDENTE_20261118.md`):
1. **R3-ERR-01 (Unità di misura DMA Run 1):** Riconciliato il decremento CNDTR nel Run 1: 296 trasferimenti con `PSIZE=1` corrispondono esattamente a **592 byte** (non 296 byte).
2. **R3-ERR-02 (Fenomenologia GPIO IDR post-stop):** I risultati dei campionamenti PB12/PB13 nei Run 1, 3 e 4 sono descritti in termini rigorosamente fenomenologici riferiti alla specifica finestra post-stop (10 ms dopo `HAL_I2S_DMAStop`).
3. **R3-ERR-03 (Allineamento Checklist ed Error Handling):** Chiarito che l'assenza di interrupt nei test nominali prova la stabilità del trasporto dati e il monitoraggio errori, ma la voce per il recupero automatico runtime post-anomalia rimane aperta (`[ ]`).
4. **R3-ERR-04 (Riconciliazione Stop DMA Status=1 / ErrorCode=0x40 / StopOK=SI):** Documentato che `Status=1` ed `ErrorCode=0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) sono generati sistematicamente dal driver ST HAL in Slave RX quando il clock esterno è attivo all'arresto; il firmware diagnostico valida lo stop via registri e durante lo streaming non vi è stato alcun errore.
5. **R3-ERR-05 (Conferma dimensione binario normale):** Verificato su file fisico che `build/phase5/tickform_phase5_i2s_rx.bin` è esattamente di **43.756 byte** con SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` (il 43.748 B era refuso in `RAPPORTO_F5_D03.md`).

## Note di Revisione R3 (18/11/2026)
La presente revisione candidata R3 integra le verifiche e le riconciliazioni scaturite dal Mandato R3 e dalla revisione indipendente ChatGPT R2:
1. **Censimento completo delle evidenze F5-D03:** Registrata formalmente la cartella [`docs/evidenze_f5_d03/`](./evidenze_f5_d03/) contenente i 5 file di log Tera Term originali scambiati nella conversazione e il relativo manifest SHA-256. Censiti i 6 run diagnostici distinti.
2. **Rettifica della cronologia firmware:** Riconciliata la cronologia delle programmazioni della STM32. È formalmente attestato che durante i 6 riavvii successivi ai test il firmware in esecuzione era `tickform_phase5_diag.bin` (F5-D03), e che il firmware normale operativo `tickform_phase5_i2s_rx.bin` (F5-A02-R2.1) è approvato ma in attesa di collaudo formale con clock attivi.
3. **Integrazione del Registro Univoco dei Disallineamenti:** Inserito il riferimento a [`docs/TICKFORM_GAP_REGISTER_R3.md`](./TICKFORM_GAP_REGISTER_R3.md), che classifica e traccia ogni discrepanza nelle categorie D (Documentale), V (Verifica necessaria) e A (Approvazione necessaria).
4. **Armonizzazione e Sincronizzazione dei Documenti Canonici:** Verificata la persistenza storica dei quattro documenti canonici (`PROJECT_STATE.md`, `PROJECT_LOG.md`, `CHAT_HANDOFF.md`, `BRINGUP_CHECKLIST.md`) e predisposto il documento [`docs/PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md`](./PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md) per consentire il loro aggiornamento non distruttivo previa autorizzazione di CHAT OVERVIEW.
5. **Portabilità e Coerenza dei Collegamenti:** Normalizzati tutti i percorsi interni in formato Markdown relativo.

---

## 1. Inventario e Classificazione dei File Documentali

| File / Percorso Relativo | Tipologia / Formato | Classificazione | Stato di Salute / Allineamento |
| :--- | :--- | :--- | :--- |
| `README.md` | Markdown | **CANONICO** | **Buono.** Presenta scopi generali, visione di sistema e perimetro iniziale. |
| `LICENSE` | Testo | **CANONICO** | **Placeholder.** Licenza MIT generica; formalizzazione definitiva demandata a rilascio V1. |
| `.gitignore` | Configurazione | **CANONICO** | **Eccellente.** Esclude build/, raw generici e temporanei; preserva data/examples/. |
| `bom/V0.0_AS_PURCHASED.md` | Markdown | **CANONICO** | **Eccellente.** 29 voci nello stato RECEIVED, chiara distinzione AS PURCHASED vs AS BUILT. |
| `bom/V0.0_AS_PURCHASED.csv` | CSV | **CANONICO** | **Eccellente.** Pienamente allineato alla tabella Markdown. |
| `bom/cost_tracking.csv` | CSV | **CANONICO** | **Buono.** Tracciamento chiaro dei costi (~131 € totali sostenuti). |
| `docs/r3_1_candidate/TICKFORM_PROJECT_MEMORY.md` | Markdown | **CANONICO (Candidato R3.1)** | **Eccellente.** Memoria centrale persistente aggiornata alle prescrizioni R3.1. |
| `docs/r3_1_candidate/TICKFORM_FULL_PROJECT_HANDOFF.md`| Markdown | **CANONICO (Candidato R3.1)** | **Eccellente.** Handoff completo per ChatGPT / nuovi agenti aggiornato a R3.1. |
| `docs/r3_1_candidate/TICKFORM_DOCUMENTATION_AUDIT.md` | Markdown | **CANONICO (Candidato R3.1)** | **Eccellente.** Il presente documento di audit aggiornato alla R3.1. |
| `docs/r3_1_candidate/TICKFORM_GAP_REGISTER_R3_1.md` | Markdown | **OPERATIVO (R3.1)** | **Aggiornato.** Registro univoco dei disallineamenti classificati in D, V, A. |
| `docs/r3_1_candidate/TABELLA_CORREZIONI_R3_1.md` | Markdown | **OPERATIVO (R3.1)** | **Nuovo.** Tabella comparativa di chiusura delle 5 osservazioni R3-ERR-01..05. |
| `docs/r3_1_candidate/TICKFORM_R3_1_CHANGELOG.md` | Markdown | **OPERATIVO (R3.1)** | **Nuovo.** Changelog di dettaglio per la transizione da R3 a R3.1. |
| `docs/r3_1_candidate/RAPPORTO_VERIFICHE_MIRATE_R3_1.md` | Markdown | **OPERATIVO (R3.1)** | **Nuovo.** Verifiche mirate su codice C HAL/I2S, binario normale ed evidenze. |
| `docs/r3_1_candidate/PIANO_PROTEZIONE_GIT_R3_1.md` | Markdown | **OPERATIVO (R3.1)** | **Nuovo.** Piano a 3 livelli di conservazione e commit atomico per file non tracciati. |
| `docs/r3_1_candidate/PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3_1.md`| Markdown | **OPERATIVO (R3.1)** | **Aggiornato.** Proposta formale non distruttiva per aggiornare i 4 canonici. |
| `docs/evidenze_f5_d03/` | Directory / Testo | **EVIDENZE FISICHE** | **Eccellente.** Contiene i 5 log Tera Term F5-D03 e il manifest SHA-256. |
| `docs/PROJECT_LOG.md` | Markdown | **CANONICO (Append-only)**| **Fermo al 13/11/2026.** Manca la sezione FASE 5 (proposta append-only pronta in R3). |
| `docs/PROJECT_STATE.md` | Markdown | **CANONICO (Stato Vivente)**| **Fermo al 13/11/2026.** Riporta FASE 5 come "Next" (proposta pronta in R3). |
| `docs/CHAT_HANDOFF.md` | Markdown | **CANONICO (Sintesi Chat)** | **Fermo al 13/11/2026.** Superato dal nuovo handoff R3 (proposta pronta in R3). |
| `docs/bringup/BRINGUP_CHECKLIST.md` | Markdown | **CANONICO (Checklist)** | **Fermo al 13/11/2026.** FASE 5 non spuntata (proposta pronta in R3). |
| `docs/decisions/ADR-001 .. ADR-006` | Markdown | **CANONICI** | **Eccellenti.** 6 decisioni architetturali immutate e pienamente applicate. |

---

## 2. Analisi di Coerenza e Riconciliazione delle Evidenze

### 2.1. Tracciabilità crittografica e isolamento delle varianti firmware
L'audit ha accertato che i tre target CMake definiti in `firmware/stm32g431/CMakeLists.txt` compilano artefatti binari indipendenti con hash SHA-256 univoci e verificati:
- **FASE 3 (Baseline):** `build/phase3/tickform_phase3_xo.bin` (SHA-256 `E0B8F706...`, 37.004 B) -> Convalidato.
- **FASE 5 (Normale operativo):** `build/phase5/tickform_phase5_i2s_rx.bin` (SHA-256 `04B95777...`, 43.756 B) -> Approvato, collaudo a banco PENDENTE.
- **FASE 5 (Diagnostica F5-D03):** `build/diag/tickform_phase5_diag.bin` (SHA-256 `29786361...`, 52.308 B) -> Convalidato con successo il 17/11/2026.

### 2.2. Integrità dello Schema CAD KiCad R0.1
Lo schema `hardware/v0_perfboard/schematic/phase4_pcm1808c/tickform_phase4_pcm1808c.kicad_sch` è risultato **completamente conforme e integro** (ERC: 0 errori, 0 avvisi).  
Si evidenzia che lo schema R0.1 documenta fedelmente la FASE 4 (clock 96 kHz su PCM1808 C); i collegamenti I²S su PB12/13/15 e il modulo DS3231 appartengono al banco sperimentale breadboard e verranno recepiti formalmente nella futura Revisione R0.2 solo previa autorizzazione della supervisione.

### 2.3. Riconciliazione delle Prove Sperimentali F5-D03
L'esame dei 5 log originali archiviati in `docs/evidenze_f5_d03/` ha permesso di separare nettamente le ritrasmissioni periodiche CDC dai test distinti, evidenziando esattamente **6 run diagnostici**:
- **Run negativi / anomali (Run 1, 3, 4):** Documentano stalli hardware e flood di interrupt SPI2 dovuti a contatti volanti su breadboard. Nel Run 1 il DMA si ferma dopo 296 trasferimenti (592 byte) con pin PB12/PB13 stabili ad HIGH nella finestra post-stop; nel Run 3 lo streaming si interrompe dopo 1122 CB con 518k IRQ errore e pin stabili a LOW post-stop; nel Run 4 lo streaming completa 3714 CB con 3,56M IRQ errore e clock oscillanti post-stop.
- **Run positivi nominali (Run 2, 5A, 5B):** Documentano il funzionamento impeccabile della ricezione DMA: esattamente 3.750–3.751 callback in 10s, zero interrupt SPI2, zero errori HAL, zero overrun, e clock LRCK a 96 kHz perfettamente rilevato (1920–1922 transizioni in 10 ms).
- **Riconciliazione dello Stop DMA (Status=1, ErrorCode=0x40, StopOK=SI):** Documentato che `ErrorCode=0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) all'arresto è impostato incondizionatamente dal driver ST HAL nel ramo Slave RX di `HAL_I2S_DMAStop` (C01-B-ERR-01); il firmware valida `StopOK=SI` verificando direttamente che il canale DMA e RXDMAEN siano spenti (C01-B-ERR-02); zero errori durante i 10s di streaming.
- **Sei riavvii successivi:** Attribuiti formalmente al firmware diagnostico `tickform_phase5_diag.bin` (in assenza di riprogrammazioni); riclassificati come evidenza osservazionale di ripetibilità del boot e del LED, non come serie di log archiviati.

---

## 3. Piano di Azione e Governance Documentale

1. **Stato della Documentazione R3.1:**
   - La suite di documenti candidati R3.1 posizionata in `docs/r3_1_candidate/` recepisce puntualmente le osservazioni R3-ERR-01..05 ed è pronta per l'approvazione finale da parte di CHAT OVERVIEW.
   - È stato predisposto il piano di salvaguardia `docs/r3_1_candidate/PIANO_PROTEZIONE_GIT_R3_1.md` per garantire la conservazione a tre livelli dei file sorgente ed evidenze non ancora tracciati in Git.
2. **Aggiornamento dei Documenti Canonici:**
   - I 4 documenti canonici storici non sono stati modificati nel filesystem locale. Non appena CHAT OVERVIEW avrà approvato `docs/PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md`, le modifiche potranno essere applicate in un unico passaggio controllato.
3. **Collaudo del Firmware Normale (GAP-V-01):**
   - L'attività tecnica prioritaria consiste nel programmare ed eseguire a banco il firmware normale operativo `tickform_phase5_i2s_rx.bin` (F5-A02-R2.1) con clock attivi, salvandone il log per chiudere formalmente FASE 5.
4. **Isolamento di FASE 6:**
   - FASE 6 (RAW Streaming su PC) rimane tassativamente **NON INIZIATA e NON ANCORA AUTORIZZATA**.
