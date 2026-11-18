# TICKFORM — REGISTRO UNICO DEI DISALLINEAMENTI (GAP REGISTER R3.1)

**Data:** 18 novembre 2026  
**Revisione:** R3.1 Candidata  
**Ambito:** Riconciliazione integrale documentazione, evidenze sperimentali, verifiche mirate codice/binari, Git e KiCad  
**Supervisione:** CHAT OVERVIEW  
**Repository:** `A:\Documenti\Tickform`

---

## 1. Criteri di Classificazione

I disallineamenti e le lacune del progetto sono catalogati in tre classi tassative secondo il Mandato R3 (§9):

- **D — Documentale:** Discrepanza o lacuna risolvibile integralmente mediante l'analisi, la correzione o l'armonizzazione delle fonti e dei dati già disponibili nel repository o negli archivi di chat. Non richiede interventi hardware né decisioni di merito non già prese.
- **V — Verifica necessaria:** Mancanza di evidenza oggettiva, log strumentale o misura fisica. Richiede una verifica a banco, una lettura strumentale o un test controllato per essere chiusa.
- **A — Approvazione necessaria:** Scelta architetturale, autorizzazione di gate, modifica della baseline o avanzamento di fase che richiede una decisione formale ed esplicita della supervisione (CHAT OVERVIEW).

---

## 2. Tabella Sinottica del Gap Register R3.1

| ID | Cat. | Descrizione Sintetica | Gravità | Necessità OVERVIEW | Necessità HW Test | Stato R3.1 |
| :--- | :---: | :--- | :---: | :---: | :---: | :---: |
| **GAP-D-01** | D | Mancata inclusione dei log F5-D03 e rettifica unità DMA (592 byte) | ALTA | NO | NO | **RISOLTO IN R3.1** |
| **GAP-D-02** | D | Attribuzione errata del firmware dei sei riavvii a F5-A02-R2.1 anziché F5-D03 | CRITICA | NO | NO | **RISOLTO IN R3.1** |
| **GAP-D-03** | D | Campionamento GPIO IDR post-stop, range 1,9–2,1 MSps e limiti Nyquist su BCK | MEDIA | NO | NO | **RISOLTO IN R3.1** |
| **GAP-D-04** | D | Confusione tra CAD (resistenze 10k R1/R2) e cablaggio banco AS BUILT (non misurato) | MEDIA | NO | NO | **RISOLTO IN R3.1** |
| **GAP-D-05** | D | Presenza di collegamenti non portabili `file:///A:/...` nei Markdown | BASSA | NO | NO | **RISOLTO IN R3.1** |
| **GAP-D-06** | D | Documenti canonici (`STATE`, `LOG`, `HANDOFF`, `CHECKLIST`) fermi a FASE 4 | ALTA | SÌ (approv. proposta) | NO | **PROPOSTA R3.1 PRONTA** |
| **GAP-D-07** | D | Discrepanza dimensione file BIN normale (43.748 vs 43.756 byte) | BASSA | NO | NO | **RISOLTO IN R3.1 (43.756 B)** |
| **GAP-D-08** | D | Chiarimento codice stop I²S (`Status=1, ErrorCode=0x40, StopOK=SI`) | BASSA | NO | NO | **RISOLTO IN R3.1 (Nominale ST HAL)** |
| **GAP-V-01** | V | Collaudo formale del firmware normale `tickform_phase5_i2s_rx` con log salvato | ALTA | SÌ (autorizz. flash) | SÌ | **APERTO (Post-R3.1)** |
| **GAP-V-02** | V | Rilievo strumentale e documentazione AS BUILT del cablaggio MD0/MD1 su breadboard | MEDIA | NO | SÌ | **APERTO (Post-R3.1)** |
| **GAP-V-03** | V | Verifica strumentale (LA/PulseView) della continuità del clock BCK a 6,144 MHz | MEDIA | NO | SÌ | **APERTO (Post-R3.1)** |
| **GAP-V-04** | V | Esecuzione del piano di protezione e consolidamento Git (sorgenti F5 non tracciati) | ALTA | SÌ (approv. piano) | NO | **PIANO R3.1 PRONTO** |
| **GAP-A-01** | A | Autorizzazione al flash e al collaudo del firmware normale F5-A02-R2.1 | ALTA | SÌ | NO | **IN ATTESA OVERVIEW** |
| **GAP-A-02** | A | Approvazione della strategia di stabilizzazione HW e futuro passaggio a KiCad R0.2 | MEDIA | SÌ | NO | **IN ATTESA OVERVIEW** |
| **GAP-A-03** | A | Chiusura formale di FASE 5 e autorizzazione al passaggio a FASE 6 (RAW Stream) | CRITICA | SÌ | NO | **IN ATTESA OVERVIEW** |
| **GAP-A-04** | A | Approvazione formale dell'applicazione delle sincronizzazioni ai 4 documenti canonici | MEDIA | SÌ | NO | **IN ATTESA OVERVIEW** |

---

## 3. Schede Dettagliate delle Nuove Risoluzioni e Verifiche in R3.1

### GAP-D-07 — Discrepanza dimensione binario normale operativo (R3-ERR-05)
- **Categoria:** D — Documentale / Verifica tecnica
- **Descrizione:** Alcuni documenti storici citavano 43.748 byte ed altri 43.756 byte per `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin` (aventi entrambi lo stesso hash SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`).
- **Verifica oggettiva:** L'ispezione diretta sul target locale ha accertato che il file reale presente su disco misura **esattamente 43.756 byte** con SHA-256 `04B95777...`. La cifra 43.748 era un refuso di trascrizione manuale originatosi in `RAPPORTO_F5_D03.md`.
- **Risoluzione in R3.1:** Bonificate tutte le tabelle documentali della suite R3.1 con il valore esatto di **43.756 byte**.
- **Stato:** **RISOLTO**.

---

### GAP-D-08 — Chiarimento documentale su Stop I²S, Status ed ErrorCode (R3-ERR-04)
- **Categoria:** D — Documentale / Analisi codice
- **Descrizione:** Nei log F5-D03 la riga `Stop: Status=1, ErrorCode=0x40, StopOK=SI` presentava apparenti incongruenze rispetto a `HAL Error CB = 0` ed `OVR = 0` durante lo streaming.
- **Verifica oggettiva:** L'analisi del driver ST HAL (`stm32g4xx_hal_i2s.c:1450-1456`) e del firmware diagnostico (`i2s_rx_diag.c:692-712`) ha dimostrato che:
  1. `Status=1` (`HAL_ERROR`) ed `ErrorCode=0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) sono generati sistematicamente dal driver ST per indicare che il Master clock esterno è ancora attivo all'atto dello stop software dello Slave Receiver.
  2. La logica diagnostica verifica nei registri hardware che il canale DMA e la periferica SPI2 siano spenti, riconoscendo tale codice come nominale (`StopOK=SI`).
  3. Durante i 10 secondi di streaming non si è verificato alcun errore.
- **Risoluzione in R3.1:** Documentato l'intero meccanismo in `RAPPORTO_VERIFICHE_MIRATE_R3_1.md` (§1), chiarendo che si tratta di un comportamento previsto e deterministico.
- **Stato:** **RISOLTO / CHIUSO DOCUMENTALMENTE**.

---

### GAP-V-04 — Protezione del lavoro e piano di consolidamento Git
- **Categoria:** V — Verifica necessaria / A — Approvazione necessaria
- **Descrizione:** I sorgenti di FASE 5, i file di log e la documentazione risiedono come file non tracciati nel filesystem locale.
- **Risoluzione in R3.1:** Elaborato il documento `PIANO_PROTEZIONE_GIT_R3_1.md` che definisce:
  1. Livello 1: Snapshot locale verificabile con hash SHA-256 indipendente da Git.
  2. Livello 2: Backup esterno al filesystem `A:`.
  3. Livello 3: Due commit atomici sequenziali (Commit A: sorgenti e build; Commit B: memoria persistente, evidenze e documenti canonici) pronti per autorizzazione di CHAT OVERVIEW.
- **Stato:** **PIANO PROPOSTO IN ATTESA DI AUTORIZZAZIONE OVERVIEW**.
