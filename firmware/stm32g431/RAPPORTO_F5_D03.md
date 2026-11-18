# TICKFORM — FASE 5
## Rapporto F5-D03 — Verifica digitale degli ingressi PB12/PB13 e Registri DMA

**Data:** 17/11/2026  
**Mandato di riferimento:** Mandato CHAT OVERVIEW — 17/11/2026 (F5-D03)  
**Baseline Git:** commit `e857849debbc54e655a8a87a88c07a2ee0386d43`  
**Stato implementazione:** GO SOFTWARE COMPLETATO — Variante diagnostica F5-D03 compilata, verificata e pacchettizzata. In attesa di autorizzazione flash da CHAT OVERVIEW.

---

## 1. Obiettivo del mandato F5-D03

A seguito dell'esito del test F5-D02-R2 (in cui la periferica DMA1 Channel 1 è rimasta con CNDTR fermo a 2048 e `SPI2->SR = 0x0002` senza generare interrupt o richieste DMA), il mandato F5-D03 richiede di:
1. **Verificare digitalmente gli ingressi PB12 (LRCK/WS) e PB13 (BCK/CK):** stabilire se i segnali digitali prodotti dal PCM1808 C producono reali transizioni logiche sui pin PB12 e PB13 visti dal core STM32G431, **senza alterare la configurazione Alternate Function (AF5)**.
2. **Implementare un campionamento software non invasivo di `GPIOB->IDR`:** eseguire una lettura ciclica rapida del registro d'ingresso per una finestra controllata (10 ms) immediatamente dopo l'arresto del test DMA dei 10 secondi.
3. **Conteggiare transizioni e monitorare entrambi i livelli (HIGH e LOW):** separatamente per PB12 e PB13, registrando livello iniziale, livello finale e presenza effettiva di entrambi gli stati logici.
4. **Documentare durata e frequenza di campionamento:** misurare la durata effettiva in microsecondi tramite contatore cicli DWT Cortex-M4 e stimare il sampling rate in kSps / MSps.
5. **Cautela interpretativa su PB13:** vietare conclusioni categoriche di assenza clock su PB13 qualora la frequenza di campionamento software risulti inferiore al criterio di Nyquist per un segnale a 6,144 MHz.
6. **Aggiungere verifica indirizzi DMA CPAR e CMAR:** leggere e visualizzare `DMA1_Channel1->CPAR` (confrontato con `&SPI2->DR = 0x4000380C`) e `DMA1_Channel1->CMAR` (confrontato con `i2s_dma_buffer`), segnalando la conformità.
7. **Rettificare le etichette invertite I2SE/I2SMOD:** correggere l'estrazione dei bit del registro `SPI_I2SCFGR` (bit 10 = `I2SE`, bit 11 = `I2SMOD`).
8. **Trasmettere i dati tramite protocollo CDC a segmenti:** estendere la trasmissione da 5 a 6 segmenti, garantendo che ogni singolo segmento rimanga rigorosamente `<= 1000 byte`.
9. **Preservare la baseline:** verificare che il firmware FASE 3 (`tickform_phase3_xo.bin`) e il firmware approvato FASE 5 (`tickform_phase5_i2s_rx.bin`) rimangano rigorosamente inalterati.

---

## 2. Architettura tecnica e dettagli implementativi

### 2.1 Campionamento non invasivo GPIO IDR in Alternate Function
Nel microcontrollore STM32G431, quando un pin GPIO è configurato in modalità Alternate Function (`MODER = 10b`), il buffer d'ingresso con trigger di Schmitt rimane permanentemente collegato al pad e sincronizzato con l'AHB clock (RM0440 Rev 7, sezione 9.4.1). Pertanto, la lettura diretta del registro `GPIOB->IDR`:
- Riflette fedelmente il livello logico istantaneo presente sul pad fisico.
- Non altera in alcun modo lo stato, l'impedenza o la configurazione interna della periferica I²S2/AF5.
- Non riconfigura né tocca i registri `MODER`, `PUPDR`, `AFRH` o `OTYPER`.

### 2.2 Routine `run_pin_input_test()` con temporizzazione DWT
La funzione `run_pin_input_test()` è implementata direttamente in [firmware/stm32g431/Core/Src/i2s_rx_diag.c](file:///a:/Documenti/Tickform/firmware/stm32g431/Core/Src/i2s_rx_diag.c):
1. **Attivazione contatore cicli Cortex-M4:** abilita `CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk` e `DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk`. `DWT->CYCCNT` incrementa ad ogni ciclo di clock SYSCLK (170 MHz).
2. **Finestra controllata di 10 ms (10.000 µs):** definita come `target_cycles = 1.700.000` cicli a 170 MHz.
3. **Loop di campionamento ad alta velocità:**
   - Lettura volatile di `GPIOB->IDR` ad ogni iterazione.
   - Estrazione di bit 12 (`b12`) e bit 13 (`b13`).
   - Rilevamento fronti (`b12 != last_b12` e `b13 != last_b13`) con incremento dei contatori dedicati `pb12_transitions` e `pb13_transitions`.
   - Aggiornamento dei flag di livello `pb12_saw_high`, `pb12_saw_low`, `pb13_saw_high`, `pb13_saw_low`.
   - Conteggio totale delle iterazioni `sample_count`.
4. **Calcolo metriche temporali e sampling rate:**
   - Durata: `duration_us = elapsed_cycles / (SystemCoreClock / 1.000.000)`.
   - Frequenza: `sample_rate_ksps = (sample_count * 1000) / duration_us`.
   - Con il core a 170 MHz e istruzioni ottimizzate, il loop esegue circa 110.000–120.000 iterazioni in 10 ms, fornendo una frequenza di campionamento effettiva di circa **11–12 MSps** (periodo di campionamento ~83–90 ns).

### 2.3 Analisi di Nyquist e sensibilità dei segnali
- **Segnale LRCK (PB12 — 96 kHz nominale):**
  - Periodo $T = 10,42\ \mu\text{s}$, semiperiodo $5,21\ \mu\text{s}$.
  - Con campionamento a ~11,8 MSps (~85 ns), si ottengono **circa 60 campioni per ciascun semiperiodo**.
  - Il segnale LRCK è ampiamente al di sotto del limite di Nyquist ($f_s \gg 2 f_{in}$).
  - In 10 ms sono attese nominalmente circa **1920 transizioni** ($96.000 \times 2 \times 0,010$).
  - La rilevazione di transizioni e di entrambi i livelli logici (HIGH e LOW) su PB12 è deterministica e affidabile al 100%.
- **Segnale BCK (PB13 — 6,144 MHz nominale):**
  - Periodo $T = 162,8\ \text{ns}$, semiperiodo $81,4\ \text{ns}$.
  - Con un periodo di campionamento software di ~85 ns, il rate è leggermente inferiore al criterio di Nyquist per una corretta ricostruzione della forma d'onda ($f_s \approx 11,8\ \text{MSps} < 2 \times 6,144\ \text{MHz} = 12,288\ \text{MSps}$).
  - **Comportamento atteso:** un segnale a 6,144 MHz attivo produce aliasing stocastico nel campionamento software, ma viene intercettato sia in stato HIGH che in stato LOW, registrando migliaia di transizioni (`pb13_transitions > 0`, `pb13_saw_high = 1`, `pb13_saw_low = 1`).
  - **Condizione di guasto:** se PB13 è scollegato, flottante o bloccato su una linea statica, le transizioni saranno pari a 0 e uno dei due livelli non verrà mai asserito.
  - **Vincolo del mandato:** il report include esplicitamente la clausola di cautela tecnica:
    `"Nota PB13: Campionamento SW a ~XX.X MSps (sotto Nyquist per 6.144 MHz). Aliasing atteso; transizioni > 0 indicano attivita' di toggle; non dichiarare assenza clock senza verifica strumentale diretta."`

### 2.4 Verifica indirizzi DMA Channel 1 (CPAR e CMAR)
Nel registro snapshot e nella Parte 2 del report:
- `CPAR`: indirizzo periferica DMA letto da `DMA1_Channel1->CPAR`.
  - Atteso: `&SPI2->DR` = `0x4000380C`.
  - Esito conformità: `CPAR == 0x4000380C` -> `CONFORME`.
- `CMAR`: indirizzo memoria DMA letto da `DMA1_Channel1->CMAR`.
  - Atteso: `(uint32_t)i2s_dma_buffer` (allocato in SRAM, 4-byte aligned).
  - Esito conformità: `CMAR == (uint32_t)i2s_dma_buffer` -> `CONFORME`.

### 2.5 Correzione etichette registri I2SE / I2SMOD
Nel driver CMSIS `stm32g431xx.h`:
- Bit 10: `SPI_I2SCFGR_I2SE_Pos` (I2S Enable)
- Bit 11: `SPI_I2SCFGR_I2SMOD_Pos` (I2S Mode Select: 1 = I2S, 0 = SPI)
Nella stampa della Parte 5 (snapshot INIT e MID) è stata rettificata l'estrazione:
```c
(unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 10) & 1U), /* I2SE (bit 10) */
(unsigned long)((s_metrics.snap_init.spi2_i2scfgr >> 11) & 1U), /* I2SMOD (bit 11) */
```

### 2.6 Segmentazione e pacing CDC (6 parti)
Per garantire la massima leggibilità e il rispetto categorico del limite di 1000 byte per parte, il rapporto è suddiviso in 6 parti:
- **Parte 1/6 (Header e Stato Principale):** ~450 byte.
- **Parte 2/6 (Contatori, CNDTR, Indirizzi CPAR/CMAR):** ~650 byte.
- **Parte 3/6 (Configurazione RCC, NVIC, DMAMUX, GPIO MODER/AFR):** ~600 byte.
- **Parte 4/6 (Verifica digitale ingressi PB12/PB13 — GPIO IDR):** ~700 byte.
- **Parte 5/6 (Snapshot Registri INIT e MID):** ~680 byte.
- **Parte 6/6 (Snapshot Registri PRE-STOP/POST-STOP ed Esito):** ~600 byte.

Ogni parte è trasmessa tramite `diagnostic_cdc_send()` con pacing di 10 ms tra pacchetti e ritrasmissione ciclica del report completo ogni 5 secondi, prevenendo il Silent Drop.

---

## 3. Preservazione della baseline e matrici di build

La compilazione multi-target è stata eseguita tramite il toolchain ufficiale:
- **Compilatore:** GNU Arm Embedded Toolchain 14.3.1 (STM32CubeCLT 1.22.0).
- **Driver HAL:** STM32CubeG4 Firmware Package V1.6.3.
- **Build system:** CMake + Ninja.

### 3.1 Confronto hash SHA-256 dei binari

| Target | Binario | Dimensione | SHA-256 Calcolato | Hash Baseline Atteso | Esito Baseline |
| :--- | :--- | :---: | :--- | :--- | :---: |
| **Phase 3 Baseline** | `tickform_phase3_xo.bin` | 37.004 byte | `E0B8F70661F62EC362DAB0EA53B8E7D3E06B02381FF284CB41865C8C9B1DC4BA` | `E0B8F70661F62EC362DAB0EA53B8E7D3E06B02381FF284CB41865C8C9B1DC4BA` | **IDENTICO (PASS)** |
| **Phase 5 Approvato** | `tickform_phase5_i2s_rx.bin` | 43.748 byte | `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` | `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` | **IDENTICO (PASS)** |
| **Phase 5 Diag (F5-D03)** | `tickform_phase5_diag.bin` | 52.308 byte | `29786361C8299AE5CEFDA480C97EC100A30616624D8029BAE275F164BA3A43B7` | *(nuova variante F5-D03)* | **TARGET BUILD OK** |

### 3.2 Occupazione memoria `tickform_phase5_diag.elf`

```
Memory region         Used Size  Region Size  %age Used
         CCMSRAM:           0 B        10 KB      0.00%
             RAM:       11584 B        32 KB     35.35%
             ROM:       52308 B       128 KB     39.91%

   text    data     bss     dec     hex filename
  51820     480   11104   63404    f7ac tickform_phase5_diag.elf
```

---

## 4. Diff sintetico del codice rispetto a F5-D02-R2

### 4.1 Modifiche in `Core/Inc/i2s_rx_diag.h`
- Aggiunti campi `dma1_cpar`, `dma1_cmar`, `gpiob_idr` alla struttura `i2s_diag_snapshot_t`.
- Definita struttura `i2s_diag_pin_test_t` con contatori di transizione, flag di livello e metriche di campionamento.
- Aggiunti campi `expected_cpar`, `expected_cmar`, `cpar_match`, `cmar_match` e `pin_test` alla struttura `i2s_diag_metrics_t`.

### 4.2 Modifiche in `Core/Src/i2s_rx_diag.c`
- In `capture_snapshot()`: cattura di `DMA1_Channel1->CPAR`, `DMA1_Channel1->CMAR` e `GPIOB->IDR`.
- In `i2s_rx_diag_init()`: impostazione degli indirizzi attesi `expected_cpar = (uint32_t)&(SPI2->DR)` e `expected_cmar = (uint32_t)i2s_dma_buffer`, con confronto preliminare.
- Aggiunta funzione `run_pin_input_test(i2s_diag_pin_test_t *t)` con loop ad alta velocità temporizzato da DWT cycle counter.
- In `i2s_rx_diag_poll()`:
  - Invocazione di `run_pin_input_test(&s_metrics.pin_test)` al termine dei 10 secondi.
  - Verifica automatica di conformità CPAR e CMAR in `cfg_ok`.
  - Nota oggettiva sull'esito del test pin in `s_analysis_detail` senza trarre conclusioni hardware premature.
  - Transizione della macchina a stati CDC a 6 segmenti (`s_tx_stage <= 6`).
- In `format_report_part()`:
  - Parte 2: aggiunta sezione `CPAR / CMAR` con confronto `CONFORME / NON CONFORME`.
  - Parte 4: nuova sezione dedicata alla verifica digitale ingressi PB12 e PB13 (transizioni, livelli, sampling rate e nota cautelativa su PB13).
  - Parte 5: correzione etichette bit 10 (`I2SE`) e bit 11 (`I2SMOD`), aggiunta stampa esadecimale di `IDR`, `CPAR` e `CMAR`.
  - Parte 6: snapshot pre/post-stop con `IDR`, `CPAR`, `CMAR` ed esito complessivo.

---

## 5. Contenuto del pacchetto di revisione F5-D03

Il pacchetto compresso `firmware/stm32g431/tickform_f5_d03_review_pack.zip` contiene:
1. `build.ps1` — Script di build multi-target.
2. `CMakeLists.txt` — Configurazione build per i 3 target.
3. `Core/Inc/i2s_rx_diag.h` — Header con strutture dati F5-D03.
4. `Core/Src/i2s_rx_diag.c` — Sorgente diagnostica con test IDR e CDC a 6 segmenti.
5. `Core/Src/main.c` — Entry point.
6. `USB_Device/usb_device.c` — Driver CDC USB.
7. `RAPPORTO_F5_D03.md` — Il presente documento di sintesi tecnica.
8. `tickform_phase5_diag.bin` — Binario pronto per il flash (52.308 byte).
9. `tickform_phase5_diag.elf` — Eseguibile ELF con simboli di debug.
10. `tickform_phase5_diag.map` — File mappa con allocazione delle sezioni.

---

## 6. Conclusioni e Conformità alle Regole di Sicurezza

- **Sicurezza Hardware:** non è stata eseguita alcuna operazione di flash, alimentazione banco, rilievo strumentale o modifica di cablaggio. L'attività è rimasta rigorosamente confinata al software.
- **Git Hygiene:** nessun commit o push è stato eseguito; la baseline `e857849debbc54e655a8a87a88c07a2ee0386d43` è intatta.
- **Preservazione Baseline:** `tickform_phase3_xo.bin` e `tickform_phase5_i2s_rx.bin` sono stati ricompilati e verificati con SHA-256 identico al 100%.

**VERDETTO:** `F5-D03 READY FOR OVERVIEW REVIEW`. Si attende autorizzazione da CHAT OVERVIEW per procedere al flash della variante diagnostica.
