# TICKFORM — FASE 5
## Rapporto F5-A02-R2.1 — Micro-correzione conclusiva diagnostica DMA
**Data:** 13/11/2026  
**Riferimento Mandato:** Mandato CHAT OVERVIEW F5-A02-R2.1  
**Commit Baseline:** `e857849debbc54e655a8a87a88c07a2ee0386d43`  
**Progetto Firmware:** `firmware/stm32g431/`

---

### 1. Risoluzione delle Micro-correzioni F5-A02-R2.1

In ottemperanza al mandato F5-A02-R2.1 della CHAT OVERVIEW, sono state applicate le due rifiniture conclusive nel modulo firmware sperimentale I²S2 Slave RX + DMA (`firmware/stm32g431/Core/Src/i2s_rx.c` e `Core/Inc/i2s_rx.h`):

#### 1.1. Controllo hardware e software dell'arresto DMA
- **Implementazione della verifica a due livelli (Hardware + HAL Status):**
  Dopo l'invocazione di `HAL_I2S_DMAStop(&hi2s2)` e lo svuotamento del registro RX (`__HAL_I2S_FLUSH_RX_DR(&hi2s2)`), il firmware esegue un controllo rigoroso:
  1. **Verifica hardware dei registri:**
     - Canale DMA: `(DMA1_Channel1->CCR & DMA_CCR_EN) == 0` (canale disabilitato a livello hardware).
     - Richiesta periferica: `(SPI2->CR2 & SPI_CR2_RXDMAEN) == 0` (richiesta DMA di ricezione disabilitata).
     Se uno dei due flag risulta ancora asserito, l'arresto è considerato fallito (`stop_ok = 0`).
  2. **Verifica stato e codice di errore HAL:**
     - Caso A: `stop_status == HAL_OK` con `ErrorCode == HAL_I2S_ERROR_NONE` (arresto pulito standard).
     - Caso B: `stop_status == HAL_ERROR` accompagnato **esclusivamente** da `HAL_I2S_ERROR_BUSY_LINE_RX` (`0x40`), comportamento fisiologico e documentato in modalità slave RX con generatore di clock master esterno continuo (PCM1808).
     - Qualsiasi altro errore (es. `HAL_I2S_ERROR_DMA`, `HAL_I2S_ERROR_TIMEOUT`) o combinazione anomala esita in `stop_ok = 0`.
- **Integrazione nel criterio PASS / FAIL:**
  Se `!s_metrics.stop_ok`, il test esita tassativamente in `FAIL: DMA STOP FAILED (channel or request not disabled, or unexpected HAL error)` con accensione del LED di errore PC6.
- **Rappresentazione dinamica nel report:**
  La riga `DMA Stop:` non riporta più un generico `COMPLETED`, ma lo stato effettivo verificato:
  - In caso nominale: `OK (BUSY_LINE_RX expected, DMA1_Ch1=OFF, RXDMAEN=OFF)`.
  - In caso di anomalia: `FAIL (status=%d, err=0x%02lX, DMA_EN=%u, RXDMAEN=%u)`.

#### 1.2. Rettifica dicitura flag FRE (STM32G4 RM0440 §42.7.8)
- Il riferimento al manuale di riferimento STM32G4 RM0440 confermando che il flag FRE è architetturalmente applicabile alla periferica I²S in modalità slave, la precedente dicitura è stata formalmente rettificata.
- Nel rapporto ASCII su USB CDC, la riga diagnostica è ora formulata esattamente come:
  `Frame Error (FRE): NON MONITORATO — sincronismo frame non verificato`
- Si conferma che il criterio `PASS` attesta unicamente la ricezione DMA funzionale entro la finestra di tolleranza di frequenza (±2% a ~96 kHz), senza dichiarare verificata l'integrità qualitativa dei campioni o l'assenza di errori di sincronismo di frame.

---

### 2. Struttura del Report Diagnostico USB CDC

Il rapporto diagnostico ASCII emesso su USB CDC (115200 8N1) presenta la seguente struttura definitiva:

```text
========================================
TICKFORM FASE 5 - I2S2 RX + DMA TEST
========================================
Duration:     10003 ms (nominal 10000 ms)
Master:       PCM1808 C (96 kHz nominal)
STM32:        I2S2 Slave RX, Philips 24b/32b frame
Pins:         PB12=WS (LRCK), PB13=CK (BCK), PB15=SD (DOUT)
DMA:          DMA1 Ch1 circular, buffer 1024 slots (4096 B)
DMA Stop:     OK (BUSY_LINE_RX expected, DMA1_Ch1=OFF, RXDMAEN=OFF)

--- CALLS & THROUGHPUT ---
Half Cplt:    1876 (rate ~187.5/s)
Full Cplt:    1875 (rate ~187.5/s, balance diff=1)
Total DMA CB: 3751 (expected: 3751, tol +/-2%: 3675..3826)
Slots recv:   1920512
Bytes recv:   7682048
Throughput:   767974 B/s (nominal 768000 B/s)

--- ERROR MONITORING ---
Error CB:     0
Overrun(OVR): 0
Frame Error (FRE): NON MONITORATO — sincronismo frame non verificato
HAL Error:    0x00000000
DMA Stalled:  NO

--- RAW SAMPLES (First 4 frames: MS, LS -> PCM24) ---
F0: L=(0x0000,0x0000 -> 0) R=(0x0000,0x0000 -> 0)
F1: L=(0x0000,0x0000 -> 0) R=(0x0000,0x0000 -> 0)
F2: L=(0x0000,0x0000 -> 0) R=(0x0000,0x0000 -> 0)
F3: L=(0x0000,0x0000 -> 0) R=(0x0000,0x0000 -> 0)

--- OVERALL STATUS ---
PASS: DMA RX ~96 kHz FUNCTIONAL (rate within +/-2%, audio conversion NOT YET validated)
========================================
```

---

### 3. Verifica Compilazione e Isolamento Baseline

Compilazione multi-target verificata con successo tramite toolchain ufficiale:

```powershell
.\build.ps1 -CubeCltRoot 'B:\Programmi Scaricati\stm32_clt\STM32CubeCLT_1.22.0' -CubeG4Root "C:\Users\omnia\STM32Cube\Repository\STM32Cube_FW_G4_V1.6.3" -Target all
```

#### Confronto Risorse e Footprint
| Metrica | FASE 3 Baseline (`tickform_phase3_xo`) | FASE 5 Variante (`tickform_phase5_i2s_rx`) | Delta F5 vs F3 |
| :--- | :--- | :--- | :--- |
| **Flash (ROM)** | 37.004 B (28,23% di 128 KB) | 43.756 B (33,38% di 128 KB) | +6.752 B |
| **RAM standard** | 6.544 B (19,97% di 32 KB) | 11.712 B (35,74% di 32 KB) | +5.168 B |
| **CCMSRAM** | 0 B (0,00% di 10 KB) | 0 B (0,00% di 10 KB) | 0 B |
| **Buffer DMA** | Assente | 4.096 B statico (`i2s_dma_buffer`) | +4.096 B |
| **Warning / Errori**| **0 / 0** | **0 / 0** | **0 / 0** |

- **Isolamento della produzione FASE 3:** Il binario `tickform_phase3_xo` è preservato identico al byte rispetto al baseline `e857849`.
- **Default sicuro:** Il parametro di [build.ps1](file:///A:/Documenti/Tickform/firmware/stm32g431/build.ps1) rimane impostato su `$Target = 'phase3'`.

---

### 4. Pacchetto di Revisione Indipendente (ZIP)

L'archivio ZIP autonomo è stato aggiornato:
[tickform_f5_a02_review_pack.zip](file:///A:/Documenti/Tickform/firmware/stm32g431/tickform_f5_a02_review_pack.zip)

Contenuto (10 file):
1. `i2s_rx.c`: sorgente modulo con controlli DMA stop a 2 livelli e rettifica stringa FRE.
2. `i2s_rx.h`: header con campi di tracciamento stop hardware.
3. `main.c`: entry point isolato via macro `TICKFORM_PHASE5_I2S_RX`.
4. `stm32g4xx_hal_conf.h`: configurazione HAL con modulo I2S abilitato.
5. `CMakeLists.txt`: build configuration multi-target.
6. `build.ps1`: script PowerShell con default `'phase3'`.
7. `git_diff_e857849.patch`: patch completa del working tree rispetto a `e857849`.
8. `build_phase3.log`: log di compilazione pulita FASE 3 (0 warning, 0 errori).
9. `build_phase5.log`: log di compilazione pulita FASE 5 (0 warning, 0 errori).
10. `RAPPORTO_F5_A02_R2.md`: presente relazione tecnica aggiornata a R2.1.

---

### 5. Stato di Conformità e Vincoli

- **Stato Git:** Nessun commit, nessun push, nessun rebase o merge eseguito.
- **Hardware:** Nessun cavetto modificato, nessun flash su target, nessun alimentatore acceso (vincolo READ-ONLY su banco).
- **CAD KiCad:** Nessuna modifica apportata ai file CAD KiCad di FASE 4.
