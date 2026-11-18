# Tickform — Context Handoff per Nuove Sessioni / Supervisione

Documento di sintesi operativa per il passaggio rapido di contesto. Per la trattazione tecnica completa consultare `docs/TICKFORM_PROJECT_MEMORY.md` (Revisione R3.1).

- **Data:** 18 novembre 2026
- **Root repository:** `A:\Documenti\Tickform`
- **Branch Git attivo:** `main` (HEAD = `e857849debbc54e655a8a87a88c07a2ee0386d43`, allineato a `origin/main`)
- **Stato Working Tree:** 4 file tracciati modificati (`CMakeLists.txt`, `stm32g4xx_hal_conf.h`, `main.c`, `build.ps1`), sorgenti I²S untracked protetti da backup verificato su disco B: e D:. Nessun push autorizzato.
- **Stato Progetto:** FASE 5 (PCM1808 → STM32 I²S2 Slave DMA). Qualifica del trasporto dati hardware e diagnostica interna F5-D03 COMPLETATA con successo (acquisizioni nominali di 10s con 3.750–3.751 CB e zero errori). Collaudo del firmware normale F5-A02-R2.1 in attesa di mandato formale.
- **FASE 6 (Streaming RAW verso PC):** RIGOROSAMENTE NON INIZIATA E NON ANCORA AUTORIZZATA.

## Punti Chiave da Sapere Immediatamente
1. **Evidenze di Laboratorio F5-D03 Riconciliate:**
   - 5 file Tera Term archiviati in `docs/evidenze_f5_d03/` con hash SHA-256 verificati.
   - 6 run diagnostici distinti censiti: run anomali iniziali (Run 1: CNDTR diminuito di 296 halfword/592 byte prima dello stallo; Run 3 e 4 con flood di interrupt); run nominali finali regolari con 3.750–3.751 callback DMA in 10s e ZERO errori SPI2/HAL.
   - Le ritrasmissioni periodiche CDC (stessi contatori ed eventi) non sono prove indipendenti.
2. **Stop I²S (`Status=1, ErrorCode=0x40, StopOK=SI`):**
   - Comportamento predeterminato del driver ST HAL (`HAL_I2S_DMAStop`), che nel ramo Slave RX imposta incondizionatamente `0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) senza verificare esplicitamente il flag BSY (C01-B-ERR-01).
   - Il firmware diagnostico valida lo stop (`StopOK=SI`) verificando che il canale DMA e RXDMAEN siano disabilitati a registro (C01-B-ERR-02). Zero errori durante i 10s di streaming.
3. **Rettifica Firmware dei Sei Reboot:**
   - Durante i 6 riavvii (3 reset NRST + 3 cicli USB) la STM32 eseguiva il firmware diagnostico `tickform_phase5_diag.bin` (F5-D03, SHA-256 `29786361...`), NON il firmware normale `tickform_phase5_i2s_rx.bin`.
   - Il firmware normale è approvato e compilato (43.756 byte, SHA-256 `04B95777...`), ma NON è ancora stato collaudato a banco con i clock attivi.
4. **Campionamento GPIO IDR post-stop:**
   - Frequenza reale misurata: 1,9–2,1 MSps. La misura avviene post-stop: PB12 (LRCK 96 kHz) campionato correttamente (~1920 transizioni in 10 ms). PB13 (BCK 6,144 MHz) sotto Nyquist: le transizioni campionate risentono di aliasing e zero transizioni non dimostrano assenza di clock.
5. **Hardware KiCad R0.1 vs Banco:**
   - Progetto KiCad R0.1 integro e conforme per FASE 4 (ERC: 0 errori, 0 avvisi). Non include PB12/13/15 e DS3231.
   - Resistenze pull-up MD0/MD1: nel CAD sono due da 10 kΩ; sul banco fisico l'operatore ricorda un cavo a Y e le resistenze non sono state misurate con multimetro (livello HIGH accertato operativamente).
6. **Prossima Attività da Autorizzare:**
   - Mandato di flash e collaudo del firmware normale `tickform_phase5_i2s_rx.bin` (10s di acquisizione con log seriale salvato).
   - Rilievo fotografico e misura delle resistenze AS BUILT prima del passaggio a perfboard e KiCad R0.2.
