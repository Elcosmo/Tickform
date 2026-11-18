# TICKFORM — RAPPORTO DI VERIFICA KICAD E CABLAGGIO REALE (R3)

**Data:** 18 novembre 2026  
**Autore:** Antigravity (Revisore Tecnico e Archivista di Progetto)  
**Supervisione:** CHAT OVERVIEW  
**Cartella Progetto KiCad:** `hardware/v0_perfboard/schematic/phase4_pcm1808c/`  
**Modalità:** ISPEZIONE IN SOLA LETTURA (Nessuna modifica a file KiCad, netlist o librerie)

---

## 1. Stato di Integrità del Progetto KiCad 10 (Rev R0.1)

L'audit non distruttivo condotto sui sorgenti CAD presenti nel repository ha accertato che il progetto approvato di FASE 4 è **perfettamente integro e non ha subito alcuna modifica**:

| File di Progetto KiCad | Dimensione | SHA-256 Verificato | Stato di Integrità Git |
| :--- | :---: | :---: | :--- |
| `tickform_phase4_pcm1808c.kicad_pro` | 601 B | `7702AFD87285B34898E9BB9979513A1ACA0A757D088CC6EA54111127E6A1760D` | Tracciato, **UNMODIFIED** |
| `tickform_phase4_pcm1808c.kicad_sch` | 47.247 B | `16AB73A3F589AB0048E0728C1CEEF54BBF4EC040E4CC98904532B22F60E192C8` | Tracciato, **UNMODIFIED** |
| `tickform_symbols.kicad_sym` | 12.872 B | `E0758BD5B811ADB90A3D4141587D6428D1C71D1EAE536C165ABEC84D1BEF7D91` | Tracciato, **UNMODIFIED** |
| `sym-lib-table` | 181 B | `62EBBED9093E2D378AB9FD06B1107A8051973A70E4FD2A64130F56196AB79622` | Tracciato, **UNMODIFIED** |
| `tickform_phase4_pcm1808c.net` | 18.904 B | `C685CF972A4F24ED3B058A4A303312CCBB83810DC116B7063FFF0C824D4344D8` | Tracciato, **UNMODIFIED** |
| `tickform_phase4_pcm1808c.pdf` | 287.019 B | `F0FE53A25413717E3427D69B85CB456671C53D5E6156445D3D92191E6ADB03A9` | Tracciato, **UNMODIFIED** |
| `tickform_phase4_pcm1808c.svg` | 2.140.538 B | `85A65888D7D074A4E5C820B0A27CFA89587C5D71DFDA70A38ED8F585CDD97A1B` | Tracciato, **UNMODIFIED** |
| `erc_report.rpt` | 357 B | `082BA9601B162FD2497EB67D5B56DF0B31FECE74ED5A63413A4FB393F14A7F8F` | Tracciato, **UNMODIFIED** |
| `README.md` | 14.262 B | `690BC291974F18A25F3A6BB59E740467A5EB6FAA00C2951663DC5672481EEEE2` | Tracciato, **UNMODIFIED** |
| `generate_schematic.py` | 47.297 B | `DBE023A5E5F10C656282E7A8AE9CE3B199FA26C8457E8AF2726C44C9AD7024F8` | Tracciato, **UNMODIFIED** |
| `tickform_phase4_pcm1808c.kicad_prl` | 2.311 B | `5620112745B13A6098F8AC7680CE482220E148210BE7FE75B2EA115D85CD2CA2` | Untracked (config locale KiCad) |
| `tickform_phase4_pcm1808c_review_pack_R0.1.zip` | 454.184 B | `2FAD45746D67117B361650529DC8807D401691F46AACBA734E3519E637F09AC9` | Untracked (pacchetto review R0.1) |

- **Verifica ERC nativa KiCad:** `0 Errors, 0 Warnings` (convalidata in FASE 4 tramite `kicad-cli 10.0.8`).
- **Nessuna modifica:** Il working tree Git per la directory `hardware/v0_perfboard/schematic/phase4_pcm1808c` non presenta alcun file modificato.

---

## 2. Perimetro e Limiti dello Schema KiCad R0.1

È essenziale ribadire il perimetro documentale dello schema R0.1:
1. **Riferito a FASE 4:** Lo schema documenta esclusivamente il bring-up del generatore di clock audio su modulo **PCM1808 C** con oscillatore attivo a 24,576 MHz.
2. **I²S e STM32 non connessi nel CAD:** Nello schema R0.1 i pin audio del breakout PCM1808 C (`LRC` J2.4, `BCK` J2.6) sono collegati esclusivamente ai canali di misura passiva dell'analizzatore logico (`INSTR1`). Il pin dati `OUT` (J2.5) e l'ingresso analogico `J3` sono lasciati aperti (OPEN / NC). **Non vi è alcun collegamento disegnato verso i pin I²S della STM32 (PB12, PB13, PB15).**
3. **DS3231 assente:** Il modulo RTC Maxim DS3231 (validato in FASE 2) non fa parte dello schema R0.1.
4. **Analizzatore logico escluso dalla netlist:** Il simbolo `INSTR1` è marcato `in_bom no` e `on_board no`; serve solo a documentare graficamente dove erano collegate le sonde durante il test di laboratorio a 96 kHz.

---

## 3. Matrice Comparativa a Tre Colonne

La seguente matrice confronta rigorosamente le tre realtà tecniche del progetto per evitare sovrapposizioni tra CAD, hardware costruito in FASE 4 e banco volante impiegato in FASE 5 (F5-D03):

| Sottosistema / Linea | (A) Schema KiCad R0.1 Approvato | (B) Prototipo Fisico Costruito (FASE 4) | (C) Configurazione Sperimentale Banco F5-D03 |
| :--- | :--- | :--- | :--- |
| **Alimentazione Analogica +5V** | WeAct Pin 4 (`V+`) → Breakout Pin `J1.1 (+5V)` | Cavo volante diretto WeAct V+ → J1.1 (+5V). Misurati 5,0 V. | Identica (WeAct V+ → J1.1). |
| **Alimentazione Digitale 3.3V** | WeAct Pin 1 (`3V3`) → Breakout Pin `J2.1 (3.3V)` | Cavo volante diretto WeAct 3V3 → J2.1 (3.3V). Misurati 3,29 V. | Identica (WeAct 3V3 → J2.1). |
| **Massa Comune GND** | WeAct Pin 2 (`GND`) → Breakout Pin `J2.2 (GND_2)` | Cavo volante diretto WeAct GND → J2.2. Massa comune. | Identica (WeAct GND → J2.2). |
| **Terminale J1.3 (GND)** | Aperto all'esterno (NC) con nota presunzione interna. | Nessun cavetto esterno installato su J1.3. | Lasciato aperto all'esterno. |
| **XO 24.576 MHz Supply & Bypass**| VCC a 3,3 V da WeAct; GND a massa; C1 100 nF bypass locale. | C1 (100 nF) documentato storicamente in FASE 3, non riesaminato su C. | Stesso montaggio breadboard con C1 bypass. |
| **Master Clock SCK** | XO Pin 5 (`OUT`) → Breakout Pin `J2.3 (SCK)` via filo diretto. | Filo diretto senza resistenze esterne. R_onboard ~100 Ω accertata. | Identica (XO OUT → J2.3). |
| **Pull-Up MD0 / MD1** | **Due resistenze distinte da 10 kΩ (`R1`, `R2`)** tra `J1.2` e MD0/MD1. | Livello HIGH presente (avvio 96 kHz). Topologia e valori non riesaminati. | **Derivazione a Y; resistenze fisiche NON misurate a banco con multimetro; livello HIGH accertato.** |
| **Formato FMT (J1.6)** | Lasciato OPEN (pull-down interno chip seleziona I2S standard). | Aperto all'esterno. Livello logico LOW confermato. | Lasciato aperto all'esterno. |
| **Word Clock (LRCK / WS)** | `J2.4 (LRC)` → Sonda Logic Analyzer `INSTR1 CH1/D0`. | Sonda LA collegata. Misurati 96,000 kHz con cursori PulseView. | **Cavetto Dupont volante da `J2.4` al pin `PB12` della WeAct.** |
| **Bit Clock (BCK)** | `J2.6 (BCK)` → Sonda Logic Analyzer `INSTR1 CH2/D1`. | Sonda LA collegata. Osservati 16 periodi in 2,625 µs (~6,144 MHz). | **Cavetto Dupont volante da `J2.6` al pin `PB13` della WeAct.** |
| **Data Output (DOUT / SD)** | `J2.5 (OUT)` lasciato **OPEN / NC**. | Aperto all'esterno (ricezione non collaudata in FASE 4). | **Cavetto Dupont volante da `J2.5` al pin `PB15` della WeAct.** |
| **Modulo DS3231 RTC** | **Non presente** nello schema KiCad R0.1. | Standalone in FASE 2 (I2C1 su PA15/PB7; SQW 1 Hz). Non su perfboard. | Collegato a banco (I2C1 + SQW) per futura integrazione metrologica. |
| **Stadio Audio AFE (J3)** | Ingressi analogici `J3.1..J3.3` lasciati **OPEN / NC**. | Connettore tripolare aperto. Nessun segnale collegato. | Lasciato aperto (non connesso). |

---

## 4. Valutazione dei Rischi dei Collegamenti Sperimentali su Breadboard

L'analisi dell'incidente di stallo hardware del 17/11/2026 e l'ispezione della configurazione a banco evidenziano le seguenti criticità:

1. **Vulnerabilità meccanica ed elasticità dei contatti:** I cavetti jumper Dupont maschio-femmina su breadboard presentano forze di ritenzione deboli e soggette a usura o ossidazione. Un micro-spostamento può causare apertura momentanea dei pin di configurazione (MD0/MD1) o disconnessione dei clock ad alta frequenza (BCK a 6,144 MHz).
2. **Degrado dell'integrità del segnale (Signal Integrity):** L'assenza di un piano di massa continuo sotto i percorsi dei clock veloci (SCK a 24,576 MHz e BCK a 6,144 MHz), combinata con le induttanze parassite dei cavetti volanti (lunghezza tipica 10–20 cm), genera ringing, ground bounce e potenziale sovra-oscillazione.
3. **Rischio di stallo e interrupt flood:** Quando le linee I²S fluttuano a causa di falsi contatti, la periferica SPI2/I²S2 della MCU può generare centinaia di migliaia o milioni di interrupt spuri (come evidenziato nei run anomali 3 e 4 di F5-D03, con oltre 3,5 milioni di IRQ), saturando il bus e la CPU.

---

## 5. Piano di Consolidamento Hardware e Roadmap CAD

Per passare in modo sicuro dal banco sperimentale breadboard a un prototipo solido e pronto per FASE 6, si raccomanda la seguente sequenza metodologica:

### Passo 1 — Rilievo strumentale AS BUILT (GAP-V-02)
- Prima di qualsiasi modifica, fotografare ad alta risoluzione l'intero banco di lavoro.
- Eseguire a circuito spento la misura con multimetro digitale delle resistenze di pull-up su MD0 e MD1 per annotarne il valore ohmico effettivo e confermare la topologia a Y.
- Verificare la continuità di tutte le masse verso la WeAct.

### Passo 2 — Collaudo del Firmware Normale sul banco stabilizzato (GAP-V-01)
- Convalidare la ricezione I²S con il firmware normale `tickform_phase5_i2s_rx.bin` (previa autorizzazione di CHAT OVERVIEW) sul banco volante attualmente funzionante, salvando il log formale.

### Passo 3 — Redazione dello Schema KiCad Revisione R0.2 (GAP-A-02)
- Aggiornare formalmente il progetto KiCad alla **Revisione R0.2**:
  - Aggiungere le connessioni digitali tra PCM1808 C e WeAct: `J2.4 (LRC)` → `PB12 (WS)`, `J2.6 (BCK)` → `PB13 (CK)`, `J2.5 (OUT)` → `PB15 (SD)`.
  - Inserire il modulo RTC Maxim DS3231 con le connessioni I2C1 (`PA15=SCL`, `PB7=SDA`) e il segnale `SQW` verso il timer hardware (preparazione a FASE 7).
  - Documentare fedelmente la rete di pull-up MD0/MD1 verificata al Passo 1.
  - Eseguire la verifica nativa ERC (`0 errors, 0 warnings`) ed esportare PDF, SVG e netlist.

### Passo 4 — Consolidamento su Perfboard Saldata
- Solo dopo approvazione formale dello schema R0.2 da parte di CHAT OVERVIEW, realizzare il cablaggio definitivo mediante saldature su perfboard (mantenendo header femmina per i moduli intercambiabili e test point per le sonde dell'analizzatore logico).
- **Divieto operativo:** Nessuna saldatura o modifica irreversibile deve essere effettuata prima dell'approvazione del mandato di consolidamento.
