# Tickform — FASE 4 — Schema Elettrico CAD (PCM1808 C Bring-up)
## Schema KiCad R0.1 — Approvato da CHAT OVERVIEW (13/11/2026)

**Cartella progetto:** `hardware/v0_perfboard/schematic/phase4_pcm1808c/`
**Tool CAD:** KiCad 10 (versione `10.0.8`, motore ed ERC nativi verificati tramite `kicad-cli`)
**Stato revisione:** **APPROVATO TECNICAMENTE DA CHAT OVERVIEW** (13/11/2026)
**File di progetto nativi:**
- Progetto: [`tickform_phase4_pcm1808c.kicad_pro`](tickform_phase4_pcm1808c.kicad_pro)
- Foglio schema (A3 Landscape, Rev R0.1): [`tickform_phase4_pcm1808c.kicad_sch`](tickform_phase4_pcm1808c.kicad_sch)
- Libreria simboli locale autonoma: [`tickform_symbols.kicad_sym`](tickform_symbols.kicad_sym)
- Tabella librerie simboli: [`sym-lib-table`](sym-lib-table)
- Esportazione vettoriale PDF: [`tickform_phase4_pcm1808c.pdf`](tickform_phase4_pcm1808c.pdf)
- Esportazione vettoriale SVG: [`tickform_phase4_pcm1808c.svg`](tickform_phase4_pcm1808c.svg)
- Rapporto ERC nativo KiCad: [`erc_report.rpt`](erc_report.rpt)
- Netlist esportata: [`tickform_phase4_pcm1808c.net`](tickform_phase4_pcm1808c.net)
- Script generatore riproducibile e deterministico: [`generate_schematic.py`](generate_schematic.py)
- Pacchetto completo ZIP di review: [`tickform_phase4_pcm1808c_review_pack_R0.1.zip`](tickform_phase4_pcm1808c_review_pack_R0.1.zip)

---

## 1. Descrizione del Circuito e Distinzione delle Connessioni

Lo schema rappresenta fedelmente la configurazione reale di banco utilizzata per raggiungere e verificare la generazione del clock audio seriale **LRCK a 96,000 kHz** sul breakout **PCM1808 C**.

### A. WeAct Studio V1.0 STM32G431CBU6 Core Board (`U1`)
- **Orientamento fisico:** Connettore USB-C rivolto verso l'alto.
- **Fila header destra (posizioni identificate fisicamente dal banco):**
  - **1° pin (in alto):** `3V3` (uscita alimentazione digitale a 3,3 V dal regolatore LDO onboard).
  - **2° pin:** `GND` (massa comune di alimentazione e segnale).
  - **3° pin:** `B11` (`PB11`, non utilizzato in questa fase, contrassegnato con No-Connect).
  - **4° pin:** `V+` (uscita alimentazione a ~4,7 V – 5,0 V derivata da VBUS USB-C a valle del diodo Schottky onboard).
- **Alimentazione:** La WeAct è alimentata esclusivamente via cavo USB-C collegato al PC. Nessun programmatore o alimentatore esterno collegato.

### B. Oscillatore Attivo XO 24.576 MHz (`U2`) e Bypass Locale (`C1`)
- **Package:** Metal can DIP-14 a 4 pin.
- **Pin 1 (`OE`):** `OPEN` (non collegato, abilitazione interna attiva).
- **Pin 4 (`GND`):** Collegato alla massa comune `GND` tramite cavetto esterno da WeAct Pin 2.
- **Pin 5 (`OUT`):** Uscita clock 24,576 MHz, collegata direttamente al pin `SCK` del breakout PCM1808 C con filo diretto.
- **Pin 8 (`VCC`):** Alimentato a 3,3 V tramite cavetto esterno da WeAct Pin 1.
- **Bypass locale (`C1`, 100 nF):** Condensatore ceramico da **100 nF** collegato tra il Pin 8 (`VCC`) e il Pin 4 (`GND`) a ridosso dell'oscillatore.
  - *Classificazione documentale:* **Documentato storicamente in FASE 3** (`PROJECT_LOG.md#L180`), non nuovamente verificato a banco nel collaudo PCM1808 C.

### C. Modulo Breakout PCM1808 C (`MOD1`)
*Nota identificativa:* Modulo breakout commerciale generico per TI PCM1808. La marcatura `SKU:01325` non è stata confermata fisicamente sull'esemplare C e non viene utilizzata come identificativo accertato.

#### 1. Cavetti esterni realmente installati a banco
- **Alimentazione analogica:** `WeAct Header Pin 4 (V+)` → `Breakout Pin J1.1 (+5V)` (cavo volante diretto).
- **Alimentazione digitale:** `WeAct Header Pin 1 (3V3)` → `Breakout Pin J2.1 (3.3V)` (cavo volante diretto).
- **Massa modulo:** `WeAct Header Pin 2 (GND)` → `Breakout Pin J2.2 (GND_2)` (cavo volante diretto).
- **Clock Master SCK:** `XO Pin 5 (OUT)` → `Breakout Pin J2.3 (SCK)` (filo diretto, nessuna resistenza esterna).
- **Resistenze di Pull-Up Master Mode (96 kHz, 256 × Fs):**
  - Resistenza assiale da **10 kΩ** (`R1`) attestata fisicamente tra il terminale `J1.2 (3.3)` e `J1.4 (MD0)` → Livello logico **HIGH**.
  - Resistenza assiale da **10 kΩ** (`R2`) attestata fisicamente tra il terminale `J1.2 (3.3)` e `J1.5 (MD1)` → Livello logico **HIGH**.

#### 2. Terminali esterni lasciati aperti (OPEN / NC)
- **`J1.3 (GND)`:** **Nessun cavetto esterno installato.** Il terminale è rimasto non cablato all'esterno. Nello schema CAD è provvisto di No-Connect e annotato come `NC esterno`.
- **`J1.6 (FMT)`:** Lasciato **OPEN** all'esterno → Livello logico **LOW** garantito dal pull-down interno al chip (seleziona formato standard I2S 24-bit).
- **`J2.5 (OUT)`:** Lasciato **OPEN** all'esterno → Nessun cavetto verso la MCU STM32 (ricezione I2S non attiva in FASE 4).
- **`J3.1 (RIN), J3.2 (GND), J3.3 (LIN)`:** Connettore analogico tripolare lasciato **OPEN** all'esterno (stadio AFE non connesso).

#### 3. Collegamenti interni del breakout dimostrati mediante misura
- **Percorso `J2.3 (SCK) → Pin 6 (SCKI)`:** Rilevata mediante misura passiva con multimetro una resistenza serie onboard di circa **100 Ω** lungo la pista interna tra il terminale esterno `SCK` e il piedino 6 del chip PCM1808 TSSOP-14.
- **Continuità massa analogica su C:** Misurata a banco sull'esemplare C la resistenza tra `AGND chip pin 2` e `GND 2 breakout (J2.2)` pari a circa **0,2 Ω**.
  *Nota metodologica:* Questa misura è confermata specificamente per questo percorso; **non viene estesa automaticamente ad altre continuità interne non misurate su C**.

#### 4. Collegamenti interni del breakout ipotizzati / presunti su scheda C
- **Continuità 3,3 V (`J2.1 ↔ J1.2` e Pin 4 `VDD`):**
  - *Chiarimento storico:* La misura di continuità a 0 Ω tra i due terminali 3,3 V documentata in `PROJECT_LOG.md#L88` (`3.3V ↔ 3.3V continuity PASS`) è stata eseguita in FASE 0 **esclusivamente sugli esemplari PCM1808 #1 e #2**.
  - **Sulla scheda PCM1808 C**, la continuità interna tra `J2.1 (3.3V)` e `J1.2 (3.3)` **NON è stata misurata con multimetro prima del test**. Essa è **soltanto ipotizzata / presunta** per analogia costruttiva con i moduli #1 e #2. Nel collaudo di FASE 4 è risultata operativamente attiva (in quanto MD0 e MD1 sono andati a livello HIGH permettendo l'avvio a 96 kHz).
  - *Nel CAD:* Il terminale `J1.2` alimenta `R1` e `R2` sulla net equipotenziale `3V3`, corredato da nota esplicita di presunzione interna e assenza di ponticelli esterni.
- **Continuità GND (`J2.2 ↔ J1.3` e Pin 5 `DGND`):** Presunta per conformità di layout.
- **Mappatura terminali breakout ↔ piedini silicio TSSOP-14:** Presunta in base alla serigrafia del PCB e alle specifiche del datasheet ufficiale TI SLES177B.

### D. Strumentazione di Misura (`INSTR1`, Logic Analyzer 8CH)
- **Dispositivo:** Analizzatore logico USB 8 canali / 24 MHz nominali controllato tramite PulseView / sigrok.
- **Ruolo:** Strumento di sola acquisizione passiva ad alta impedenza. Non fornisce alimentazione al circuito.
- **Configurazione CAD e Netlist:** Il simbolo `INSTR1` è configurato con `in_bom no` e `on_board no`. **Compare nello schema grafico per documentare i punti di misura delle sonde, ma è intenzionalmente escluso dalla netlist PCB esportata (`tickform_phase4_pcm1808c.net`).** I suoi collegamenti sono verificati a banco e graficamente nello schema, ma non sono dichiarati verificati tramite la netlist PCB.
- **Massa:** Pin `GND` collegato alla massa comune `GND` tramite cavetto esterno su WeAct Pin 2.
- **Canali acquisiti:**
  - `CH1 / D0`: Collegato al pin `J2.4 (LRC)` del PCM1808 C tramite sonda LA (frequenza rilevata: **96,000 kHz tramite cursori PulseView**).
  - `CH2 / D1`: Collegato al pin `J2.6 (BCK)` del PCM1808 C tramite sonda LA (osservati circa 16 periodi in 2,625 µs, compatibile con **6,144 MHz nominali**; nessuna misura metrologica esatta a 6,144 MHz dichiarata).
- **Canali non utilizzati:** `CH3..CH8` contrassegnati con No-Connect.
- **Divieto esplicito su SCK:** Il segnale master clock SCK a 24,576 MHz **NON** è collegato all'analizzatore logico poiché supera la frequenza di Nyquist campionabile a 24 MS/s.

---

## 2. Matrice Completa delle Connessioni Elettriche (Rev R0.1)

| Rete / Nodo | Origine | Destinazione | Tipologia | Classificazione | Esito / Evidenza di Banco |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`+5V`** | WeAct Pin 4 (`V+`) | MOD1 `J1.1 (+5V)` | Cavo volante diretto | **Cavo esterno reale** | Misurati 5,0 V (rail analogico da USB PC) |
| **`3V3`** | WeAct Pin 1 (`3V3`) | MOD1 `J2.1 (3.3V)` | Cavo volante diretto | **Cavo esterno reale** | Misurati 3,2 V (rail digitale da LDO WeAct) |
| **`3V3`** | WeAct Pin 1 (`3V3`) | XO U2 Pin 8 (`VCC`) | Cavo volante diretto | **Cavo esterno reale** | Alimentazione oscillatore 24,576 MHz |
| **`3V3 (bypass)`** | XO U2 Pin 8 (`VCC`) | C1 Pin 1 (100 nF) | Terminale C1 | **Documentato FASE 3** | Non nuovamente verificato su C |
| **`3.3 (J1.2)`** | MOD1 Pin `J2.1` | MOD1 Pin `J1.2` | Pista PCB breakout | **Interno IPOTIZZATO su C** | Misurato 0 Ω solo su #1/#2 in F0; nessun cavo est. |
| **`3V3 (pull-up)`**| MOD1 `J1.2 (3.3)` | R1 Pin 1, R2 Pin 1 | Terminali resistenze | **Componente esterno reale**| Distribuzione 3,3 V ai pull-up MD0/MD1 |
| **`MD0`** | R1 Pin 2 (10 kΩ) | MOD1 `J1.4 (MD0)` | Terminale R1 | **Componente esterno reale**| Configurazione Master: livello HIGH |
| **`MD1`** | R2 Pin 2 (10 kΩ) | MOD1 `J1.5 (MD1)` | Terminale R2 | **Componente esterno reale**| Configurazione Master: livello HIGH |
| **`GND`** | WeAct Pin 2 (`GND`) | MOD1 `J2.2 (GND_2)` | Cavo volante diretto | **Cavo esterno reale** | Massa di riferimento modulo C |
| **`GND`** | WeAct Pin 2 (`GND`) | XO U2 Pin 4 (`GND`) | Cavo volante diretto | **Cavo esterno reale** | Massa oscillatore |
| **`GND`** | WeAct Pin 2 (`GND`) | INSTR1 Pin 1 (`GND`) | Cavo probe LA | **Cavo esterno reale** | Massa analizzatore logico (escluso da netlist) |
| **`GND (bypass)`** | XO U2 Pin 4 (`GND`) | C1 Pin 2 (100 nF) | Terminale C1 | **Documentato FASE 3** | Bypass XO; non nuovamente verificato su C |
| **`GND (J1.3)`** | MOD1 Pin `J2.2` | MOD1 Pin `J1.3` | Piano PCB breakout | **Interno IPOTIZZATO su C** | Misurato 0 Ω solo su #1/#2; nessun cavo est. su J1.3 |
| **`AGND interno`**| Chip Pin 2 (`AGND`)| MOD1 Pin `J2.2 (GND_2)`| Pista PCB breakout | **Interno DIMOSTRATO su C** | Misurati circa 0,2 Ω con multimetro su C |
| **`PCM_SCK`** | XO U2 Pin 5 (`OUT`) | MOD1 `J2.3 (SCK)` | **Filo diretto** (no R est.) | **Cavo esterno reale** | Master Clock 24,576 MHz ($256 \times F_s$) |
| **`SCK interna`**| MOD1 `J2.3 (SCK)` | Chip Pin 6 (`SCKI`) | Pista PCB + R serie | **Interno DIMOSTRATO** | R_onboard ~100 Ω accertata passivamente |
| **`PCM_LRC`** | MOD1 `J2.4 (LRC)` | INSTR1 Pin 2 (`CH1_D0`) | Sonda LA passiva | **Cavo esterno reale (misura)** | Rilevati **96,000 kHz tramite cursori PulseView** |
| **`PCM_BCK`** | MOD1 `J2.6 (BCK)` | INSTR1 Pin 3 (`CH2_D1`) | Sonda LA passiva | **Cavo esterno reale (misura)** | ~16 periodi in 2,625 µs (coerente con 6,144 MHz) |
| *NC / OPEN* | WeAct Pin 3 (`B11`)| — | Nessun cavo | **Aperto esterno** | GPIO B11 non utilizzato |
| *NC / OPEN* | XO U2 Pin 1 (`OE`) | — | Nessun cavo | **Aperto esterno** | Pull-up interno oscillatore |
| *NC / OPEN* | MOD1 `J1.3 (GND)` | — | Nessun cavo | **Aperto esterno** | Massa attestata solo su J2.2 |
| *NC / OPEN* | MOD1 `J1.6 (FMT)` | — | Nessun cavo | **Aperto esterno** | Pull-down interno chip a LOW |
| *NC / OPEN* | MOD1 `J2.5 (OUT)` | — | Nessun cavo | **Aperto esterno** | DOUT non collegato alla MCU |
| *NC / OPEN* | MOD1 `J3.1..J3.3` | — | Nessun cavo | **Aperto esterno** | Connettore analogico tripolare non connesso |
| *NC / OPEN* | INSTR1 `CH3..CH8` | — | Nessun cavo | **Aperto esterno** | Canali LA inutilizzati |

---

## 3. Mappatura Connettori Breakout ↔ Pin Chip Silicio (TI SLES177B)

Riferimento: **TI PCM1808 Datasheet SLES177B (Rev. Agosto 2015), Sez. 5, pag. 3**.

| Terminale Breakout | Serigrafia | Pin TSSOP-14 | Nome Pin Datasheet | Funzione da Datasheet Ufficiale SLES177B |
| :--- | :--- | :--- | :--- | :--- |
| *Non esposto* | — | **Pin 1** | `VREF` | Reference voltage decoupling (bypass SMD locale su scheda) |
| **J1.3 / J2.2 / J3.2** | `GND` | **Pin 2** | `AGND` | Analog ground (connesso a J2.2: ~0,2 Ω misurati su C) |
| **J1.1** | `+5V` | **Pin 3** | `VCC` | Analog power supply (+5 V) |
| **J1.2 / J2.1** | `3.3` / `3.3V`| **Pin 4** | `VDD` | Digital power supply (+3.3 V) |
| **J1.3 / J2.2** | `GND` | **Pin 5** | `DGND` | Digital ground |
| **J2.3** | `SCK` | **Pin 6** | `SCKI` | System clock input; 256/384/512 fS (tramite R_onboard ~100 Ω) |
| **J2.4** | `LRC` | **Pin 7** | `LRCK` | Sampling clock input/output (Word Clock) |
| **J2.6** | `BCK` | **Pin 8** | `BCK` | Audio data bit clock input/output |
| **J2.5** | `OUT` | **Pin 9** | `DOUT` | Audio data digital output |
| **J1.4** | `MD0` | **Pin 10** | `MD0` | Mode control input 0 |
| **J1.5** | `MD1` | **Pin 11** | `MD1` | Mode control input 1 |
| **J1.6** | `FMT` | **Pin 12** | `FMT` | Audio data format select input |
| **J3.3** | `LIN` | **Pin 13** | `VINL` | Analog input, left channel |
| **J3.1** | `RIN` | **Pin 14** | `VINR` | Analog input, right channel |

---

## 4. Script Generatore Portabile e Deterministico (`generate_schematic.py`)

Lo script di generazione è stato strutturato per garantire piena portabilità e riproducibilità:
- **Percorsi relativi:** Nessun percorso assoluto codificato (`OUTPUT_DIR = os.path.dirname(...)`).
- **UUID deterministici:** Tutti gli identificatori univoci UUID di componenti, pin, fili, testi e giunzioni sono calcolati tramite `uuid.uuid5` con namespace e chiavi semantiche stabili. L'esecuzione ripetuta dello script genera file byte-per-byte identici, azzerando le differenze spurie in Git.

---

## 5. Rapporto ERC e Netlist KiCad

1. **Electrical Rules Check (KiCad CLI 10.0.8):**
   - Violazioni: `0`
   - Errori: `0`
   - Avvisi: `0`
2. **Netlist PCB (`tickform_phase4_pcm1808c.net`):**
   - Le reti elettriche del circuito permanente (+5V, 3V3, GND, MD0, MD1, PCM_SCK, PCM_LRC, PCM_BCK) risultano coerenti e prive di floating nets.
   - Lo strumento esterno `INSTR1` non compare nella netlist in virtù della proprietà `on_board no`.
