# TICKFORM — FASE 5
## Rapporto F5-D02 — Diagnostica interna SPI2/I²S2 → DMAMUX → DMA1
**Data:** 15/11/2026  
**Riferimento Mandato:** Mandato CHAT OVERVIEW F5-D02  
**Commit Baseline:** `e857849debbc54e655a8a87a88c07a2ee0386d43`  
**Progetto Firmware:** `firmware/stm32g431/`

---

### 1. Contesto e Obiettivo della Diagnostica

A valle della milestone F5-D01-R1, che ha confermato strumentalmente la presenza a banco dei segnali di clock generati dal modulo PCM1808 C (LRCK = 96,000 kHz, BCK = 6,144 MHz compatibile), l'esecuzione del firmware F5-A02-R2.1 ha registrato 0 callback DMA al termine della finestra di 10 secondi.

Il presente mandato implementa una variante software diagnostica temporanea (`tickform_phase5_diag`) che consente di indagare e discriminare in modo oggettivo e non invasivo:
1. Periferica SPI2/I²S2 non abilitata o non operativa a livello registri.
2. Mancato riconoscimento dei clock sul lato MCU (assenza di transizioni/dati sul registro di stato SPI2).
3. Mancato inoltro delle richieste di ricezione `SPI2_RX` attraverso DMAMUX1 verso DMA1 Channel 1.
4. DMA attivo e trasferimenti in memoria eseguiti, ma mancata invocazione/mascheramento delle interrupt/callback.

---

### 2. Architettura della Variante Diagnostica F5-D02

La variante è implementata nei file dedicati [i2s_rx_diag.h](file:///A:/Documenti/Tickform/firmware/stm32g431/Core/Inc/i2s_rx_diag.h) e [i2s_rx_diag.c](file:///A:/Documenti/Tickform/firmware/stm32g431/Core/Src/i2s_rx_diag.c), preservando al 100% byte-for-byte il firmware di produzione FASE 3 e il firmware approvato F5-A02-R2.1.

#### 2.1. Snapshot dei Registri Hardware
Il firmware registra quattro istantanee complete (`i2s_diag_snapshot_t`):
- `INIT` (t = 0 ms): immediatamente dopo l'avvio di `HAL_I2S_Receive_DMA`.
- `MID` (t ~ 5000 ms): a metà della finestra di esecuzione.
- `PRE-STOP` (t ~ 10000 ms): immediatamente prima dell'invocazione di `HAL_I2S_DMAStop()`.
- `POST-STOP`: dopo l'arresto controllato e lo svuotamento del registro DR.

Ciascuno snapshot acquisisce:
- **SPI2 / I²S2:**
  - `I2SCFGR`: `I2SE` (bit 11), `I2SMOD` (bit 10), `I2SCFG` (bits 9:8), `I2SSTD` (bits 5:4), `DATLEN` (bits 2:1), `CHLEN` (bit 0).
  - `CR2`: `RXDMAEN` (bit 0), `ERRIE` (bit 5).
  - `SR`: `RXNE` (bit 0), `TXE` (bit 1), `CHSIDE` (bit 2), `OVR` (bit 6), `BSY` (bit 7), `FRE` (bit 8), `FRLVL` (bits 10:9).
- **DMA1 Channel 1:**
  - `CCR`: `EN` (bit 0), `TCIE` (bit 1), `HTIE` (bit 2), `TEIE` (bit 3), `CIRC` (bit 5), `MINC` (bit 7), `PSIZE` (bits 9:8), `MSIZE` (bits 11:10), `PL` (bits 13:12).
  - `CNDTR`: valore del contatore di trasferimenti rimanenti (iniziale 2048).
  - `DMA1->ISR`: `GIF1` (bit 0), `TCIF1` (bit 1), `HTIF1` (bit 2), `TEIF1` (bit 3).
- **DMAMUX1 Channel 0:**
  - `DMAMUX1_Channel0->CCR`: campo `DMAREQ_ID` (bits 6:0), confrontato con `12U` (`DMA_REQUEST_SPI2_RX`).
- **GPIO Port B e Clock:**
  - `GPIOB->MODER` e `GPIOB->AFR[1]` (AFRH): configurazione modalità AF e AF5 per PB12 (`I2S2_WS`), PB13 (`I2S2_CK`), PB15 (`I2S2_SD`).
  - `RCC->APB1ENR1` (bit `SPI2EN`), `RCC->AHB1ENR` (bits `DMA1EN`, `DMAMUX1EN`), `RCC->AHB2ENR` (bit `GPIOBEN`).
- **Interrupt e NVIC:**
  - `NVIC_GetEnableIRQ(DMA1_Channel1_IRQn)` e `NVIC_GetEnableIRQ(SPI2_IRQn)`.
  - Contatori grezzi d'ingresso (`raw_irq_dma_count`, `raw_irq_spi_count`) incrementati in testa agli IRQ handler prima di inoltrare la chiamata ai gestori HAL.

#### 2.2. Modalità di Misura e Rispetto dei Vincoli Metrologici
- **Nessuna lettura di `SPI2->DR` durante la prova:** Il firmware non esegue letture su `SPI2->DR` durante la finestra di acquisizione. È stata rimossa dal polling la macro `__HAL_I2S_CLEAR_OVRFLAG` (la quale legge internamente `DR` e `SR`). Gli overrun sono monitorati unicamente tramite gli snapshot del registro `SR` e la callback HAL degli errori. L'unico flush di `DR` avviene a prova conclusa, dopo l'arresto controllato del DMA.
- **Campionamento CNDTR a carico minimo:** Nel ciclo principale di polling viene campionato `DMA1_Channel1->CNDTR` tracciando valore minimo, valore massimo e contatore di variazioni (`cndtr_changes`).
- **Zero overhead negli interrupt:** Gli interrupt incrementano esclusivamente contatori scalari volatili a 32 bit (1 ciclo clock). Nessuna operazione di formattazione o invio USB è presente negli IRQ.

---

### 3. Logica di Classificazione e Output USB CDC

Allo scadere dei 10 secondi, il report formattato ASCII viene emesso su USB CDC (115200 8N1) e ritrasmesso periodicamente ogni 5 secondi.

La diagnostica applica una discriminazione oggettiva basata sulle evidenze dei registri, senza attribuire a essi capacità deduttive superiori all'evidenza istantanea:

| Condizione Rilevata | Classificazione Riportata | Deducibilità Tecnica |
| :--- | :--- | :--- |
| Clock periferiche disabilitati, pin non in AF5, `I2SE=0`, `RXDMAEN=0`, DMAMUX != 12, o NVIC mascherato | `CONFIGURAZIONE REGISTRI INCONGRUENTE` | Errore di inizializzazione driver/software; evidenzia il singolo parametro difforme. |
| Registri congruenti, `CNDTR` fisso a 2048 (0 variazioni), `DMA1->ISR = 0`, `SPI2_SR: RXNE=0, BSY=0, OVR=0, FRLVL=0` | `DMA SENZA RICHIESTE/TRASFERIMENTI OSSERVATI` | Nessuna attività dati osservata negli snapshot di `SPI2_SR`. Fotografa lo stato della periferica all'istante di campionamento e non costituisce prova diretta dell'assenza fisica dei clock esterni BCK/LRCK. |
| Registri congruenti, `CNDTR` fisso a 2048 (0 variazioni), ma `SPI2_SR: RXNE=1` oppure `OVR=1` | `DMA SENZA RICHIESTE/TRASFERIMENTI OSSERVATI` | Attività dati/clock osservata in `SPI2_SR` (RXNE/OVR/FRLVL asseriti), ma `CNDTR` non è variato. Le richieste non sono state servite dal DMA (verificare DMAMUX1 o controller DMA1). |
| `CNDTR` decrementato/variato (`cndtr_changes > 0`), ma callback Half/Full = 0 | `DMA CHE TRASFERISCE DATI SENZA CALLBACK OSSERVATE` | `CNDTR` variato (trasferimenti DMA da `SPI2_DR` in memoria osservati), ma nessuna callback è stata invocata durante la prova. Verificare IRQ handler, HTIE/TCIE e maschere NVIC. |
| `CNDTR` variato e callback Half/Full > 0 | `DMA E CALLBACK ENTRAMBI OSSERVATI` | Trasferimenti DMA e invocazione callback Half/Full entrambi osservati durante la prova, senza dichiarare automaticamente continuità nominale per 10 secondi o assenza di errori audio. |

---

### 4. Verifica di Compilazione, Risorse e Isolamento Baseline

Tutti i target compilano in modo deterministico con toolchain STM32CubeCLT 1.22.0 (GCC 14.3.1) con **0 errori e 0 warning**:

```powershell
.\build.ps1 -CubeCltRoot 'B:\Programmi Scaricati\stm32_clt\STM32CubeCLT_1.22.0' -CubeG4Root "C:\Users\omnia\STM32Cube\Repository\STM32Cube_FW_G4_V1.6.3" -Target all
```

#### Tabella di Confronto e Impronte Crittografiche SHA-256
| Target | Flash (ROM) | RAM standard | CCMSRAM | SHA-256 del file `.bin` | Note |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`tickform_phase3_xo`** | 37.004 B (28,23%) | 6.544 B (19,97%) | 0 B | `E0B8F70661F62EC362DAB0EA53B8E7D3E06B02381FF284CB41865C8C9B1DC4BA` | Baseline FASE 3 identica al byte |
| **`tickform_phase5_i2s_rx`** | 43.756 B (33,38%) | 11.712 B (35,74%) | 0 B | `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` | Variante F5-A02-R2.1 **identica al singolo bit** |
| **`tickform_phase5_diag`** | 46.840 B (35,74%) | 13.944 B (42,55%) | 0 B | `45BC15EB5D8FA74CE388C94E946411AFB459A1F9F95538A79B022C6718A0BE41` | Variante Diagnostica F5-D02 |

- **Isolamento confermato:** L'hash SHA-256 di `tickform_phase5_i2s_rx.bin` è identico al bit a quello certificato nel riscontro F5-A02-R2.1.
- **Default di build sicuro:** [build.ps1](file:///A:/Documenti/Tickform/firmware/stm32g431/build.ps1) mantiene `-Target phase3` come valore predefinito.

---

### 5. Pacchetto di Rilascio Diagnostico (ZIP)

È stato generato l'archivio autonomo:
[tickform_f5_d02_review_pack.zip](file:///A:/Documenti/Tickform/firmware/stm32g431/tickform_f5_d02_review_pack.zip)

Contenuto dell'archivio (13 file):
1. `i2s_rx_diag.c`: modulo diagnostico con snapshot a 4 stadi e sniffing registri.
2. `i2s_rx_diag.h`: header strutture snapshot e metriche diagnostiche.
3. `i2s_rx.c`: modulo F5-A02-R2.1 approvato (invariato).
4. `i2s_rx.h`: header F5-A02-R2.1 approvato (invariato).
5. `main.c`: entry point unificato multi-target.
6. `stm32g4xx_hal_conf.h`: configurazione HAL.
7. `CMakeLists.txt`: build configuration multi-target con target 3 `tickform_phase5_diag`.
8. `build.ps1`: script di compilazione con supporto `-Target diag`.
9. `git_diff_e857849.patch`: patch completa del working tree verso il commit `e857849`.
10. `build_phase3.log`: log di compilazione target Phase 3.
11. `build_phase5.log`: log di compilazione target Phase 5.
12. `build_diag.log`: log di compilazione target Phase 5 Diag.
13. `RAPPORTO_F5_D02.md`: presente relazione tecnica.

---

### 6. Dichiarazione di Conformità e STOP

- **SOLO SOFTWARE:** Nessun dispositivo MCU WeAct flashato.
- **Nessun power-up o cablaggio modificato:** Il banco hardware permane a riposo.
- **Nessun commit o push su Git:** L'albero di lavoro rimane non consolidato.
- **STOP:** In attesa delle determinazioni e dell'autorizzazione della CHAT OVERVIEW per l'eventuale flash e acquisizione della diagnostica hardware.
