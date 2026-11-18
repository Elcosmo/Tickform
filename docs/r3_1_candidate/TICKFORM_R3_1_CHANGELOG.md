# TICKFORM — CHANGELOG DELLA REVISIONE CANDIDATA R3.1

**Data:** 18 novembre 2026  
**Autore:** Antigravity (Ingegnere di Sistema e Archivista di Progetto)  
**Supervisione:** CHAT OVERVIEW  
**Documento di riferimento:** Revisione indipendente ChatGPT R3 (`TICKFORM_R3_REVIEW_INDIPENDENTE_20261118.md`) e Mandato R3.1  
**Repository:** `A:\Documenti\Tickform`  
**Collocazione copie R3.1:** `docs/r3_1_candidate/` (la versione R3 precedente è preservata intatta in `docs/`)

---

## 1. Sommario della Revisione R3.1

La revisione R3.1 applica le cinque rettifiche mirate richieste dalla revisione indipendente sulla candidata R3, preservando integralmente tutte le ricostruzioni corrette precedentemente approvate (roadmap, cronologia del firmware diagnostico F5-D03, integrità di KiCad R0.1, separazione dei 6 run diagnostici e isolamento di FASE 6):

1. **R3-ERR-01 (Unità di misura DMA):** Rettificata la quantificazione del volume trasferito prima dello stallo nel Run 1: CNDTR è diminuito di **296 halfword (592 byte)**, correggendo il precedente refuso di "296 byte".
2. **R3-ERR-02 (Cronologia e cause dei guasti):** Riclassificate tutte le asserzioni causali. È stato esplicitato che il campionamento GPIO IDR avviene **dopo** lo stop DMA; i livelli fissi HIGH/LOW fotografano la finestra post-stop e non misurano retroattivamente l'andamento continuo durante i 10s di ricezione. I flood di interrupt sono descritti come fenomeni misurati, separando rigorosamente fatti, osservazioni dell'operatore ed ipotesi di cablaggio.
3. **R3-ERR-03 (Checklist e gestione errori):** Nella proposta di aggiornamento della checklist canonica (`PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3_1.md`), la voce `Overflow/error handling implemented` è stata **lasciata non spuntata `[ ]`** (poiché F5-D03 convalida il monitoraggio/reporting, non il recupero automatico post-anomalia); la voce `DMA stability verified` è stata ristretta formalmente ad *«acquisizioni nominali di 10 secondi con firmware diagnostico»*.
4. **R3-ERR-04 (Stop I²S, Status ed ErrorCode):** Completata l'ispezione in sola lettura del driver ST HAL (`stm32g4xx_hal_i2s.c`) e del firmware diagnostico (`i2s_rx_diag.c`). Dimostrato che `Status=1` (`HAL_ERROR`) ed `ErrorCode=0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) sono il comportamento sistematico e deterministico di `HAL_I2S_DMAStop` in modalità Slave RX quando il Master clock continua ad oscillare, e che `i2s_rx_diag.c` (righe 708–709) riconosce tale codice come nominale (`StopOK=SI`). Non vi è alcuna anomalia di ricezione.
5. **R3-ERR-05 (Dimensione e hash BIN normale):** Eseguita la verifica fisica sul target locale: confermata la dimensione di **43.756 byte** con SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`. Tracciata l'origine del valore 43.748 byte come refuso tipografico di trascrizione in `RAPPORTO_F5_D03.md`.
6. **Protezione Git:** Elaborato il documento `PIANO_PROTEZIONE_GIT_R3_1.md` con snapshot locale di sicurezza, backup esterno e piano di consolidamento in due commit atomici approvati.

---

## 2. Dettaglio delle Modifiche Applicate per Documento

### 2.1. `docs/r3_1_candidate/TICKFORM_PROJECT_MEMORY.md`
- Aggiornata l'intestazione e inserite le Note di Revisione R3.1.
- §8.A (Run 1): Corretto il testo in *«CNDTR diminuito di 296 halfword (592 byte) prima dello stallo osservato; zero callback Half/Full»*.
- §8.A (Run 1, 3, 4): Riformulate le diagnosi in *Osservazioni strumentali fenomenologiche*, specificando che il GPIO IDR è post-stop e che l'origine dei flood di IRQ SPI2 (con OVR=0 e HAL Error last=0) resta un'ipotesi tecnica non isolata.
- §8.A (Stop I²S): Integrata la spiegazione tecnica di `Status=1`, `ErrorCode=0x40` e `StopOK=SI`.
- §11 (Problemi risolti #8): Riformulata la voce distinguendo fenomeno osservato, ipotesi di falso contatto ed esito dell'intervento meccanico.
- §13.2 (Cronologia crittografica): Confermato il valore esatto di **43.756 byte** per `tickform_phase5_i2s_rx.bin` (eliminando ogni riferimento a 43.748).

### 2.2. `docs/r3_1_candidate/TICKFORM_FULL_PROJECT_HANDOFF.md`
- Aggiornate le Note di Revisione R3.1.
- §4.2: Integrato il chiarimento sullo stop I²S in modalità Slave Receiver.
- §5 (Tabella F5-D03): Corretto il Run 1 a 592 byte e riformulate le descrizioni dei run anomali in modo oggettivo.
- §6: Confermato il valore di 43.756 byte per il binario normale operativo.

### 2.3. `docs/r3_1_candidate/TICKFORM_DOCUMENTATION_AUDIT.md`
- Aggiornate le Note di Revisione R3.1.
- Censiti i nuovi documenti specialistici R3.1: `RAPPORTO_VERIFICHE_MIRATE_R3_1.md`, `PIANO_PROTEZIONE_GIT_R3_1.md`, `TABELLA_CORREZIONI_R3_1.md`, `TICKFORM_GAP_REGISTER_R3_1.md`, `PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3_1.md`.
- Convalidata l'integrità complessiva dell'archivio.

### 2.4. Nuovi Documenti Generati per R3.1
- `docs/r3_1_candidate/RAPPORTO_VERIFICHE_MIRATE_R3_1.md`: Trattazione tecnica esaustiva su Stop I²S, ErrorCode 0x40, dimensione BIN e campionamento GPIO IDR.
- `docs/r3_1_candidate/PIANO_PROTEZIONE_GIT_R3_1.md`: Piano a 3 livelli per proteggere i file non tracciati e guidare il futuro commit di consolidamento.
- `docs/r3_1_candidate/TABELLA_CORREZIONI_R3_1.md`: Matrice comparativa dettagliata R3 vs R3.1.
- `docs/r3_1_candidate/TICKFORM_GAP_REGISTER_R3_1.md`: Registro disallineamenti aggiornato con chiusura documentale di GAP-D-07 e GAP-D-08.
- `docs/r3_1_candidate/PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3_1.md`: Bozza non distruttiva aggiornata per i 4 documenti canonici.
- `docs/r3_1_candidate/MANIFEST_R3_1.txt`: Manifest con dimensioni ed hash SHA-256 dei file R3.1.
