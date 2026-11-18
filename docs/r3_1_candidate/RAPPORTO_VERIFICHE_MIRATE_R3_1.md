# TICKFORM — RAPPORTO DELLE VERIFICHE TECNICHE MIRATE (R3.1)

**Data:** 18 novembre 2026  
**Autore:** Antigravity (Ingegnere di Sistema e Archivista di Progetto)  
**Supervisione:** CHAT OVERVIEW  
**Destinatario:** CHAT OVERVIEW / Revisore Tecnico Indipendente  
**Riferimento:** Mandato R3.1 (§4, §5) e Revisione Indipendente R3 (`TICKFORM_R3_REVIEW_INDIPENDENTE_20261118.md`)  
**Repository:** `A:\Documenti\Tickform`  
**Modalità:** ISPEZIONE IN SOLA LETTURA (Nessuna modifica a sorgenti, firmware o configurazioni)

---

## 1. Verifica R3-ERR-04 — Analisi dello Stop I²S, Status=1 ed ErrorCode=0x40

### 1.1. Contesto e quesito tecnico
Nei log diagnostici nominali F5-D03 (Run 2, Run 5A, Run 5B) compare la seguente riga di stato conclusiva:
```text
Stop:   Status=1, ErrorCode=0x40, StopOK=SI
```
Al contempo, durante l'intero intervallo di acquisizione di 10 secondi, i contatori documentano:
- `Raw IRQ SPI2 = 0`
- `HAL I2S Error CB = 0` (last=0x00000000)
- `SPI2 OVR contati = 0`

La supervisione ha richiesto di esaminare in sola lettura il codice del firmware diagnostico (`firmware/stm32g431/Core/Src/i2s_rx_diag.c`) e i driver HAL per stabilire l'origine esatta di tali valori, se costituiscano un'anomalia o un comportamento atteso, e come venga determinato `StopOK=SI`.

---

### 1.2. Origine nel codice sorgente (`i2s_rx_diag.c`)

Nel file `firmware/stm32g431/Core/Src/i2s_rx_diag.c`, alla scadenza della finestra di 10.000 ms, la sequenza di arresto viene eseguita alle righe 675–712:

```c
/* firmware/stm32g431/Core/Src/i2s_rx_diag.c:691-712 */

/* Stop DMA and flush DR */
s_metrics.stop_status = HAL_I2S_DMAStop(&hi2s2);
__HAL_I2S_FLUSH_RX_DR(&hi2s2);

/* Capture snapshot POST-STOP */
capture_snapshot(&s_metrics.snap_post_stop, HAL_GetTick() - s_start_tick);
s_metrics.cndtr_post_stop = s_metrics.snap_post_stop.dma1_cndtr;

/* Verify stop status */
s_metrics.stop_error_code = hi2s2.ErrorCode;
s_metrics.dma_channel_enabled = ((DMA1_Channel1->CCR & DMA_CCR_EN) != 0U);
s_metrics.rxdmaen_enabled = ((SPI2->CR2 & SPI_CR2_RXDMAEN) != 0U);

if (s_metrics.dma_channel_enabled || s_metrics.rxdmaen_enabled) {
    s_metrics.stop_ok = 0;
} else if (s_metrics.stop_status == HAL_OK && s_metrics.stop_error_code == HAL_I2S_ERROR_NONE) {
    s_metrics.stop_ok = 1;
} else if (s_metrics.stop_status == HAL_ERROR && s_metrics.stop_error_code == HAL_I2S_ERROR_BUSY_LINE_RX) {
    s_metrics.stop_ok = 1;
} else {
    s_metrics.stop_ok = 0;
}
```

La formattazione del report avviene alla riga 581:
```c
"  Stop:   Status=%d, ErrorCode=0x%02lX, StopOK=%s\r\n"
```
dove:
- `Status` stampa `(int)s_metrics.stop_status` (il valore restituito da `HAL_I2S_DMAStop(&hi2s2)`).
- `ErrorCode` stampa `(unsigned long)s_metrics.stop_error_code` (il campo `hi2s2.ErrorCode`).
- `StopOK` stampa `"SI"` se `s_metrics.stop_ok == 1`, altrimenti `"NO"`.

---

### 1.3. Origine nel driver ST HAL (`stm32g4xx_hal_i2s.c`)

L'ispezione del driver ufficiale STMicroelectronics (`STM32Cube_FW_G4_V1.6.3\Drivers\STM32G4xx_HAL_Driver\Src\stm32g4xx_hal_i2s.c`) rivela il comportamento di `HAL_I2S_DMAStop`:

```c
/* Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.c:1425-1463 */

else if ((hi2s->Init.Mode == I2S_MODE_MASTER_RX) || (hi2s->Init.Mode == I2S_MODE_SLAVE_RX))
{
    /* Abort the I2S DMA rx Stream/Channel */
    if (hi2s->hdmarx != NULL)
    {
        /* Disable the I2S DMA rx Stream/Channel */
        if (HAL_OK != HAL_DMA_Abort(hi2s->hdmarx))
        {
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
            errorcode = HAL_ERROR;
        }
    }

    /* Disable I2S peripheral */
    __HAL_I2S_DISABLE(hi2s);

    /* Clear OVR flag */
    __HAL_I2S_CLEAR_OVRFLAG(hi2s);

    /* Disable the I2S Rx DMA request */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);

    if (hi2s->Init.Mode == I2S_MODE_SLAVE_RX)
    {
        /* Set the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_BUSY_LINE_RX);

        /* Set the I2S State ready */
        hi2s->State = HAL_I2S_STATE_READY;
        errorcode = HAL_ERROR;
    }
    else
    {
        /* Read DR to Flush RX Data */
        READ_REG((hi2s->Instance)->DR);
    }
}
hi2s->State = HAL_I2S_STATE_READY;
return errorcode;
```

Nel file di intestazione `stm32g4xx_hal_i2s.h` (riga 102):
```c
#define HAL_I2S_ERROR_BUSY_LINE_RX (0x00000040U) /*!< Busy Rx Line error */
```
e nel file `stm32g4xx_hal_def.h`:
```c
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;
```

---

### 1.4. Risposte puntuali ai quesiti del Mandato R3.1

1. **Quale funzione produce `Status`?**  
   Viene prodotto direttamente dalla funzione di libreria ST `HAL_I2S_DMAStop(&hi2s2)`.
2. **Quale significato ha `Status=1`?**  
   Corrisponde al valore dell'enum `HAL_ERROR` (`0x01U`).
3. **Da dove proviene `ErrorCode=0x40`? (C01-B-ERR-01)**  
   Proviene dalla macro `#define HAL_I2S_ERROR_BUSY_LINE_RX (0x00000040U)`.  
   Nel driver ST, quando la periferica è configurata come **I²S Slave Receiver** (`hi2s->Init.Mode == I2S_MODE_SLAVE_RX`), la funzione `HAL_I2S_DMAStop()` esegue **sistematicamente ed incondizionatamente**:
   ```c
   SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_BUSY_LINE_RX);
   errorcode = HAL_ERROR;
   ```
   Nel frammento di driver analizzato, il driver imposta tale codice **senza verificare esplicitamente il flag BSY**. La generazione di 0x40 e il ritorno di HAL_ERROR non sono quindi una misura attiva con cui il driver abbia testato la continuità del clock esterno, bensì l'esito predeterminato di quel ramo dell'implementazione HAL per Slave RX.
4. **Come viene determinato `StopOK=SI`? (C01-B-ERR-02)**  
   La logica diagnostica in `i2s_rx_diag.c` (righe 704–712) gestisce esplicitamente questa peculiarità del driver HAL:
   - Verifica direttamente le condizioni di disabilitazione hardware nel costrutto software: `(DMA1_Channel1->CCR & DMA_CCR_EN) == 0` (canale DMA spento) e `(SPI2->CR2 & SPI_CR2_RXDMAEN) == 0` (richiesta DMA I2S disabilitata).
   - Non attribuisce al criterio `StopOK` verifiche dirette di registri non presenti in tale if (`SPI2->CR1.SPE` o `I2SCFGR.I2SE`), le quali costituiscono letture diagnostiche dello snapshot post-stop separato.
   - Se il canale DMA e RXDMAEN sono disabilitati, e lo stato di errore corrisponde ad `HAL_I2S_ERROR_BUSY_LINE_RX` (oppure `HAL_OK` / `ERROR_NONE`), assegna **`s_metrics.stop_ok = 1`** (`StopOK=SI`).
5. **Il comportamento è previsto, anomalo o non determinabile?**  
   È **100% PREVISTO, DETERMINISTICO e DOCUMENTATO** nel codice sorgente e nell'HAL di ST. Non rappresenta alcuna anomalia fisica, né un guasto della ricezione. Durante i 10 secondi di streaming non si è verificato alcun errore (`HAL Error CB = 0`, `Raw IRQ SPI2 = 0`, `OVR = 0`). La segnalazione `0x40` si genera esclusivamente all'istante di stop $t=10.000\ \text{ms}$.
6. **La diagnostica può dichiarare il successo dello stop pur registrando uno stato HAL non nominale?**  
   Sì, perché l'autore del firmware diagnostico ha inserito la clausola specifica (riga 708) dopo aver accertato nei registri che il canale DMA e la periferica SPI2 sono stati effettivamente arrestati e che il buffer di ricezione è stato svuotato (`__HAL_I2S_FLUSH_RX_DR`).

---

## 2. Verifica R3-ERR-05 — Dimensione effettiva e Hash del Binario Normale

### 2.1. Verifica fisica diretta sul filesystem
L'ispezione diretta eseguita su `A:\Documenti\Tickform` ha rilevato i seguenti attributi oggettivi per il binario del firmware normale:

- **Percorso assoluto:** `A:\Documenti\Tickform\firmware\stm32g431\build\phase5\tickform_phase5_i2s_rx.bin`
- **Dimensione effettiva in byte:** **`43.756 byte`**
- **Hash SHA-256 completo:** **`04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`**
- **Data e ora di modifica file:** `2026-11-17T11:57:48.500812Z` (UTC) / `12:57:48` (CET)
- **Target associato:** Target CMake `tickform_phase5_i2s_rx` (sorgente `Core/Src/i2s_rx.c`, macro `-DTICKFORM_PHASE5_I2S_RX`).

---

### 2.2. Origine della discrepanza (43.748 vs 43.756 byte)
È stata eseguita una ricerca testuale su tutti i file storici e di log del repository:
1. Nel log di build originario [`firmware/stm32g431/build_phase5.log`](../../firmware/stm32g431/build_phase5.log) e nel rapporto di approvazione della build F5-A02-R2.1 ([`firmware/stm32g431/RAPPORTO_F5_A02_R2.md`](../../firmware/stm32g431/RAPPORTO_F5_A02_R2.md), tabella memoria) la dimensione della Flash è sempre stata indicata correttamente come:
   ```text
   43.756 B (33,38% di 128 KB) | SHA-256: 04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465
   ```
2. Anche nei successivi [`RAPPORTO_F5_D02.md`](../../firmware/stm32g431/RAPPORTO_F5_D02.md) e [`RAPPORTO_F5_D02_R2.md`](../../firmware/stm32g431/RAPPORTO_F5_D02_R2.md) la dimensione riportata era **43.756 byte**.
3. La cifra errata **43.748 byte** è comparsa per la prima volta come **refuso tipografico di trascrizione manuale** nella tabella riepilogativa del rapporto [`firmware/stm32g431/RAPPORTO_F5_D03.md`](../../firmware/stm32g431/RAPPORTO_F5_D03.md) (dove l'hash era identico, ma la cifra 56 è stata trascritta come 48).
4. Tale refuso è stato poi ripreso nel documento storico `TICKFORM_HANDOVER_REPOSITORY_GIT_GESTIONE_2026-11-18.md` e in alcune righe della revisione R2.

### 2.3. Conclusione
Il file binario non è mai cambiato: l'unico valore corretto e conforme all'hash SHA-256 `04B95777...` è **43.756 byte**.  
Tutti i documenti candidati R3.1 sono stati bonificati eliminando ogni occorrenza residua di 43.748 byte.

---

## 3. Verifica R3-ERR-01 — Unità di Misura DMA nel Run 1

Nel primo run anomalo F5-D03 (`Testo incollato(20261117-133519)(1).txt`):
- `CNDTR iniziale` = 2048
- `CNDTR finale` = 1752
- Differenza = $2048 - 1752 = 296$ trasferimenti.
- Il canale DMA1 Channel 1 è configurato con `PSIZE = 01b` (16 bit / Halfword) e `MSIZE = 01b` (16 bit / Halfword).
- Il volume di dati trasferito prima dello stallo ammonta quindi a:
  $$296\ \text{halfword} \times 2\ \text{byte/halfword} = 592\ \text{byte}$$
- La dicitura "296 byte" presente nella R3 era errata per confusione tra numero di trasferimenti a 16 bit e numero di byte.
- Nei documenti candidati R3.1 è stata adottata la formulazione rigorosa:
  *«CNDTR diminuito di 296 halfword (592 byte) prima dello stallo osservato; zero callback Half/Full».*

---

## 4. Verifica R3-ERR-02 — Distinzione Metodologica del Campionamento GPIO IDR

Nel firmware F5-D03, la funzione `run_pin_input_test()` viene invocata alla riga 715 di `i2s_rx_diag.c`, **a valle** dell'arresto del DMA (`HAL_I2S_DMAStop` alla riga 692).  
Pertanto:
1. La finestra di misura da 10 ms del GPIO IDR analizza lo stato elettrico statico o dinamico presente sui pin `PB12` e `PB13` **esclusivamente nei 10 millisecondi successivi all'arresto della prova**.
2. Il rilievo di livelli logici fissi HIGH (Run 1) o fissi LOW (Run 3) in tale finestra post-stop fotografa la condizione terminale del bus, ma **non dimostra direttamente né retroattivamente quale sia stato l'andamento continuo delle linee durante i precedenti 10 secondi di ricezione**.
3. I flood di centinaia di migliaia o milioni di interrupt SPI2 osservati nei run anomali 3 e 4 sono stati registrati con `OVR = 0` e `HAL I2S Error CB last = 0x00000000`. L'ipotesi che essi siano scaturiti da fluttuazioni, glitch o falsi contatti è tecnicamente plausibile e correlata con la successiva risoluzione meccanica, ma **non è una causa isolata o dimostrata strumentalmente**.
4. Nella revisione R3.1 tutte le asserzioni apodittiche sono state riformulate in rigorose descrizioni fenomenologiche.
