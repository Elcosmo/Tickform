# TICKFORM — REGISTRO UNICO DEI DISALLINEAMENTI (GAP REGISTER R3)

**Data:** 18 novembre 2026  
**Revisione:** R3 Candidata  
**Ambito:** Riconciliazione integrale documentazione, evidenze sperimentali, firmware, KiCad e Git  
**Supervisione:** CHAT OVERVIEW  
**Repository:** `A:\Documenti\Tickform`

---

## 1. Criteri di Classificazione

I disallineamenti e le lacune del progetto sono catalogati in tre classi tassative secondo il Mandato R3 (§9):

- **D — Documentale:** Discrepanza o lacuna risolvibile integralmente mediante l'analisi, la correzione o l'armonizzazione delle fonti e dei dati già disponibili nel repository o negli archivi di chat. Non richiede interventi hardware né decisioni di merito non già prese.
- **V — Verifica necessaria:** Mancanza di evidenza oggettiva, log strumentale o misura fisica. Richiede una verifica a banco, una lettura strumentale o un test controllato per essere chiusa.
- **A — Approvazione necessaria:** Scelta architetturale, autorizzazione di gate, modifica della baseline o avanzamento di fase che richiede una decisione formale ed esplicita della supervisione (CHAT OVERVIEW).

---

## 2. Tabella Sinottica del Gap Register

| ID | Cat. | Descrizione Sintetica | Gravità | Necessità OVERVIEW | Necessità HW Test | Stato R3 |
| :--- | :---: | :--- | :---: | :---: | :---: | :---: |
| **GAP-D-01** | D | Mancata inclusione dei log F5-D03 nel repository e confusione sulle prove positive | ALTA | NO | NO | **RISOLTO IN R3** |
| **GAP-D-02** | D | Attribuzione errata del firmware dei sei riavvii a F5-A02-R2.1 anziché F5-D03 | CRITICA | NO | NO | **RISOLTO IN R3** |
| **GAP-D-03** | D | Interpretazione e campionamento GPIO IDR: stima teorica e limite Nyquist su BCK | MEDIA | NO | NO | **RISOLTO IN R3** |
| **GAP-D-04** | D | Confusione tra CAD (resistenze 10k R1/R2) e cablaggio banco AS BUILT (non misurato) | MEDIA | NO | NO | **RISOLTO IN R3** |
| **GAP-D-05** | D | Presenza di collegamenti non portabili `file:///A:/...` nei Markdown | BASSA | NO | NO | **RISOLTO IN R3** |
| **GAP-D-06** | D | Documenti canonici (`PROJECT_STATE`, `LOG`, `HANDOFF`, `CHECKLIST`) fermi a FASE 4 | ALTA | SÌ (approv. proposta) | NO | **RISOLTO IN C01/C02** |
| **GAP-V-01** | V | Collaudo formale del firmware normale `tickform_phase5_i2s_rx` con log salvato | ALTA | SÌ (autorizz. flash) | SÌ | **RISOLTO (20/11/2026)** |
| **GAP-V-02** | V | Documentazione AS BUILT e verifica strumentale MD0/MD1 e alimentazioni | MEDIA | NO | SÌ | **AS BUILT CONFERMATO / MISURA PENDENTE (H01)** |
| **GAP-V-03** | V | Verifica strumentale (LA/PulseView) della continuità del clock BCK a 6,144 MHz | MEDIA | NO | SÌ | **APERTO (Post-R3)** |
| **GAP-V-04** | V | Verifica allineamento e pulizia del working tree Git prima del commit FASE 5 | BASSA | NO | NO | **RISOLTO IN C01/C02** |
| **GAP-A-01** | A | Autorizzazione al flash e al collaudo del firmware normale F5-A02-R2.1 | ALTA | SÌ | NO | **RISOLTO (20/11/2026)** |
| **GAP-A-02** | A | Approvazione della strategia di stabilizzazione HW e futuro passaggio a KiCad R0.2 | MEDIA | SÌ | NO | **IN ATTESA OVERVIEW** |
| **GAP-A-03** | A | Chiusura formale di FASE 5 e autorizzazione al passaggio a FASE 6 (RAW Stream) | CRITICA | SÌ | NO | **IN ATTESA OVERVIEW (subordinata a H01)** |
| **GAP-A-04** | A | Approvazione formale dell'applicazione delle sincronizzazioni ai 4 documenti canonici | MEDIA | SÌ | NO | **RISOLTO IN C01/C02** |
| **GAP-H-01** | V/A | Stabilizzazione meccanica ed elettrica cablaggi hardware banco (Gate H01) | ALTA | SÌ | SÌ | **APERTO / NEXT (Gate H01)** |
| **GAP-T-01** | D/V | Implementazione continuous 64-bit sample counter e recovery attiva overflow/errori | ALTA | SÌ | SÌ | **APERTO (Requisiti FASE 6)** |

---

## 3. Schede Dettagliate delle Lacune e dei Disallineamenti

### GAP-D-01 — Integrazione ed esame completo dei log F5-D03
- **Categoria:** D — Documentale
- **Descrizione:** La memoria R2 considerava unicamente il primo log anomalo (13:35:19) e trattava le successive acquisizioni positive come dichiarazioni verbali prive di file. Nella conversazione erano invece stati condivisi 5 file Tera Term originali contenenti sia test negativi/anomali che test regolari. Inoltre, le ritrasmissioni periodiche CDC rischiavano di essere contate erroneamente come test distinti.
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md` (§8), `TICKFORM_FULL_PROJECT_HANDOFF.md` (§8), `TICKFORM_DOCUMENTATION_AUDIT.md` (§2.3), `TABELLA_CORREZIONI_R2.md` (CORR-04).
- **Evidenza disponibile:** Archivio `TICKFORM_F5D03_EVIDENZE_CHAT_20261118.zip`, contenente `MANIFEST_EVIDENZE_F5D03_20261118.txt` e i 5 log Tera Term (`Testo incollato(20261117-133519)(1).txt`, `Testo incollato(20261117-220546).txt`, `Testo incollato(20261117-222021).txt`, `Testo incollato(20261117-222542).txt`, `Testo incollato(20261117-224318).txt`).
- **Gravità:** ALTA.
- **Causa accertata:** Mancata archiviazione sistematica nel filesystem di tutti gli allegati scambiati nelle chat.
- **Correzione applicata in R3:**
  1. Estrazione byte-for-byte dei 5 file di evidenza nella cartella di repository `docs/evidenze_f5_d03/` e verifica crittografica degli hash SHA-256 contro il manifest.
  2. Disamina dettagliata di ciascun log: individuati esattamente **6 run diagnostici distinti**, separando nettamente i blocchi di ritrasmissione periodica CDC (aventi contatori e timestamp identici).
  3. Mappatura completa dei run: Run 1 (13:35:19, 0 CB, CNDTR 1752, SPI2 IRQ 103), Run 2 (22:05:46, 3751 CB regolare), Run 3 (22:20:21, 1122 CB, SPI2 IRQ 518k), Run 4 (22:25:42, 3714 CB, SPI2 IRQ 3,56M), Run 5A (22:43:18 rep 1, 3750 CB regolare), Run 5B (22:43:18 rep 2-3, 3751 CB regolare).
- **Necessità decisione OVERVIEW:** NO.
- **Necessità prova HW:** NO.
- **Criterio oggettivo di chiusura:** I 5 log sono archiviati nel repository con SHA-256 verificati e tutti i 6 run sono documentati nelle matrici comparative di `TICKFORM_PROJECT_MEMORY.md`.

---

### GAP-D-02 — Rettifica obbligatoria del firmware in esecuzione durante i sei riavvii
- **Categoria:** D — Documentale
- **Descrizione:** Nei documenti R2, le prove positive finali e i 6 riavvii (3 reset NRST + 3 cicli power-cycle USB) venivano attribuiti a `tickform_phase5_i2s_rx.bin` (F5-A02-R2.1). L'evidenza della sessione e la dichiarazione dell'operatore confermano che sulla STM32 era stato programmato il firmware diagnostico `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`) e che **nessuna nuova programmazione ha avuto luogo** prima dei sei riavvii.
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md` (§8.B, §9, §13), `TICKFORM_FULL_PROJECT_HANDOFF.md` (§8.B, §14), `TICKFORM_DOCUMENTATION_AUDIT.md` (§2.3).
- **Evidenza disponibile:** Dichiarazione dell'operatore nel mandato F5-D03, cronologia operativa di banco del 17/11/2026, intestazioni `DIAGNOSTICA F5-D03` presenti in tutti i log Tera Term.
- **Gravità:** CRITICA.
- **Causa accertata:** Errore di deduzione dell'agente precedente, che ha confuso la disponibilità nel repository del file compilato F5-A02-R2.1 con la sua effettiva programmazione ed esecuzione su target durante i reboot.
- **Correzione applicata in R3:**
  1. Rettifica formale: il firmware in esecuzione durante l'intera seduta diagnostica e durante i 6 riavvii successivi è identificato come `tickform_phase5_diag` (target CMake `tickform_phase5_diag`, build F5-D03).
  2. Istituzione del principio guida: *Firmware compilato $\neq$ firmware approvato $\neq$ firmware programmato $\neq$ firmware collaudato*.
  3. Classificazione del collaudo del firmware normale operativo (`tickform_phase5_i2s_rx.bin`) come attività residua e gate aperto obbligatorio prima della chiusura formale di FASE 5.
  4. Redazione della tabella completa della cronologia di programmazione firmware.
- **Necessità decisione OVERVIEW:** NO.
- **Necessità prova HW:** NO (la prova HW sarà necessaria per collaudare il firmware normale, non per correggere la registrazione storica).
- **Criterio oggettivo di chiusura:** La documentazione R3 attribuisce i log e i sei reboot a `tickform_phase5_diag` e indica `tickform_phase5_i2s_rx` come collaudo pendente.

---

### GAP-D-03 — Prestazioni campionamento GPIO IDR e limiti interpretativi del clock BCK
- **Categoria:** D — Documentale
- **Descrizione:** La stima preliminare di 11–12 MSps formulata in F5-D02 non corrispondeva alla realtà; la R2 aveva recepito il valore reale citando però unicamente il singolo dato a 2.067 kSps (2,07 MSps) del primo log, ignorando che gli altri log registrano tra 1908 e 1921 kSps. Inoltre, la memoria R2 traeva la conclusione che "zero transizioni su PB13 dimostrano assenza di clock BCK", senza considerare il limite di Nyquist e la presenza di aliasing.
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md` (§6.4, §8.A), `TICKFORM_FULL_PROJECT_HANDOFF.md` (§6.4, §8.A).
- **Evidenza disponibile:** Log F5-D03: 13:35:19 (2067 kSps), 22:05:46 (1921 kSps), 22:20:21 (2067 kSps), 22:25:42 (1917 kSps), 22:43:18 (1908 kSps). Codice sorgente `i2s_rx_diag.c` (`run_pin_input_test()` eseguito dopo lo stop DMA).
- **Gravità:** MEDIA.
- **Causa accertata:** Semplificazione interpretativa nell'analisi del campionamento software.
- **Correzione applicata in R3:**
  1. Registrazione della frequenza reale misurata su target nell'intervallo **1908–2067 kSps (~1,9–2,1 MSps)**.
  2. Chiarimento metodologico su PB12 (LRCK, 96 kHz): la finestra da 10 ms con ~2 MSps campiona circa 10 volte per semi-periodo, fornendo evidenza affidabile delle transizioni (rilevate esattamente 1920–1922 transizioni nei run positivi).
  3. Chiarimento metodologico su PB13 (BCK, 6,144 MHz): il campionamento a ~2 MSps è ampiamente sotto Nyquist. Il rilievo di zero transizioni in una finestra di 10 ms post-stop **non dimostra in modo definitivo l'assenza fisica del clock**, potendo derivare da aliasing, sincronismo di campionamento o stallo circoscritto. Si specifica che per conclusioni definitive su BCK occorre una verifica strumentale dedicata (PulseView / oscilloscopio).
  4. Precisazione che `run_pin_input_test()` esamina i pin **dopo** l'arresto del test DMA, non simultaneamente ai 10 secondi di streaming.
- **Necessità decisione OVERVIEW:** NO.
- **Necessità prova HW:** NO.
- **Criterio oggettivo di chiusura:** La trattazione in `TICKFORM_PROJECT_MEMORY.md` e `HANDOFF` recepisce l'intervallo 1,9–2,1 MSps e le relative riserve teoriche su Nyquist/aliasing.

---

### GAP-D-04 — Distinzione rigorosa tra schema CAD R0.1 e cablaggio AS BUILT
- **Categoria:** D — Documentale
- **Descrizione:** Lo schema KiCad R0.1 documenta la configurazione di FASE 4 con due resistenze assiali da 10 kΩ (`R1` e `R2`) connesse a 3,3 V per forzare MD0 e MD1 a livello HIGH. Nei documenti R2 tale configurazione CAD veniva data per certa anche sul banco sperimentale AS BUILT. L'operatore ha tuttavia precisato di aver utilizzato una derivazione volante a Y e di non ricordare il valore della resistenza fisica effettivamente installata; inoltre, sul banco non è stata eseguita una misura ohmica del valore delle resistenze di pull-up.
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md` (§3.1, §4.2, §5.1), `TICKFORM_FULL_PROJECT_HANDOFF.md` (§3.1, §4.2, §5.1), `TICKFORM_DOCUMENTATION_AUDIT.md`.
- **Evidenza disponibile:** File sorgente KiCad R0.1 (`tickform_phase4_pcm1808c.kicad_sch`), README di KiCad R0.1, dichiarazioni dell'operatore nel mandato F5-D03 e R3.
- **Gravità:** MEDIA.
- **Causa accertata:** Sovrapposizione concettuale tra disegno CAD approvato per FASE 4 e cablaggio volante temporaneo su breadboard.
- **Correzione applicata in R3:**
  1. Distinzione netta tra:
     - *CAD (KiCad R0.1 approvato):* Due resistenze distinte da 10 kΩ `R1` e `R2` collegate al terminale `J1.2` (3.3V).
     - *Banco fisico sperimentale AS BUILT:* Livello logico HIGH accertato operativamente (l'ADC si è avviato in modalità Master 96 kHz sia in FASE 4 che in F5-D03), ma valore ohmico effettivo e topologia fisica (derivazione a Y con singola resistenza o due resistenze) **non verificati con misura strumentale diretta e rimasti non confermati**.
  2. Preservazione integrale dello schema KiCad R0.1 (che non deve essere modificato).
  3. Raccomandazione di fotografare il banco e rilevare la netlist AS BUILT con multimetro prima di procedere al futuro consolidamento hardware.
- **Necessità decisione OVERVIEW:** NO.
- **Necessità prova HW:** NO.
- **Criterio oggettivo di chiusura:** La matrice comparativa a tre colonne separa formalmente la colonna CAD dalla colonna Banco Fisico Reale.

---

### GAP-D-05 — Normalizzazione dei percorsi relativi nei documenti Markdown
- **Categoria:** D — Documentale
- **Descrizione:** Nei documenti R2 erano rimasti numerosi link con schema assoluto `file:///A:/Documenti/Tickform/...`, che ne limitavano la portabilità su host differenti o per altri agenti AI.
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md`, `TICKFORM_FULL_PROJECT_HANDOFF.md`.
- **Evidenza disponibile:** Rilievo della revisione indipendente ChatGPT R2 (§6) e Mandato R3 (§10).
- **Gravità:** BASSA.
- **Causa accertata:** Mancata conversione automatica di URL assoluti durante la stesura iniziale.
- **Correzione applicata in R3:** Sostituzione di tutti i collegamenti assoluti con link relativi rispetto alla cartella `docs/` (es. `../firmware/...`, `decisions/ADR-001...`, `./evidenze_f5_d03/...`), mantenendo il percorso assoluto Windows esclusivamente come indicazione informativa d'ambiente.
- **Necessità decisione OVERVIEW:** NO.
- **Necessità prova HW:** NO.
- **Criterio oggettivo di chiusura:** Nessun link di navigazione interna contiene prefissi `file:///A:/`.

---

### GAP-D-06 — Disallineamento cronologico dei documenti canonici storici
- **Categoria:** D — Documentale / A — Approvazione necessaria
- **Descrizione:** I quattro documenti canonici del repository (`PROJECT_STATE.md`, `PROJECT_LOG.md`, `CHAT_HANDOFF.md`, `BRINGUP_CHECKLIST.md`) sono fermi al commit `e857849debbc54e655a8a87a88c07a2ee0386d43` del 13/11/2026. Non contengono traccia degli sviluppi di FASE 5 (F5-A02, F5-D02, F5-D02-R2, F5-D03, verifiche del 17/11/2026).
- **Documenti coinvolti:** `docs/PROJECT_STATE.md`, `docs/PROJECT_LOG.md`, `docs/CHAT_HANDOFF.md`, `docs/bringup/BRINGUP_CHECKLIST.md`.
- **Evidenza disponibile:** Output di `git log -n 1 --stat e857849`, analisi del filesystem live.
- **Gravità:** ALTA.
- **Causa accertata:** Rigorosa applicazione della regola di governance che vieta all'agente di modificare i documenti canonici senza previa autorizzazione esplicita della supervisione.
- **Correzione proposta in R3:** Redazione di un documento dedicato (`PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md`) che formula le proposte puntuali, non distruttive e append-only per aggiornare i 4 documenti canonici solo dopo formale approvazione di CHAT OVERVIEW.
- **Necessità decisione OVERVIEW:** SÌ (per autorizzare l'applicazione delle modifiche).
- **Necessità prova HW:** NO.
- **Criterio oggettivo di chiusura:** Approvazione formale da parte di CHAT OVERVIEW e successivo aggiornamento (o commit) dei quattro documenti canonici.

---

### GAP-V-01 — Collaudo del firmware normale operativo F5-A02-R2.1
- **Categoria:** V — Verifica necessaria
- **Descrizione:** Il firmware approvato per la ricezione normale I²S2 Slave DMA (`tickform_phase5_i2s_rx.bin`, SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`) necessitava di un collaudo controllato a banco con log seriale salvato per certificare la qualifica nominale.
- **Documenti coinvolti:** `firmware/stm32g431/RAPPORTO_F5_A02_R2.md`, `BRINGUP_CHECKLIST.md` (FASE 5 checklist), `PROJECT_STATE.md`, `docs/evidenze_f5_n01_n03/`.
- **Evidenza disponibile:** File binario `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin`, 3 file di log originali recuperabili archiviati in `docs/evidenze_f5_n01_n03/` (`F5_N02_RESET.txt`, `F5_N02_R1_RESET.txt`, `F5_N03_POWER_CYCLE.txt`) e relativo manifest SHA-256. Per F5-N01 e F5-N01-R1, non essendovi file di log originali recuperabili, gli esiti provengono direttamente dai rapporti operativi trasmessi alla OVERVIEW.
- **Gravità:** ALTA.
- **Stato:** **RISOLTO (20/11/2026)**. Mandato CHAT OVERVIEW F5-N01/N02/N03 eseguito: flash riuscito al primo tentativo con verifica crittografica; collaudi nominali positivi (F5-N01-R1 PASS 3751 CB, F5-N02-R1 PASS 3751 CB, F5-N03 PASS 3750 CB; 0 errori). Preservati anche F5-N01 (FAIL originale per connettore Dupont scollegato a banco, collegamento esatto non identificato nella documentazione, privo di log salvato) e F5-N02 (eseguito ma non validabile per campioni azzerati). Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03; contenuto audio, formato e sincronismo non ancora validati.
- **Criterio oggettivo di chiusura:** Esecuzione del test a banco, verifica della ricezione nominale (~3750 CB DMA) con zero errori, salvataggio dei file di log nel repository con manifest.

---

### GAP-V-02 — Documentazione AS BUILT e verifica strumentale del cablaggio MD0/MD1 e alimentazioni
- **Categoria:** V — Verifica necessaria
- **Descrizione:** L'esatta configurazione fisica del cablaggio delle linee MD0 e MD1 sulla breadboard, le masse e l'oscillatore necessitavano di ricostruzione AS BUILT e verifica strumentale.
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md` (§3.1, §5.1), `PROJECT_STATE.md`.
- **Evidenza disponibile:** Conferme fornite dall'operatore a CHAT OVERVIEW in data 20/11/2026.
- **Gravità:** MEDIA.
- **Stato:** **AS BUILT CONFERMATO DA OPERATORE / MISURAZIONE STRUMENTALE PENDENTE (Demandata a Gate H01)**.
  - *Ricostruzione AS BUILT (confermata dall'operatore):* L'operatore ha confermato la configurazione fisica con derivazione a Y con una resistenza da 10 kΩ per ramo verso 3,3 V, coerente con lo schema KiCad (livello HIGH confermato operativamente); XO saldato su millefori con condensatore ceramico di disaccoppiamento da 100 nF; J1.3 GND connesso direttamente a GND ST-Link; DS3231 conforme (nessuna batteria CR2032 non ricaricabile).
  - *Verifica strumentale ohmica indipendente:* La misurazione ohmica con multimetro e il rilievo fotografico non sono stati effettuati a banco; restano previsti come controllo strumentale nel Gate H01 prima di FASE 6.
- **Criterio oggettivo di chiusura:** Esecuzione del controllo strumentale e documentazione fotografica/ohmica nel Gate H01.

---

### GAP-V-03 — Verifica strumentale della continuità del clock BCK a 6,144 MHz
- **Categoria:** V — Verifica necessaria
- **Descrizione:** Il campionamento GPIO IDR a ~1,9–2,1 MSps è insufficiente per attestare la forma d'onda e la frequenza esatta del clock seriale BCK (6,144 MHz nominali), producendo aliasing e transizioni variabili (da 0 a oltre 8000 transizioni a seconda delle condizioni).
- **Documenti coinvolti:** `TICKFORM_PROJECT_MEMORY.md` (§6.4, §8.A), `firmware/stm32g431/Core/Src/i2s_rx_diag.c`.
- **Evidenza disponibile:** Log F5-D03 (transizioni PB13 variabili: 0 in run 1 e 3; 7290 in run 2; 7552 in run 4; 8152 e 8139 in run 5A/5B), acquisizioni storiche PulseView di FASE 4 (compatibilità con 6,144 MHz).
- **Gravità:** MEDIA.
- **Causa accertata:** Limite intrinseco del campionamento GPIO via CPU polling rispetto alla frequenza di Nyquist.
- **Azione richiesta:** Se richiesto dalla supervisione per chiudere formalmente FASE 5, effettuare una cattura passiva con analizzatore logico a 24 MHz su PB13 durante la ricezione I2S attiva per documentare continuità e jitter di BCK.
- **Necessità decisione OVERVIEW:** SÌ (se la supervisione lo ritiene propedeutico a FASE 6).
- **Necessità prova HW:** SÌ.
- **Criterio oggettivo di chiusura:** Esportazione di un grafico/misura PulseView con BCK = 6,144 MHz stabili durante lo streaming.

---

### GAP-V-04 — Verifica di integrità del repository Git prima del consolidamento
- **Categoria:** V — Verifica necessaria
- **Descrizione:** Nel working tree locale di `A:\Documenti\Tickform` sono presenti 4 file tracciati modificati (`CMakeLists.txt`, `stm32g4xx_hal_conf.h`, `main.c`, `build.ps1`) e decine di file non tracciati (sorgenti I²S, rapporti di review, log, zip). Tali modifiche non sono ancora state consolidate in commit Git.
- **Documenti coinvolti:** Repository Git intero, `RAPPORTO_VERIFICA_GIT_R3.md`.
- **Evidenza disponibile:** Output di `git status --short` e `git diff --stat`.
- **Gravità:** BASSA (le modifiche sono protette e integre nel filesystem locale e documentate negli zip di review).
- **Causa accertata:** Rispetto rigoroso del vincolo operativo che vieta `git commit` e `git push` automatici.
- **Azione richiesta:** Mantenere lo stato attuale fino a conclusione della review R3. Al momento opportuno, sottoporre a CHAT OVERVIEW una proposta formale di commit di consolidamento FASE 5.
- **Necessità decisione OVERVIEW:** SÌ (al momento del commit).
- **Necessità prova HW:** NO.
- **Criterio oggettivo di chiusura:** Esecuzione controllata del commit di consolidamento autorizzato da OVERVIEW.

---

### GAP-A-01 — Autorizzazione formale per flash e test di F5-A02-R2.1
- **Categoria:** A — Approvazione necessaria
- **Descrizione:** La programmazione della STM32 e l'avvio del test di ricezione DMA con il firmware normale `tickform_phase5_i2s_rx.bin` (SHA-256 `04B95777...`) richiede autorizzazione esplicita.
- **Documenti coinvolti:** `firmware/stm32g431/build/phase5/tickform_phase5_i2s_rx.bin`, `firmware/stm32g431/RAPPORTO_F5_A02_R2.md`.
- **Evidenza disponibile:** Binario già compilato e validato crittograficamente; procedura di test definita.
- **Gravità:** ALTA.
- **Azione richiesta:** CHAT OVERVIEW valuta la presente revisione R3 e, se approvata, emette mandato operativo per programmare e collaudare F5-A02-R2.1.
- **Criterio oggettivo di chiusura:** Mandato formale di autorizzazione emesso dalla supervisione.

---

### GAP-A-02 — Strategia di stabilizzazione hardware e passaggio a KiCad R0.2
- **Categoria:** A — Approvazione necessaria
- **Descrizione:** Il prototipo attuale poggia su connessioni volanti (jumper Dupont su breadboard) sensibili a contatti intermittenti. Occorre stabilire se consolidare il circuito mediante perfboard saldata prima di FASE 6 o se procedere alle prime acquisizioni RAW sul banco attuale stabilizzato meccanicamente, e quando aggiornare lo schema KiCad alla revisione R0.2 (per includere PB12/13/15 e DS3231).
- **Documenti coinvolti:** `hardware/v0_perfboard/schematic/phase4_pcm1808c/`, `RAPPORTO_VERIFICA_KICAD_R3.md`.
- **Evidenza disponibile:** Analisi dei rischi di stallo da contatto volante; schema KiCad R0.1 integro.
- **Gravità:** MEDIA.
- **Azione richiesta:** CHAT OVERVIEW stabilisce se autorizzare la revisione dello schema KiCad a R0.2 prima o dopo il collaudo del firmware normale.
- **Criterio oggettivo di chiusura:** Decisione formale di CHAT OVERVIEW sulla roadmap hardware.

---

### GAP-A-03 — Chiusura formale di FASE 5 e autorizzazione a FASE 6 (RAW Stream)
- **Categoria:** A — Approvazione necessaria
- **Descrizione:** La FASE 6 (streaming continuo dei campioni RAW verso PC via USB nativa e archiviazione loss-less) è attualmente **NON INIZIATA e NON ANCORA AUTORIZZATA**. Non deve essere avviata alcuna implementazione di FASE 6 prima del completamento e dell'approvazione formale di tutti i gate di FASE 5.
- **Documenti coinvolti:** `docs/bringup/BRINGUP_CHECKLIST.md` (FASE 6), `docs/PROJECT_STATE.md`.
- **Evidenza disponibile:** Criteri di accettazione di FASE 5 parzialmente pendenti (manca collaudo F5-A02-R2.1).
- **Gravità:** CRITICA.
- **Azione richiesta:** Mantenere il blocco tassativo su FASE 6 fino a formale autorizzazione della supervisione.
- **Criterio oggettivo di chiusura:** Mandato esplicito di avvio FASE 6 emesso da CHAT OVERVIEW.

---

### GAP-A-04 — Approvazione della proposta di sincronizzazione dei documenti canonici
- **Categoria:** A — Approvazione necessaria
- **Descrizione:** L'aggiornamento dei file canonici `PROJECT_STATE.md`, `PROJECT_LOG.md`, `CHAT_HANDOFF.md` e `BRINGUP_CHECKLIST.md` è pronto in bozza non distruttiva nel documento `PROPOSTA_SINCRONIZZAZIONE_CANONICI_R3.md`.
- **Documenti coinvolti:** I 4 documenti canonici in `docs/`.
- **Evidenza disponibile:** Testo completo delle modifiche proposto in formato diff/append-only.
- **Gravità:** MEDIA.
- **Azione richiesta:** CHAT OVERVIEW esamina la proposta e autorizza l'applicazione materiale ai file canonici.
- **Criterio oggettivo di chiusura:** Autorizzazione esplicita all'applicazione.
