# TICKFORM — FASE 5
## RAPPORTO TECNICO F5-D02-R2 — Risoluzione Silent Drop CDC

**Data:** 17 novembre 2026  
**Supervisione:** ChatGPT Overview  
**Mandato di riferimento:** Mandato F5-D02-R2  
**Target:** WeAct Studio STM32G431 Core Board V1.0 (STM32G431CBU6)  
**Ambiente di build:** STM32CubeCLT 1.22.0 / ARM GCC 14.3.1, STM32CubeG4 V1.6.3, CMake 3.28.1, Ninja 1.11.1  
**Verdetto:** `F5-D02-R2 READY FOR OVERVIEW REVIEW`

---

### 1. Esito del Gate Preliminare Obbligatorio

In conformità al punto 1 del Mandato F5-D02-R2, sono state verificate le condizioni statiche e dinamiche della trasmissione CDC:

1. **Dimensione di `s_report`:**  
   In `i2s_rx_diag.c` (F5-D02-R1), il buffer era dichiarato staticamente a `char s_report[3500]`.
2. **Limite passato a `snprintf`:**  
   `snprintf(s_report, sizeof(s_report), ...)` utilizzava `sizeof(s_report)` = 3500 byte come limite superiore di scrittura.
3. **Lunghezza effettiva del testo prodotto:**  
   - Con i 4 snapshot estesi e l'analisi testuale completa, la lunghezza calcolata richiesta a `snprintf` è risultata pari a **2900 caratteri**.
   - Poiché 2900 < 3500, non vi era alcun troncamento nel buffer `s_report`: l'intera stringa veniva correttamente composta con terminatore nullo a byte 2900 (`strlen(s_report) == 2900`).
4. **Verifica limite buffer USB (`usb_device.c`):**  
   - In `firmware/stm32g431/USB_Device/usb_device.c`, riga 14:
     ```c
     static uint8_t tx_buffer[1536];
     ```
   - In `diagnostic_cdc_send()`, riga 114:
     ```c
     size_t length = strlen(text);
     if (length > sizeof(tx_buffer)) { return 0; }
     ```
   - Poiché `2900 > 1536`, la condizione di scarto era **sistematicamente soddisfatta ad ogni chiamata**. `diagnostic_cdc_send()` ha restituito `0` incondizionatamente, scartando l'intero pacchetto senza mai invocare `USBD_CDC_TransmitPacket()`.
5. **Raggiungimento del percorso di trasmissione:**  
   È stato accertato che `diagnostic_cdc_send(s_report)` veniva chiamata regolarmente sia al completamento dei 10 secondi (`elapsed >= 10000U`) sia nel ciclo di polling periodico a 5000 ms (`s_test_state == TEST_STATE_COMPLETED`). Entrambi i tentativi fallivano a causa del controllo di dimensione.
6. **Polarità e circuito LED PC6:**  
   - Circuito hardware WeAct Studio STM32G431: LED blu collegato tra PC6 e GND con resistenza di limitazione (configurazione **Active-HIGH**).
   - Livello logico HIGH (3.3 V) = LED acceso visibile.
   - Livello logico LOW (0 V) = LED spento visibile.
   - Comportamento firmware:
     - Durante i primi 10 secondi (`TEST_STATE_RUNNING`): toggle a 2 Hz (`GPIOC->ODR ^= (1UL << 6U)` ogni 250 ms).
     - Al termine (`TEST_STATE_COMPLETED`): se `is_stalled == 1` o errori presenti, PC6 è forzato HIGH (acceso fisso); se non vi è stallo (`total_callbacks > 0`), PC6 è forzato LOW (spento).
7. **Gestione interna di `diagnostic_cdc_send()`:**  
   - Se `length <= sizeof(tx_buffer)` e lo stack è pronto (`USBD_STATE_CONFIGURED` e `cdc->TxState == 0`), la funzione copia i dati in `tx_buffer` via `memcpy()` e avvia la trasmissione USB non bloccante.
   - Se `cdc->TxState != 0` (trasferimento precedente non ancora concluso sul bus) o se il dispositivo non è configurato, la funzione ritorna immediatamente `0` senza bloccare, senza accodare e senza sovrascrivere `tx_buffer`.

**Conclusione Gate:** Il *Silent Drop* è pienamente confermato su base deterministica.

---

### 2. Modifica Software Autorizzata

La correzione è stata applicata **esclusivamente** a:
`firmware/stm32g431/Core/Src/i2s_rx_diag.c`

Nessun altro file è stato alterato. `usb_device.c`, `main.c`, `CMakeLists.txt` e i file storici sono rimasti al 100% intatti.

#### A. Segmentazione del rapporto in 5 parti (tutte <= 763 byte)
Il rapporto originale è stato riorganizzato in 5 segmenti logici indipendenti, formattati on-demand tramite la funzione `format_report_part(stage, buf, max_len)`:

- **PARTE 1/5 (Intestazione e Stato Principale):** Durata, master, pin, configurazione buffer, classificazione e testo di analisi integrale. Lunghezza: **546–763 B**.
- **PARTE 2/5 (Contatori ed Eventi CNDTR):** Callback Half/Full, totale, interrupt grezzi DMA/SPI, errori HAL, OVR, valori CNDTR (iniziale, min, max, pre, post, variazioni). Lunghezza: **~577 B**.
- **PARTE 3/5 (Verifica Registri e Configurazione):** Abilitazione clock RCC (SPI2, DMA1, DMAMUX1, GPIOB), NVIC, DMAMUX request ID (12), configurazione GPIO PB12/PB13/PB15 (MODER e AFR5). Lunghezza: **~571 B**.
- **PARTE 4/5 (Snapshot Registri Runtime INIT e MID):** Registri completi all'avvio e a t=5000 ms (`SPI2_I2SCFGR`, `CR2`, `SR`, `DMA1_CCR`, `CNDTR`, `ISR`, `DMAMUX_CCR`). Lunghezza: **~644 B**.
- **PARTE 5/5 (Snapshot Registri PRE/POST-STOP ed Esito):** Registri all'istante di stop, registri post-stop, status HAL_I2S_DMAStop, stop error code, stop OK. Lunghezza: **~499 B**.

Tutti i 4 snapshot, i campi registro, i contatori e l'analisi qualitativa stabiliti in F5-D02-R1 sono preservati integralmente.

#### B. Macchina a stati di trasmissione non bloccante
- È stata implementata la macchina a stati `s_tx_stage` (0 = idle, 1..5 = invio parte 1..5).
- **Pacing inter-segmento:** intervallo minimo di 10 ms tra pacchetti consecutivi per consentire al bus USB Full Speed e al driver host di completare la ricezione.
- **Avanzamento protetto:** `s_tx_stage` avanza allo stadio successivo solo quando `diagnostic_cdc_send()` restituisce `1` (pacchetto accettato dal sottosistema USB). Se CDC è occupata (`TxState != 0`), il polling ritenta al ciclo successivo senza perdita di dati.
- **Protezione da sovrapposizione:** la ritrasmissione periodica ogni 5000 ms può avviarsi esclusivamente quando `s_tx_stage == 0` (rapporto precedente completamente terminato). Nessun nuovo rapporto può interrompere o corrompere un invio in corso.
- **Timeout di sicurezza:** se uno stadio intermedio (> 1) non riesce a trasmettere per oltre 2000 ms (ad esempio a causa della chiusura improvvisa del terminale o disconnessione USB), la sequenza viene abortita e resettata a 0, evitando deadlock.
- **Isolamento dell'acquisizione:** la trasmissione avviene esclusivamente in `TEST_STATE_COMPLETED`. Durante la finestra di acquisizione di 10 secondi, la trasmissione USB è del tutto inattiva, garantendo zero interferenze con DMA e IRQ.
- **Ottimizzazione RAM:** il buffer `s_report[3500]` è stato rimosso; è utilizzato un unico buffer di segmento statico `s_seg_buffer[1024]`, riducendo l'impronta RAM del firmware di **2476 byte**.

---

### 3. Risultati della Verifica Statica e Software

La suite di test automatizzata (`scratch/verify_software.ps1`) e la build completa hanno verificato:

1. **Lunghezza massima dei segmenti:**
   - Scenario 1 (DMA senza richieste, SR pulito): Max = **763 B** (PASS, limite autorizzato <= 1000 B).
   - Scenario 2 (DMA senza richieste, SR attivo): Max = **692 B** (PASS).
   - Scenario 3 (DMA con dati, senza callback): Max = **720 B** (PASS).
   - Scenario 4 (DMA e callback entrambi osservati): Max = **693 B** (PASS).
   - Scenario 5 (Configurazione incongruente): Max = **644 B** (PASS).
2. **Assenza di troncamento:**
   La concatenazione sequenziale delle 5 parti ricostruisce il 100% delle sezioni diagnostiche, registri e testi senza alcuna perdita di informazione.
3. **Simulazione macchina a stati CDC:**
   - *Test CDC temporaneamente occupata (`TxState == 1`):* ritentato con successo al rilascio del canale; 5/5 segmenti ricevuti nell'ordine corretto.
   - *Test assenza terminale all'avvio:* con host non configurato, lo stage 1 attende senza avanzare; non appena l'host si connette, tutti i 5 segmenti vengono erogati ordinatamente.
   - *Test mancata sovrapposizione:* simulata la scadenza del timer di 5000 ms durante una trasmissione in corso; il nuovo ciclo non si avvia finché lo stadio attivo non ha raggiunto 0.
4. **Verifica assenza regressioni (build pulita multi-target):**
   - **Phase 3 Baseline (`tickform_phase3_xo`):**
     - ROM: 37004 B, RAM: 6544 B
     - SHA-256: `E0B8F70661F62EC362DAB0EA53B8E7D3E06B02381FF284CB41865C8C9B1DC4BA` (identico al millesimo di byte).
   - **Phase 5 Baseline approvata (`tickform_phase5_i2s_rx`):**
     - ROM: 43756 B, RAM: 11712 B
     - SHA-256: `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465` (identico al millesimo di byte).
   - **Variante Diagnostica F5-D02-R2 (`tickform_phase5_diag`):**
     - ROM: 47404 B (128 KB max), RAM: 11488 B (32 KB max, ridotta rispetto ai 13964 B di R1)
     - SHA-256: `D42E1CBC484CD5D1E0F100302BE18DAEC3571F3CE5E2A1CE9CED0D23AA71A11C`.

---

### 4. Diff Sorgente Dettagliato (`i2s_rx_diag.c`)

```diff
--- a/firmware/stm32g431/Core/Src/i2s_rx_diag.c
+++ b/firmware/stm32g431/Core/Src/i2s_rx_diag.c
@@ -45,3 +45,8 @@
 static i2s_diag_metrics_t s_metrics;
-static char s_report[3500];
+static const char *s_classification = "";
+static const char *s_analysis_detail = "";
+static int s_tx_stage = 0;              /* 0: idle / wait for cycle; 1..5: transmitting part 1..5 */
+static uint32_t s_last_part_tick = 0;   /* Tick when the last part was accepted */
+static uint32_t s_tx_stage_start_tick = 0; /* Tick when current stage was entered (for timeout) */
+static char s_seg_buffer[1024];         /* Single scratch buffer for the active part */
@@ -139,2 +144,7 @@
     s_mid_snapshot_done = 0;
+    s_classification = "";
+    s_analysis_detail = "";
+    s_tx_stage = 0;
+    s_last_part_tick = 0;
+    s_tx_stage_start_tick = 0;
     memset(&s_metrics, 0, sizeof(s_metrics));
@@ -226,2 +236,206 @@
 }
 
+/* =========================================================================
+ * Segmented Diagnostic Report Formatter (strictly <= 1000 B per part)
+ * ========================================================================= */
+static int format_report_part(int part, char *buf, size_t max_len)
+{
+    switch (part) {
+    case 1:
+        return snprintf(buf, max_len,
+            "==================================================\r\n"
+            "TICKFORM FASE 5 - DIAGNOSTICA F5-D02 (PARTE 1/5)\r\n"
+            "SPI2/I2S2 -> DMAMUX1 -> DMA1 Channel 1\r\n"
+            "==================================================\r\n"
+            "Duration:     %lu ms (nominal 10000 ms)\r\n"
+            "Master:       PCM1808 C (96 kHz nominal)\r\n"
+            "Pins:         PB12=WS (LRCK), PB13=CK (BCK), PB15=SD (DOUT)\r\n"
+            "DMA Buffer:   1024 slots (2048 halfwords = 4096 B)\r\n\r\n"
+            "--- STATO DIAGNOSTICO PRINCIPALE ---\r\n"
+            "CLASSIFICAZIONE: [%s]\r\n"
+            "ANALISI:         %s\r\n\r\n",
+            (unsigned long)s_metrics.duration_ms, s_classification, s_analysis_detail);
+    case 2:
+        /* Contatori eventi, interrupt grezzi DMA/SPI, OVR e monitoraggio CNDTR */
+        ...
+    case 3:
+        /* Verifica RCC clock, NVIC, DMAMUX request ID e GPIO PB12/13/15 AF */
+        ...
+    case 4:
+        /* Snapshot registri runtime INIT e MID */
+        ...
+    case 5:
+        /* Snapshot registri runtime PRE/POST-STOP ed esito procedura di arresto */
+        ...
+    default:
+        buf[0] = '\0';
+        return 0;
+    }
+}
@@ -569,22 +583,34 @@
-            /* Format report */
-            int len = snprintf(s_report, sizeof(s_report), ...);
-            diagnostic_cdc_send(s_report);
-            s_last_report_tick = now;
+            /* Initialize multi-part CDC transmission sequence */
+            s_tx_stage = 1;
+            s_tx_stage_start_tick = now;
+            s_last_part_tick = 0;
+            s_last_report_tick = now;
         }
     } else if (s_test_state == TEST_STATE_COMPLETED) {
-        if ((uint32_t)(now - s_last_report_tick) >= 5000U) {
-            diagnostic_cdc_send(s_report);
-            s_last_report_tick = now;
-        }
         if (s_metrics.is_stalled || s_metrics.ovr_count > 0 || s_metrics.error_callback_count > 0) {
             GPIOC->BSRR = (1UL << 6U);
         } else {
             GPIOC->BRR = (1UL << 6U);
         }
+
+        /* Multi-part CDC transmission state machine */
+        if (s_tx_stage == 0) {
+            if ((uint32_t)(now - s_last_report_tick) >= 5000U) {
+                s_tx_stage = 1;
+                s_tx_stage_start_tick = now;
+                s_last_part_tick = 0;
+            }
+        }
+        if (s_tx_stage >= 1 && s_tx_stage <= 5) {
+            if ((uint32_t)(now - s_last_part_tick) >= 10U) {
+                format_report_part(s_tx_stage, s_seg_buffer, sizeof(s_seg_buffer));
+                if (diagnostic_cdc_send(s_seg_buffer)) {
+                    s_last_part_tick = now;
+                    s_tx_stage++;
+                    s_tx_stage_start_tick = now;
+                    if (s_tx_stage > 5) {
+                        s_tx_stage = 0;
+                        s_last_report_tick = now;
+                    }
+                } else {
+                    if (s_tx_stage > 1 && (uint32_t)(now - s_tx_stage_start_tick) >= 2000U) {
+                        s_tx_stage = 0;
+                        s_last_report_tick = now;
+                    }
+                }
+            }
+        }
     }
```

---

### 5. Artefatti Prodotti

| File | Percorso | Dimensione | SHA-256 |
|---|---|---|---|
| **Binary F5-D02-R2** | `firmware/stm32g431/build/diag/tickform_phase5_diag.bin` | 47.404 byte | `D42E1CBC484CD5D1E0F100302BE18DAEC3571F3CE5E2A1CE9CED0D23AA71A11C` |
| **ELF F5-D02-R2** | `firmware/stm32g431/build/diag/tickform_phase5_diag.elf` | — | ROM: 47404 B, RAM: 11488 B |
| **Review Pack** | `firmware/stm32g431/tickform_f5_d02_r2_review_pack.zip` | — | Pacchetto completo per audit indipendente |

---

### 6. Limiti Ancora da Validare sul Microcontrollore e Procedura Operativa

1. **Limiti non validabili in simulazione host:**
   - La corretta ricezione dei 5 pacchetti su Tera Term su PC reale dipende dal completamento dell'enumerazione USB sul sistema operativo Windows host.
   - Non è possibile verificare a priori l'effettiva reattività dei registri hardware I2S2 del chip PCM1808 C senza l'esecuzione on-target.
2. **Procedura operativa raccomandata per l'operatore (per quando sarà autorizzato il test):**
   - Collegare il cavo USB-C al PC.
   - Aprire Tera Term sulla porta COM della WeAct (velocità e parametri indifferenti per CDC).
   - Attendere l'esecuzione (LED lampeggiante a 2 Hz per 10 secondi).
   - Allo scadere dei 10 secondi, Tera Term riceverà i 5 segmenti completi in rapida sequenza (< 70 ms totali).
   - Se Tera Term viene aperto successivamente, la trasmissione viene ripetuta ogni 5 secondi in modo continuativo.
   - *Nota Windows usbser.sys:* se si preme il pulsante fisico NRST mentre Tera Term ha già la porta aperta, Windows disconnette la COM: in tal caso è necessario disconnettere e riconnettere la sessione in Tera Term (`File -> Disconnect`, poi `File -> Connect`) per riagganciare lo stream seriale.

---

### 7. Dichiarazione di Rispetto dei Divieti e Arresto

In conformità tassativa al punto 5 del Mandato F5-D02-R2:
- **NESSUN** flash STM32 eseguito.
- **NESSUN** reset o nuova alimentazione hardware effettuata.
- **NESSUNA** modifica apportata a cablaggi, PCM1808 C o oscillatore XO.
- **NESSUNA** alterazione dei firmware storici o della baseline.
- **NESSUN** commit o push Git eseguito.

**VERDETTO:**  
`F5-D02-R2 READY FOR OVERVIEW REVIEW`
