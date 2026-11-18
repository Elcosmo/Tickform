# TICKFORM — PROPOSTA DI SINCRONIZZAZIONE DEI DOCUMENTI CANONICI (R3)

**Data:** 18 novembre 2026  
**Autore:** Antigravity (Revisore Tecnico e Archivista di Progetto)  
**Supervisione:** CHAT OVERVIEW  
**Stato della proposta:** BOZZA NON DISTRUTTIVA PER APPROVAZIONE (Nessun file canonico è stato modificato sul filesystem)  
**Documenti Canonici di Destinazione:**
1. `docs/PROJECT_STATE.md`
2. `docs/PROJECT_LOG.md`
3. `docs/CHAT_HANDOFF.md`
4. `docs/bringup/BRINGUP_CHECKLIST.md`

---

## 1. Principi di Governance e Disaccoppiamento

Per preservare l'integrità documentale del progetto TICKFORM, il presente documento stabilisce la separazione tra:
- **La Memoria Persistente Centrale (`TICKFORM_PROJECT_MEMORY.md`):** Il testo di riferimento completo, omnicomprensivo e persistente, destinato a sopravvivere al passaggio tra sessioni e modelli di intelligenza artificiale.
- **I Documenti Canonici Operativi (`STATE`, `LOG`, `HANDOFF`, `CHECKLIST`):** Documenti vivi con compiti specifici, mantenuti snelli per evitare duplicazioni o contraddizioni.

### Regola Fondamentale di Aggiornamento
Nessun documento canonico viene modificato direttamente dall'agente locale senza un preventivo mandato o approvazione esplicita di CHAT OVERVIEW.  
Le seguenti sezioni contengono le modifiche puntuali proposte, pronte per essere applicate con singole operazioni di append o sostituzione di blocco non appena autorizzate.

---

## 2. Proposta per `docs/PROJECT_STATE.md`

### Obiettivo del documento
Fornire un quadro sintetico e immediato dello stato attivo, rispondendo a: *Dove siamo? Cosa funziona? Cosa manca? Qual è il prossimo gate?*

### Testo Proposto in Sostituzione dell'Intestazione e dei Gate (Righe 1–30)

```markdown
# Tickform — Project state

Fotografia vivente dello stato corrente. Aggiornare quando cambia realmente il progetto; la cronologia resta in PROJECT_LOG e le decisioni negli ADR.

- **Last updated:** 2026-11-18
- **Reference commit:** e857849 — Checkpoint PCM1808 C 96kHz bring-up and KiCad schematic
- **Current milestone:** FASE 5 — PCM1808 → STM32 (ricezione I²S2, DMA circolare, diagnostica hardware): QUALIFICA TRASPORTO COMPLETATA (Collaudo firmware normale pendente)
- **Next milestone:** Collaudo firmware normale F5-A02-R2.1 → Consolidamento hardware AS BUILT → Chiusura formale FASE 5 → (Subordinata) FASE 6 RAW Stream

## Gate status

- Gate 1 — SWD: PASS (400 kHz, rail 3V3 verificato)
- Gate 2 — minimal firmware: PASS (LED PC6 lampeggiante)
- Gate 3 — MCU clock 170 MHz: PASS (PLL e SysTick convalidati)
- Gate 4 — native USB enumeration: PASS (COM CDC 0483:5740 operativa)
- Gate 5 — LRCK nominale 96 kHz (PCM1808 C): PASS (cursori PulseView 96,000 kHz, CAD KiCad R0.1 approvato)
- Gate 6 — Trasporto I²S2 Slave DMA (F5-D03): PASS (3.750–3.751 callback DMA/10s, 0 errori SPI2/HAL con cablaggio ripristinato)
- Gate 7 — Collaudo firmware normale F5-A02-R2.1: PENDING (approvato crittograficamente, collaudo a banco con clock attivi non ancora eseguito)
- Gate 8 — Conversione audio reale (AFE + analogica): PENDING (ingressi J3 lasciati aperti; AFE non collegato)
- Gate 9 — Streaming RAW continuo verso PC (FASE 6): NON INIZIATO — NON ANCORA AUTORIZZATO
```

---

## 3. Proposta per `docs/PROJECT_LOG.md` (Append-Only)

### Obiettivo del documento
Registro storico cronologico immutabile. Le voci passate non vengono mai riscritte o cancellate; si aggiungono esclusivamente nuove sezioni datate in coda al file.

### Testo Proposto da Accodare in Fondo a `docs/PROJECT_LOG.md`

```markdown
## 2026-11-18 — FASE 5: Riconciliazione Diagnostica F5-D03, Qualifica Hardware e Allineamento Documentale R3

- **Attività svolta:** Riconciliazione integrale dei log di laboratorio F5-D03, verifica dello stato Git e KiCad, e rettifica della cronologia firmware STM32.
- **Evidenze sperimentali archiviate:**
  - Importati in `docs/evidenze_f5_d03/` i 5 file di log Tera Term originali con verifica crittografica SHA-256 contro il relativo manifest.
  - Identificati formalmente 6 run diagnostici distinti del firmware `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`):
    - Run 1 (13:35:19): Stallo hardware iniziale; CNDTR fermo a 1752, 0 CB DMA, 103 IRQ SPI2/HAL error CB; GPIO IDR PB12/13 fissi a HIGH.
    - Run 2 (22:05:46): Regolare; 3.751 CB DMA (Half=1876, Full=1875), 0 IRQ SPI2, 0 callback errore HAL, 0 OVR; GPIO PB12 con 1920 transizioni a 1921 kSps (~96 kHz).
    - Run 3 (22:20:21): Anomalo; DMA interrotto a 1122 CB con flood di 518.924 IRQ SPI2; GPIO PB12/13 fissi a LOW.
    - Run 4 (22:25:42): Anomalo; 3.714 CB DMA con flood di 3.567.483 IRQ SPI2; clock presenti al test GPIO IDR.
    - Run 5A (22:43:18 rep 1): Regolare; 3.750 CB DMA nominali in 10s, 0 IRQ SPI2, 0 errori HAL; GPIO PB12 con 1921 transizioni.
    - Run 5B (22:43:18 rep 2-3): Regolare; 3.751 CB DMA nominali in 10s, 0 IRQ SPI2, 0 errori HAL; GPIO PB12 con 1922 transizioni.
- **Rettifica fondamentale sul firmware dei 6 riavvii:**
  - A seguito del riposizionamento fisico dei collegamenti su breadboard, l'operatore ha eseguito tre reset NRST e tre cicli di spegnimento USB-C senza riscontrare accensione fissa del LED blu di errore.
  - Si registra formalmente che durante questi 6 reboot il firmware in esecuzione sulla STM32 era `tickform_phase5_diag.bin`, NON il firmware normale `tickform_phase5_i2s_rx.bin` (nessun nuovo flash è stato effettuato).
  - Tali riavvii costituiscono evidenza osservazionale di ripetibilità del boot e del LED, non collaudo certificato con log salvati.
- **Riconciliazione campionamento GPIO IDR:**
  - La frequenza effettiva misurata su target è risultata compresa tra 1908 e 2067 kSps (~1,9–2,1 MSps).
  - Il campionamento è ampiamente sufficiente per confermare la commutazione del Word Clock LRCK a 96 kHz (PB12), mentre è sotto Nyquist per il Bit Clock BCK a 6,144 MHz (PB13), generando aliasing. Zero transizioni su PB13 non provano l'assenza fisica del clock.
- **Stato CAD KiCad e Hardware AS BUILT:**
  - Lo schema KiCad R0.1 è confermato integro e approvato per FASE 4 (ERC 0 errori / 0 avvisi).
  - Si distingue formalmente la prescrizione CAD (due resistenze 10 kΩ R1/R2) dalla realtà del banco fisico AS BUILT (livello logico HIGH confermato, ma valore resistivo e topologia Y non misurati con multimetro).
- **Prossimi passi:** Collaudo del firmware normale `tickform_phase5_i2s_rx.bin` con archiviazione del log formale; FASE 6 rigorosamente NON INIZIATA e NON AUTORIZZATA.
```

---

## 4. Proposta per `docs/CHAT_HANDOFF.md`

### Obiettivo del documento
Fornire alle nuove sessioni di lavoro (e alle nuove chat di supervisione) il contesto operativo immediato senza costringerle a ricalcolare le baseline storiche.

### Testo Proposto per Sostituire Integralmente `docs/CHAT_HANDOFF.md`

```markdown
# Tickform — Context Handoff per Nuove Sessioni / Supervisione

Documento di sintesi operativa per il passaggio rapido di contesto. Per la trattazione tecnica completa consultare `docs/TICKFORM_PROJECT_MEMORY.md` (Revisione R3).

- **Data:** 18 novembre 2026
- **Root repository:** `A:\Documenti\Tickform`
- **Branch Git attivo:** `main` (HEAD = `e857849debbc54e655a8a87a88c07a2ee0386d43`, allineato a `origin/main`)
- **Stato Working Tree:** 4 file tracciati modificati (`CMakeLists.txt`, `stm32g4xx_hal_conf.h`, `main.c`, `build.ps1`), sorgenti I²S untracked protetti. Nessun commit/push autorizzato.
- **Stato Progetto:** FASE 5 (PCM1808 → STM32 I²S2 Slave DMA). Qualifica del trasporto dati e della diagnostica interna F5-D03 COMPLETATA con successo. Collaudo del firmware normale F5-A02-R2.1 in attesa di mandato formale.
- **FASE 6 (Streaming RAW verso PC):** RIGOROSAMENTE NON INIZIATA E NON ANCORA AUTORIZZATA.

## Punti Chiave da Sapere Immediatamente
1. **Evidenze di Laboratorio F5-D03 Riconciliate:**
   - 5 file Tera Term archiviati in `docs/evidenze_f5_d03/` con hash SHA-256 verificati.
   - 6 run diagnostici distinti censiti: run anomali iniziali dovuti a contatti volanti su breadboard; run nominali finali regolari con 3.750–3.751 callback DMA in 10s e ZERO errori SPI2/HAL.
   - Le ritrasmissioni periodiche CDC (stessi contatori) non sono prove indipendenti.
2. **Rettifica Firmware dei Sei Reboot:**
   - Durante i 6 riavvii (3 reset NRST + 3 cicli USB) la STM32 eseguiva il firmware diagnostico `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`), NON il firmware normale `tickform_phase5_i2s_rx.bin`.
   - Il firmware normale è approvato e compilato (SHA-256 `04B95777...`), ma NON è ancora stato collaudato a banco con i clock attivi.
3. **Campionamento GPIO IDR:**
   - Frequenza reale misurata: 1,9–2,1 MSps. PB12 (LRCK 96 kHz) campionato correttamente (~1920 transizioni in 10 ms). PB13 (BCK 6,144 MHz) sotto Nyquist: le transizioni campionate risentono di aliasing e zero transizioni non dimostrano assenza di clock.
4. **Hardware KiCad R0.1 vs Banco:**
   - Progetto KiCad R0.1 integro e conforme per FASE 4 (ERC: 0 errori, 0 avvisi). Non include PB12/13/15 e DS3231.
   - Resistenze pull-up MD0/MD1: nel CAD sono due da 10 kΩ; sul banco fisico l'operatore ricorda un cavo a Y e le resistenze non sono state misurate con multimetro (livello HIGH accertato).
5. **Prossima Attività da Autorizzare:**
   - Mandato di flash e collaudo del firmware normale `tickform_phase5_i2s_rx.bin` (10s di acquisizione con log seriale salvato).
   - Rilievo fotografico e misura delle resistenze AS BUILT prima del passaggio a perfboard e KiCad R0.2.
```

---

## 5. Proposta per `docs/bringup/BRINGUP_CHECKLIST.md`

### Obiettivo del documento
Aggiornare lo stato di spunta dei criteri di accettazione di FASE 5 e preservare i vincoli sulle fasi future.

### Testo Proposto per la Sezione FASE 5 di `BRINGUP_CHECKLIST.md`

```markdown
## FASE 5 — PCM1808 → STM32

- [x] STM32 receives I2S (I²S2 Slave Receiver operativo a 96 kHz stereo su PB12/13/15; verificato in F5-D03)
- [x] DMA running (DMA1 Channel 1 in modalità circolare; CNDTR monitorato e decrementato nominalmente)
- [x] DMA stability verified (3.750–3.751 callback Half/Full in 10 secondi con zero errori SPI2/HAL nel firmware F5-D03)
- [ ] Collaudo formale firmware normale operativo F5-A02-R2.1 (tickform_phase5_i2s_rx.bin pendente con log archiviato)
- [ ] Continuous 64-bit sample counter implemented (struttura predisposta, da convalidare nello streaming continuo)
- [x] Overflow/error handling implemented (gestione HAL Error Callback, monitor OVR e ripristino verificati in F5-D03)

Esiti FASE 5, 17-18/11/2026 (PROJECT_LOG): Qualifica del trasporto dati hardware I²S2→DMA completata con successo tramite diagnostica F5-D03 su banco con cablaggio ripristinato. Run nominali con 3750-3751 CB in 10s e zero errori. Firmware in esecuzione durante le prove e i sei reboot: tickform_phase5_diag.bin. Collaudo del firmware normale F5-A02-R2.1 identificato come prossimo gate aperto obbligatorio prima della chiusura formale della fase. FASE 6 NON INIZIATA e NON AUTORIZZATA.
```

---

## 6. Procedura di Applicazione delle Modifiche Autorizzate

Non appena CHAT OVERVIEW avrà formalmente approvato la presente proposta:
1. `docs/PROJECT_STATE.md`: Aggiornare le righe di intestazione e lo stato dei Gate.
2. `docs/PROJECT_LOG.md`: Accodare la sezione del 18/11/2026 in modalità append-only.
3. `docs/CHAT_HANDOFF.md`: Sovrascrivere con il nuovo testo di sintesi operativa R3.
4. `docs/bringup/BRINGUP_CHECKLIST.md`: Spuntare le voci convalidate di FASE 5 e registrare le note d'esito.
