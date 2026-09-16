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
