# TICKFORM — Revisione indipendente del pacchetto documentale R2

**Data:** 18 novembre 2026  
**Pacchetto ricevuto:** `tickform_audit_pack_20261118_R2.zip`  
**SHA-256 verificato:** `DB39BE192E18A5AAF0C6159E6F7E52EB37F0F548FEC3BD1E49FF4C6C0386FCA8`  
**Esito:** **R2 NON APPROVATA COME MEMORIA CANONICA; correzione mirata richiesta (R3).**  
**Perimetro:** revisione dei quattro Markdown R2 e confronto con i log Tera Term effettivamente allegati alla conversazione. Non è una verifica autonoma del filesystem locale Windows o dei sorgenti KiCad originali.

## 1. Risultati positivi effettivamente disponibili, ma non registrati in R2

La R2 (`TICKFORM_PROJECT_MEMORY.md`, §8; `TICKFORM_FULL_PROJECT_HANDOFF.md`, §8; `TICKFORM_DOCUMENTATION_AUDIT.md`, §2.3; `TABELLA_CORREZIONI_R2.md`, CORR-04) cita essenzialmente il log negativo delle 13:35:19 e trasforma i successivi test positivi in semplici dichiarazioni dell'operatore senza log. Questo è incompleto: nella conversazione sono stati condivisi i seguenti file Tera Term, disponibili come allegati e ora ricompresi nel pacchetto di evidenze `TICKFORM_F5D03_EVIDENZE_CHAT_20261118.zip`.

| Nome sorgente originale | Identificazione nel file | DMA Total CB / attesi ~3750 | Raw IRQ SPI2 | HAL I2S Error CB | Esito osservabile |
|---|---|---:|---:|---:|---|
| `Testo incollato(20261117-133519)(1).txt` | F5-D03 | 0 | 103 | 103 | Anomalo, precedente |
| `Testo incollato(20261117-220546).txt` | F5-D03 | 3751 | 0 | 0 | Regolare |
| `Testo incollato(20261117-222021).txt` | F5-D03 | 1122 | 518924 | 518924 | Anomalo |
| `Testo incollato(20261117-222542).txt` | F5-D03 | 3714 | 3567483 | 3567483 | Anomalo |
| `Testo incollato(20261117-224318).txt` | F5-D03 | 3750 e 3751 in due rapporti diversi | 0 e 0 | 0 e 0 | Due rapporti regolari distinti |

**Attenzione:** copie ripetute del medesimo rapporto all'interno di un log, con contatori identici, non rappresentano prove indipendenti: il firmware ritrasmette la diagnostica. L'ultimo file contiene invece due gruppi di contatori distinti (3750 e 3751), coerenti con due esecuzioni.

**Correzione richiesta:** separare esplicitamente: (A) evidenze digitali conservate come allegati ChatGPT ma non ancora archiviate nel repository; (B) eventuale file negativo già presente nel filesystem Windows; (C) i sei riavvii successivi senza log individuali. Non scrivere più che *tutte* le prove positive sono prive di log o che esiste un solo log F5-D03.

## 2. Firmware dei sei riavvii: attribuzione a F5-A02-R2.1 non dimostrata

In `TICKFORM_PROJECT_MEMORY.md` (§8.B, §9), `TICKFORM_FULL_PROJECT_HANDOFF.md` (§8.B, §14), `TICKFORM_DOCUMENTATION_AUDIT.md` (§2.3) e `TABELLA_CORREZIONI_R2.md` (CORR-04), Antigravity attribuisce il test positivo e i sei riavvii finali a `F5-A02-R2.1` / `tickform_phase5_i2s_rx`.

**Nella conversazione i log positivi sono esplicitamente intestati `DIAGNOSTICA F5-D03`** e non vi è evidenza che sia stato flashato il firmware F5-A02-R2.1 durante questi passaggi. L'utente ha riferito tre reset e tre power-cycle con LED finale non acceso, senza log conservati. Per continuità della sessione è plausibile che fosse ancora caricato F5-D03, ma **il binario effettivo dei sei reboot non è identificato da una nuova verifica hash/flash o da log individuali**.

**Correzione richiesta:** registrare `firmware identificato nei log salvati: F5-D03`; `firmware esatto dei sei reboot: non verificato autonomamente (presumibilmente F5-D03 per continuità del test)`. Non dichiarare un collaudo F5-A02-R2.1 avvenuto senza prova. Se la roadmap esige un collaudo separato col firmware normale, mantenerlo come prossimo gate da autorizzare/condurre.

## 3. Rappresentazione della ripetibilità dei sei reboot

L'osservazione reale dell'operatore è: **tre pressioni del tasto RESET corretto e tre power-cycle USB, tutti senza LED blu finale fisso**. Non esistono sei log Tera Term individuali. Questo supporta una **verifica pratica del comportamento LED e della ripetibilità apparente**, non certifica sei acquisizioni tutte con 3750 callback DMA ed errori zero. Scrivere così, senza livellare prove osservazionali e strumentali.

## 4. GPIO IDR: distinzione tra attività osservata e assenza di clock

Corretto sostituire la stima teorica ~11,8 MSps con i valori dei test reali. Ma non scegliere il solo esempio a 2067 kSps come se fosse l'unico: in diversi log si misurano circa **1908–2067 kSps (~1,9–2,1 MSps)**.

- **PB12/LRCK a 96 kHz:** la misura in una finestra da 10 ms con ~2 MSps è ragionevolmente indicativa delle transizioni del segnale durante quella finestra. Quando le transizioni sono zero, la linea è stata campionata senza transizioni nella finestra, non necessariamente è stata senza clock per tutti i dieci secondi.
- **PB13/BCK nominale 6,144 MHz:** la lettura software ~2 MSps è sotto Nyquist. Le **zero transizioni campionate non dimostrano definitivamente assenza fisica di clock**: l'aliasing o il sincronismo possono produrre un livello apparentemente costante. Servirebbe PulseView/strumento adeguato per una conclusione diretta.
- I log mostrano stati differenti in prove differenti: HIGH fisso nella sessione 13:35, LOW fisso in altre, numerose transizioni nei test regolari. Non fissare la memoria a un unico stato HIGH.
- Il codice F5-D03 esegue `run_pin_input_test()` **dopo** la procedura di stop DMA; non trattare automaticamente tale finestra come misura simultanea di tutti i 10 secondi di ricezione I2S.

## 5. Contatti del banco, modalità ADC e KiCad: distinguere CAD da fisico

È corretta la rettifica secondo cui il contatto specifico responsabile non è stato isolato. Tuttavia la R2 continua a descrivere il banco AS BUILT con **due pull-up R1/R2 da 10 kΩ su MD0/MD1** come dati accertati. Questi sono documentati nello **schema KiCad R0.1**, ma non risultano verificati in questa conversazione come i valori e la configurazione reali del banco: l'operatore ricorda una derivazione a Y e non ricordava il valore della resistenza. Inoltre le prove hanno coinvolto numerosi Dupont/Wago e i collegamenti sono stati ripremuti insieme.

**Correzione richiesta:** distinguere *CAD: due resistenze 10k* da *cablaggio fisico: livello HIGH osservato e topologia/valori da accertare*. Conservare una fotografia e una netlist AS BUILT prima di saldare. Non modificare il KiCad R0.1.

## 6. Link interni ai documenti: ancora non portabili

Nonostante la revisione precedente richiedesse link relativi, `TICKFORM_PROJECT_MEMORY.md` contiene numerosi collegamenti `file:///A:/Documenti/Tickform/...`; anche `TICKFORM_FULL_PROJECT_HANDOFF.md` contiene un link assoluto. Per l'obiettivo di memoria trasferibile ad altri agenti/computer, sostituire i link Markdown *inter-repository* con riferimenti relativi rispetto alla directory `docs/`. Il percorso originale Windows può rimanere come informazione di ambiente locale, non come unico modo per aprire gli allegati.

## 7. Stato della roadmap e autorizzazioni

Appropriata la rettifica: **FASE 6 NON INIZIATA, NON ANCORA AUTORIZZATA**. La sequenza recuperata è FASE 5 I2S/DMA → FASE 6 RAW→PC → FASE 7 Metrologia → FASE 8 AFE → FASE 9 Piezo → FASE 10 DSP.

Non attribuire approvazione formale a nuovi flash, al firmware normale F5-A02-R2.1, all'aggiornamento R0.2 KiCad o al commit soltanto perché sono raccomandati dai report. Per ogni attività futura, distinguere proposta, criterio già esistente, autorizzazione verificabile e operazione eseguita. In particolare, lo streaming RAW continuo deve rimanere fuori dai risultati validati F5-D03: finora è stato validato solo il trasporto I²S→DMA in condizioni di prova.

## 8. Esito e mandato minimo per R3

**Non serve ripetere l'audit completo.** Occorre una revisione R3 correttiva di `TICKFORM_PROJECT_MEMORY.md`, `TICKFORM_FULL_PROJECT_HANDOFF.md`, `TICKFORM_DOCUMENTATION_AUDIT.md` e `TABELLA_CORREZIONI_R2.md` oppure un change log R3 distinto.

1. Importare i log originali del presente pacchetto di evidenze in una posizione del repository concordata, preservandoli byte-per-byte e annotandone SHA-256.
2. Costruire un indice dei test distinti, senza contare duplicazioni CDC come riavvii.
3. Correggere firmware/timing/stato dei sei reboot e separare i sei reboot senza log dai rapporti F5-D03 conservati.
4. Correggere i limiti interpretativi della misura GPIO IDR e il riferimento a ~1,9–2,1 MSps.
5. Separare CAD 10 kΩ da AS BUILT non confermato; mantenere KiCad R0.1 intatto.
6. Convertire i link interni in relativi.
7. Non modificare sorgenti, binari, progetti KiCad, `PROJECT_LOG.md`, `PROJECT_STATE.md`, `BRINGUP_CHECKLIST.md` o Git senza autorizzazione.
8. Produrre una **R3 candidata**, un diff leggibile e un ZIP per review indipendente.

**Il valore della R2 resta elevato: la mappa del progetto e la roadmap sono sostanzialmente utilizzabili. Quello che manca è una registrazione fedele dell'intera evidenza F5-D03 e la correzione di alcuni riferimenti non dimostrati.**
