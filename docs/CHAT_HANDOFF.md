# Tickform — Context Handoff per Nuove Sessioni / Supervisione

Documento di sintesi operativa per il passaggio rapido di contesto. Per la trattazione tecnica completa consultare `docs/TICKFORM_PROJECT_MEMORY.md` e `docs/TICKFORM_FULL_PROJECT_HANDOFF.md`.

- **Data:** 20 novembre 2026
- **Root repository:** `A:\Documenti\Tickform`
- **Branch Git attivo:** `main` (HEAD = `7d714302867f93a5351c12164b412b08d0ae2e98`, allineato a `origin/main` su GitHub)
- **Stato Working Tree:** modifiche documentali coordinate post-collaudo nominale FASE 5 (F5-CLOSEOUT-DOC), preparazione Gate H01 e archiviazione evidenze F5-N01..N03 in `docs/evidenze_f5_n01_n03/`. Nessun push o commit non autorizzato.
- **Stato Progetto:** FASE 5 (PCM1808 → STM32 I²S2 Slave DMA). Qualifica nominale del firmware normale F5-A02-R2.1 COMPLETATA con successo (F5-N01-R1 PASS, F5-N02-R1 PASS, F5-N03 PASS post-power cycle; 3.750–3.751 CB e zero errori). Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03; il contenuto audio, il formato effettivo dei campioni e il sincronismo completo non sono ancora validati. FASE 5 NON ancora formalmente chiusa (requisiti aperti: contatore continuo a 64 bit dei campioni e recupero attivo overflow/errori).
- **Gate H01 (Priorità Immediata):** Ricognizione e stabilizzazione hardware a banco dei collegamenti prima di qualsiasi nuova fase.
- **FASE 6 (Streaming RAW verso PC):** RIGOROSAMENTE NON INIZIATA E NON ANCORA AUTORIZZATA.

## Punti Chiave da Sapere Immediatamente
1. **Evidenze di Laboratorio F5-D03 e F5-N01..N03 Riconciliate:**
   - Diagnostica F5-D03: 5 file Tera Term archiviati in `docs/evidenze_f5_d03/` con hash SHA-256 verificati (6 run censiti).
   - Collaudo normale F5-N01..N03: 3 file log Tera Term originali recuperabili archiviati in `docs/evidenze_f5_n01_n03/` con manifesto crittografico SHA-256 (`F5_N02_RESET.txt`, `F5_N02_R1_RESET.txt`, `F5_N03_POWER_CYCLE.txt`). F5-N01 (FAIL al primo boot per connettore DuPont scollegato a banco; collegamento esatto non identificato nella documentazione) e F5-N01-R1 (PASS dopo ripristino del contatto) privi di log Tera Term salvati, con esiti provenienti direttamente dai rapporti operativi trasmessi dall'operatore alla OVERVIEW.
   - Le ritrasmissioni periodiche USB CDC ogni 5 s non costituiscono acquisizioni indipendenti.
2. **Stop I²S (`Status=1, ErrorCode=0x40, StopOK=SI`):**
   - Comportamento predeterminato del driver ST HAL (`HAL_I2S_DMAStop`), che nel ramo Slave RX imposta incondizionatamente `0x40` (`HAL_I2S_ERROR_BUSY_LINE_RX`) senza verificare esplicitamente il flag BSY (C01-B-ERR-01).
   - Il firmware diagnostico valida lo stop (`StopOK=SI`) verificando che il canale DMA e RXDMAEN siano disabilitati a registro (C01-B-ERR-02). Zero errori durante i 10s di streaming.
3. **Firmware Normale Collaudato con Successo:**
   - Il binario `tickform_phase5_i2s_rx.bin` (43.756 byte, SHA-256 `04B9577710B948BAF02331178EFA399A6099BFAFFAF41D23BAED49823B767465`) ha superato la qualifica nominale.
   - Sono stati osservati valori RAW non nulli in F5-N02-R1 e valori zero in F5-N03. Il contenuto audio, il formato effettivo dei campioni e il sincronismo completo non sono ancora validati.
4. **Campionamento GPIO IDR post-stop:**
   - Frequenza reale misurata: 1,9–2,1 MSps. La misura avviene post-stop: PB12 (LRCK 96 kHz) campionato correttamente (~1920 transizioni in 10 ms). PB13 (BCK 6,144 MHz) sotto Nyquist: le transizioni campionate risentono di aliasing e zero transizioni non dimostrano assenza di clock.
5. **Hardware KiCad R0.1 vs Banco AS BUILT:**
   - Progetto KiCad R0.1 integro e conforme per FASE 4 (ERC: 0 errori, 0 avvisi). Non include PB12/13/15 e DS3231.
   - Conferme operatore AS BUILT: GND ST-Link connesso a J1.3 GND del PCM1808; oscillatore XO saldato su millefori con condensatore di bypass ceramico da 100 nF; MD0/MD1 con configurazione fisica confermata dall'operatore a Y con 10 kΩ per ramo verso 3,3 V (livello HIGH confermato; verifica strumentale ohmica indipendente non effettuata, demandata al Gate H01); modulo DS3231 privo di batteria CR2032 non ricaricabile.
6. **Prossima Attività da Autorizzare:**
   - Gate H01: ricognizione strumentale, serraggio e stabilizzazione meccanica/elettrica dei jumper DuPont, masse e alimentazioni a banco prima di qualsiasi nuova fase.
   - Requisiti firmware residui FASE 5 (contatore continuo 64-bit e recupero attivo da overflow/errori) da indirizzare prima della chiusura formale di FASE 5.
   - Requisito futuro di telemetria: Boot ID, Test ID progressivo, Uptime.
   - FASE 6 rigorosamente non iniziata e non autorizzata.
