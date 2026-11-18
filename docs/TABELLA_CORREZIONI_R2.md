# TICKFORM — TABELLA DELLE CORREZIONI REVISIONE CANDIDATA R2
**Confronto puntuale tra osservazioni della Review Indipendente e correzioni applicate**  
**Data:** 18 Novembre 2026  
**Stato:** REVISIONE CANDIDATA R2 — In attesa di approvazione della supervisione (CHAT OVERVIEW)  
**Ambito:** `docs/TICKFORM_PROJECT_MEMORY.md`, `docs/TICKFORM_FULL_PROJECT_HANDOFF.md`, `docs/TICKFORM_DOCUMENTATION_AUDIT.md`

---

## 1. Matrice Dettagliata delle Correzioni Applicate

| ID | Oggetto / Tema | Testo / Asserzione in Revisione R1 | Evidenza Reale del Repository / Banco | Rettifica Applicata in Revisione Candidata R2 |
| :--- | :--- | :--- | :--- | :--- |
| **CORR-01** | **Causa guasto modulo PCM1808 B** | Dichiarata in Sez. 12 come "possibile difetto di saldatura SMD del chip TSSOP-14 o componente difettoso all'origine". | `docs/PROJECT_LOG.md#L209` e `PROJECT_STATE.md#L39` registrano formalmente la causa come tassativamente **SCONOSCIUTA**. Nessuna dissaldatura o indagine microscopica è stata eseguita. | Eliminata ogni speculazione non dimostrata. Confermato formalmente che la causa del guasto del modulo B è **TOTALMENTE SCONOSCIUTA e NON DETERMINATA**. |
| **CORR-02** | **Causa blocco iniziale FASE 5 (17/11/2026)** | Dichiarata come certa: "Stallo del master clock e DMA fermo per falso contatto su MD0/MD1: I pin erano rimasti flottanti a causa di contatti imperfetti dei reofori delle resistenze". | Il mandato di audit (Sezione 6) attesta: *"Non è stato individuato con certezza il singolo contatto responsabile. Ripremendo i collegamenti fisici, il comportamento è tornato regolare."* | Rettificato rigorosamente: l'esatto contatto meccanico difettoso **NON è stato individuato con certezza**. L'operatore ha ripremuto l'insieme dei cablaggi fisici a banco (resistenze di modo, alimentazioni, clock). L'attribuzione specifica a MD0/MD1 è classificata come **ipotesi di lavoro dell'operatore**, non fatto provato. |
| **CORR-03** | **Prestazioni campionamento GPIO IDR (F5-D03)** | Dichiarata frequenza di campionamento loop IDR di "~11,8 MSps (periodo ~85 ns)". | Il file di log reale di F5-D03 (`Testo incollato(20261117-133519).txt`, riga 63) attesta: `Finestra misura: 10000 us (campioni totali: 20679)`, `Freq. campionamento: 2067 kSps (~2.0 MSps)`. Il valore 11.8 MSps era solo una stima teorica a priori in `RAPPORTO_F5_D03.md#L47`. | Rettificato il dato metrologico: la frequenza reale misurata su hardware è di **2.067 kSps (~2,07 MSps)** con 20.679 iterazioni in 10.000 µs (periodo di campionamento ~483 ns). Riconciliata l'analisi di Nyquist con il valore effettivo. |
| **CORR-04** | **Stato dei log di FASE 5 (Fuori repo vs Assenti)** | Dichiarato genericamente che "non esiste alcun file di log delle prove del 17/11/2026". | Esiste una netta distinzione tra due sessioni di prova: 1) Il log F5-D03 **esiste** ed è conservato fuori dal repo (`Downloads/Testo incollato(20261117-133519).txt`), attestante il dump a 6 parti di `tickform_phase5_diag`; 2) I 6 riavvii successivi di F5-A02-R2.1 (3 reset + 3 power cycle) sono **completamente privi di log archiviati** sia dentro che fuori dal repo. | Separati nettamente i due casi: documentata l'esistenza e il contenuto del log F5-D03 fuori dal repo, e specificata l'assenza totale di log per la successiva serie dei 6 riavvii di F5-A02-R2.1 (fondati unicamente sull'osservazione visiva dell'operatore). |
| **CORR-05** | **Separazione metodologica Dati vs Dichiarazioni vs Ipotesi** | Presentati in alcuni paragrafi risultati sperimentali e dichiarazioni dell'operatore con lo stesso livello di assertività. | Principio metrologico di TICKFORM: un dato senza log o evidenza strumentale verificabile non può essere equiparato a un collaudo strumentale congelato. | Introdotta in tutti e tre i documenti una rigida tripartizione: 1) **Dati misurati strumentalmente con evidenza/log**; 2) **Dichiarazioni dell'operatore non accompagnate da log archiviati**; 3) **Ipotesi e presunzioni tecniche non verificate**. |
| **CORR-06** | **Stato di approvazione di FASE 6** | Nella Sez. 14 dell'handoff R1 si parlava di "Prossimi passi già approvati: Avvio FASE 6 (RAW $\rightarrow$ PC Streaming)". | CHAT OVERVIEW non ha ancora autorizzato formalmente l'inizio della FASE 6. La FASE 5 è ancora formalmente IN CORSO / COMPLETATA CON RISERVE. | Rettificato: FASE 6 è classificata come **NON INIZIATA e NON ANCORA AUTORIZZATA**. I prossimi passi approvati riguardano esclusivamente il consolidamento formale di FASE 5 (archiviazione log F5-A02-R2.1 ed eventuale revisione R0.2 dello schema KiCad). |
| **CORR-07** | **Integrità e perimetro KiCad R0.1** | Necessità di ribadire che lo schema KiCad non deve essere alterato prima di esplicito mandato. | Lo schema KiCad R0.1 è approvato per la sola FASE 4. | Confermato che lo schema KiCad R0.1 rimane intatto e congelato al 100%. Le linee I2S verso STM32 (PB12/PB13/PB15) e il DS3231 rimangono classificati come cablaggi sperimentali di banco da integrare solo nella futura Rev R0.2. |
| **CORR-08** | **Proposta di sincronizzazione documentale permanente** | In R1 mancava una procedura concreta per aggiornare i documenti canonici senza sovrascritture o conflitti. | Necessità di sincronizzare `PROJECT_STATE.md`, `PROJECT_LOG.md`, `BRINGUP_CHECKLIST.md` e `TICKFORM_PROJECT_MEMORY.md`. | Formalizzata una proposta di sincronizzazione in 4 punti chiari, senza modifiche distruttive ai documenti storici (v. Sezione 2). |

---

## 2. Proposta di Sincronizzazione dei Documenti Canonici

Per recepire gli sviluppi di FASE 5 senza creare ridondanze, discrepanze o conflitti con la storia pregressa, si propone la seguente ripartizione dei ruoli documentali:

1. **`docs/PROJECT_LOG.md` (Registro Storico Append-Only):**
   - *Regola:* Nessuna modifica alle voci storiche (da M0 a FASE 4).
   - *Aggiunta proposta in coda:* Nuova sezione datata `2026-11-17 — FASE 5: Risoluzione Silent Drop CDC, indagine registri F5-D03 e sblocco DMA I2S`.
   - *Contenuto:* Registrare la cronologia dei mandati F5-A02-R2.1, F5-D02-R2, F5-D03; documentare il log esterno F5-D03 (stallo iniziale con PB12/13 HIGH a 2 MSps, 103 IRQ SPI2); documentare il successivo sblocco meccanico del banco a seguito della pressione dei cablaggi e le 6 prove positive dichiarate dall'operatore (~3750 CB/10s) prive di log salvati.

2. **`docs/PROJECT_STATE.md` (Fotografia dello Stato Vivo):**
   - *Regola:* Mantenere il documento sintetico e aggiornato all'ultima seduta di laboratorio, evitando di trasformarlo in una trattazione enciclopedica.
   - *Aggiornamento proposto:* Portare la data al `18/11/2026`; impostare `Current milestone: FASE 5 — PCM1808 -> STM32 (IN CORSO / COMPLETATA CON RISERVE)`; aggiornare lo stato dei Gate (Gate 5 PASS, Gate 6 Ricezione I2S/DMA PASS funzionale con riserva di log, Gate 7 Conversione audio reale PENDING); inserire link esplicito a `TICKFORM_PROJECT_MEMORY.md` per l'architettura dettagliata.

3. **`docs/bringup/BRINGUP_CHECKLIST.md` (Checklist Operativa dei Gate):**
   - *Regola:* Spuntare le caselle unicamente a fronte di prove stabili e riproducibili.
   - *Aggiornamento proposto:* Spuntare `STM32 receives I2S` e `DMA running` con data 17/11/2026 e nota metodologica. Mantenere espressamente NON spuntate le voci `DMA stability verified` (richiede prova estesa con log formale), `Continuous 64-bit sample counter implemented` e `Overflow/error handling implemented` (demandate alla gestione dello streaming in FASE 6).

4. **`docs/TICKFORM_PROJECT_MEMORY.md` (Memoria Centrale Persistente):**
   - *Ruolo:* Singola fonte di verità omnicomprensiva e strutturata, aggiornata a ogni milestone, contenente l'analisi ingegneristica completa, i riferimenti incrociati e la matrice di tracciabilità.
