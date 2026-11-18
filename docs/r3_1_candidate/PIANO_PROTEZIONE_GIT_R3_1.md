# TICKFORM — PIANO DI PROTEZIONE E CONSOLIDAMENTO GIT (R3.1)

**Data:** 18 novembre 2026  
**Autore:** Antigravity (Ingegnere di Sistema e Archivista di Progetto)  
**Supervisione:** CHAT OVERVIEW  
**Destinatario:** CHAT OVERVIEW  
**Riferimento:** Mandato R3.1 (§6)  
**Repository:** `A:\Documenti\Tickform`  
**Modalità operativa:** PIANO PROPOSITIVO NON DISTRUTTIVO (Nessuna operazione di commit, push o modifica Git eseguita)

---

## 1. Analisi del Rischio di Conservazione

L'audit del repository ha confermato che:
1. Il branch `main` è fermo al commit [`e857849debbc54e655a8a87a88c07a2ee0386d43`](https://github.com/Elcosmo/Tickform/commit/e857849debbc54e655a8a87a88c07a2ee0386d43) del 13/11/2026 (allineato a `origin/main`).
2. L'intero codice sorgente sviluppato per la FASE 5 (`i2s_rx.c`, `i2s_rx.h`, `i2s_rx_diag.c`, `i2s_rx_diag.h`), i 5 log originali di laboratorio in `docs/evidenze_f5_d03/`, le modifiche a `CMakeLists.txt` e l'intera suite documentale di revisione **risiedono attualmente come file non tracciati o modifiche del working tree sul singolo disco locale `A:`**.
3. Sebbene tali file siano preservati all'interno degli archivi ZIP di revisione (`tickform_f5_d03_review_pack.zip` e `tickform_documentation_reconciliation_R3_review.zip`), la loro assenza dalla cronologia Git ufficiale costituisce una **criticità di conservazione ad alta priorità**.

---

## 2. Strategia di Protezione Articolata in Tre Livelli

Per garantire la sicurezza del lavoro svolto senza violare i mandati della supervisione, si propone una strategia a tre livelli:

```text
[Livello 1: Snapshot Locale Immediato] ─── Archivio ZIP deterministico fuori da .git
                 │
                 ▼
[Livello 2: Backup Verificabile Esterno] ── Copia crittografata/verificata fuori dal disco A:
                 │
                 ▼
[Livello 3: Commit Atomico di Consolidamento] ── Commit Git approvato da CHAT OVERVIEW
```

---

## 3. Livello 1: Snapshot Verificabile Indipendente da Git (Pronto all'Uso)

Prima di qualsiasi operazione sul working tree Git, deve essere creato un archivio snapshot completo (escluso `.git/` e cartelle di build intermedie `build/` o cache Ninja):
- **Nome archivio proposto:** `tickform_fase5_full_snapshot_20261118.zip`
- **Contenuto:** L'intero albero di `A:\Documenti\Tickform\` (sorgenti firmware, schemi KiCad, script, BOM, documenti `docs/` e log `docs/evidenze_f5_d03/`).
- **Verifica:** Calcolo e archiviazione del relativo hash SHA-256 in un file manifest separato.

---

## 4. Livello 2: Backup Esterno al Filesystem Locale

Per proteggere il progetto da guasti hardware del volume locale `A:`, lo snapshot di Livello 1 deve essere copiato:
1. Su una cartella secondaria del sistema host (es. `C:\Users\omnia\Downloads\Tickform_Backups\`).
2. Su supporto esterno o cloud storage controllato dall'operatore.

---

## 5. Livello 3: Piano per il Futuro Commit di Consolidamento Git

Non appena CHAT OVERVIEW avrà formalmente ratificato la documentazione R3.1 e autorizzato il consolidamento, l'operazione dovrà avvenire mediante **due commit sequenziali e atomici**:

### Commit A — Consolidamento Sorgenti Firmware FASE 5 e Infrastructure
- **File da includere (`git add`):**
  - `firmware/stm32g431/CMakeLists.txt`
  - `firmware/stm32g431/build.ps1`
  - `firmware/stm32g431/Core/Inc/stm32g4xx_hal_conf.h`
  - `firmware/stm32g431/Core/Src/main.c`
  - `firmware/stm32g431/Core/Inc/i2s_rx.h`
  - `firmware/stm32g431/Core/Inc/i2s_rx_diag.h`
  - `firmware/stm32g431/Core/Src/i2s_rx.c`
  - `firmware/stm32g431/Core/Src/i2s_rx_diag.c`
- **Messaggio di commit proposto:**
  ```text
  feat(firmware): implement I2S2 Slave RX DMA and hardware diagnostic targets

  - Add tickform_phase5_i2s_rx target for normal 96kHz stereo audio reception
  - Add tickform_phase5_diag target for register monitoring and GPIO verification
  - Enable HAL_I2S module and configure DMAMUX1 request ID 12 for DMA1 Channel 1
  - Update build.ps1 to support multi-target builds with phase3 default
  ```

### Commit B — Consolidamento Memoria Persistente, Evidenze F5-D03 e Documenti Canonici
- **File da includere (`git add`):**
  - `docs/TICKFORM_PROJECT_MEMORY.md`
  - `docs/TICKFORM_FULL_PROJECT_HANDOFF.md`
  - `docs/TICKFORM_DOCUMENTATION_AUDIT.md`
  - `docs/TICKFORM_GAP_REGISTER_R3_1.md`
  - `docs/TABELLA_CORREZIONI_R3_1.md`
  - `docs/TICKFORM_R3_1_CHANGELOG.md`
  - `docs/evidenze_f5_d03/*` (5 log Tera Term + manifest)
  - `docs/PROJECT_STATE.md` (aggiornato)
  - `docs/PROJECT_LOG.md` (append-only aggiornato)
  - `docs/CHAT_HANDOFF.md` (aggiornato)
  - `docs/bringup/BRINGUP_CHECKLIST.md` (aggiornato)
- **Messaggio di commit proposto:**
  ```text
  docs(governance): synchronize project persistent memory and F5-D03 evidence

  - Establish TICKFORM_PROJECT_MEMORY R3.1 as persistent system reference
  - Archive verified F5-D03 raw laboratory logs in docs/evidenze_f5_d03/
  - Update PROJECT_STATE, PROJECT_LOG, CHAT_HANDOFF, and BRINGUP_CHECKLIST
  - Reconcile firmware chronology and open gates ahead of normal operational test
  ```

---

## 6. Azioni Immediate Autorizzate vs Da Sottoporre ad Autorizzazione

- **Autorizzato in questo mandato (R3.1):** Elaborazione del piano e verifica in sola lettura dello stato Git.
- **Da autorizzare da CHAT OVERVIEW:** Esecuzione dello snapshot di backup esterno, esecuzione dei comandi `git add` e `git commit`, ed eventuale `git push origin main`.
