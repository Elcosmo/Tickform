# Tickform — Bring-up checklist

Strumento operativo vivo. Stato aggiornato al 2026-11-13: le voci spuntate attestano gli esiti consolidati della receiving inspection e dei Gate 1/2/3/4/5 riportati nel PROJECT_LOG. Spuntare solo con evidenza reale, data e riferimento al test; disponibilità dichiarata e acquisto non dimostrano ricezione o funzionamento. FASE 0 completa; FASE 1 COMPLETE (Gate 1/2/3/4 PASS); FASE 2 DS3231 standalone COMPLETE (I2C e SQW 1 Hz verificati); FASE 3 XO standalone COMPLETE (power-up 3.3 V e conteggio fronti PASS); **FASE 4 PCM1808 C clock bring-up COMPLETE (Gate LRCK 96 kHz PASS, CAD KiCad R0.1 APPROVATO)**. FASE 5 PCM1808 → STM32 NEXT.

## FASE 0 — Receiving inspection

Identificare e ispezionare ogni componente ricevuto. Fotografie e verifiche di ricezione sotto riportate appartengono a questa fase; procedere all'alimentazione nelle fasi successive solo dopo i controlli pertinenti. Non assumere lo schema dei breakout o le specifiche AliExpress come certe.

Esiti: supervisione, 2026-09-25; vedere PROJECT_LOG alla stessa data. Le voci PCM1808 fronte/retro attestano ispezione, non fotografie archiviate. La rete DS3231 D2 + R4 (201) conferma la precauzione: NON inserire CR2032 non ricaricabile; batteria non necessaria per V0. Nessun test ID è stato fornito per questi esiti. Ricezione/ispezione piezo registrata nel log: non esiste una corrispondente voce in questa checklist; Piezo connected resta non spuntata.

## FASE 1 — WeAct standalone

- [x] WeAct board received and identified
- [x] STM32G431CBU6 marking verified
- [x] Board inspected for damage
- [x] 3V3 verified
- [x] ST-Link communication verified
- [x] Minimal firmware flashed
- [x] MCU clock verified
- [x] USB enumeration verified

## FASE 2 — DS3231 standalone

- [x] DS3231 module received and identified
- [x] DS3231 board inspected
- [x] Battery/charging topology checked
- [x] INT/SQW pin identified
- [x] I2C communication verified
- [x] SQW configured to 1 Hz
- [x] SQW 1 Hz verified

## FASE 3 — XO standalone

- [x] 24.576 MHz XO received
- [x] XO marking documented
- [x] XO pinout verified before power-up
- [x] XO supply requirements verified

Esiti FASE 3, 2026-10-05 (PROJECT_LOG): XO usato identificato e validato funzionalmente; pin 4 GND, pin 5 OUT, pin 8 VCC, pin 1 OPEN. Supply verificata operativamente a 3.3 V, non specifica nominale del costruttore attestata; bypass 100 nF. Clock rilevato via PA0/TIM2_ETR AF14, 16 finestre PASS, accuratezza assoluta non misurata.

## FASE 4 — PCM1808 senza AFE (Breakout PCM1808 C)

- [x] PCM1808 breakout received (moduli A, B, C)
- [x] PCM1808 front inspected
- [x] PCM1808 rear inspected
- [x] Breakout topology inspected (R_onboard ~100 Ω su SCK; AGND pin 2 → GND 2 ~0,2 Ω su C; continuità 3.3V/GND presunte su C; pinout chip TSSOP-14 allineato a TI SLES177B Rev. Agosto 2015)
- [x] PCM1808 supply rails verified (5,0 V analogica da WeAct V+; 3,2 V digitale da WeAct 3V3)
- [x] MD0/MD1/FMT configuration understood (Master Mode 256 × Fs: pull-up 10 kΩ a 3,3 V su MD0/MD1; FMT pull-down interno a LOW)
- [x] MCLK/SCK present (24,576 MHz da XO via filo diretto)
- [x] LRCK present (96,000 kHz rilevati tramite cursori PulseView — Gate LRCK 96 kHz PASS)
- [x] BCK present (circa 16 periodi in 2,625 µs su PulseView, compatibile con 6,144 MHz nominali)

Esiti FASE 4, 12-13/11/2026 (PROJECT_LOG): Breakout C validato operativamente per la generazione clock Master 96 kHz. Schema CAD nativo KiCad 10 (Rev. R0.1) approvato tecnicamente da CHAT OVERVIEW. Causa anomalia vecchia scheda B: SCONOSCIUTA. Conversione audio A/D: NON ancora verificata (spostata a FASE 5 con cablaggio I2S e DMA su STM32).

## FASE 5 — PCM1808 → STM32

- [x] STM32 receives I2S (I²S2 Slave Receiver operativo a 96 kHz stereo su PB12/13/15; verificato in F5-D03)
- [x] DMA running (DMA1 Channel 1 in modalità circolare; CNDTR monitorato e decrementato nominalmente)
- [x] DMA stability verified (acquisizioni nominali di 10 secondi convalidate con firmware diagnostico F5-D03 con 3.750–3.751 callback Half/Full; stabilità continuativa di lunga durata e streaming RAW non ancora certificati)
- [ ] Collaudo formale firmware normale operativo F5-A02-R2.1 (tickform_phase5_i2s_rx.bin approvato crittograficamente; collaudo a banco con clock attivi pendente con log archiviato)
- [ ] Continuous 64-bit sample counter implemented (struttura dati predisposta a livello firmware, da convalidare operativamente nello streaming continuo)
- [ ] Overflow/error handling implemented (strumentazione di monitoraggio e reporting errori ed OVR verificata in F5-D03; ripristino e recupero automatico post-anomalia da implementare e collaudare)

Esiti FASE 5, 17-18/11/2026 (PROJECT_LOG): Qualifica del trasporto dati hardware I²S2→DMA completata con successo tramite diagnostica F5-D03 su banco con cablaggio ripristinato. Run nominali con 3750-3751 CB in 10s e zero errori durante l'acquisizione. Firmware in esecuzione durante le prove e i sei reboot: tickform_phase5_diag.bin. Collaudo del firmware normale F5-A02-R2.1 identificato come prossimo gate aperto obbligatorio prima della chiusura formale della fase. FASE 6 NON INIZIATA e NON AUTORIZZATA.

## FASE 6 — RAW → PC

- [ ] RAW stream received on PC
- [ ] RAW saved without sample loss

## FASE 7 — Clock metrology

- [ ] BCK connected to TIM2 external clock
- [ ] DS3231 SQW connected to TIM2 capture
- [ ] BCK counts captured between SQW edges
- [ ] Effective sample rate computed
- [ ] Clock correction metadata available

## FASE 8 — AFE

- [ ] AFE VREF verified
- [ ] AFE gain characterized
- [ ] AFE noise observed
- [ ] AFE clipping behavior observed

## FASE 9 — Piezo + orologio

- [ ] Piezo connected
- [ ] First real watch waveform acquired
- [ ] First real RAW dataset archived

## FASE 10 — DSP dopo dati affidabili

Ordine previsto: BPH → rate → beat error → amplitude. Event detection e waveform viewer seguono acquisizione RAW affidabile; timegrapher plot e analisi avanzata verranno dopo. Nessun algoritmo è dichiarato validato da dati sintetici.

## Test IDs e tracciabilità

Usare ID univoci del tipo `TG-V0-TEST-001` per i test importanti. Assegnare l'ID all'esecuzione reale, conservandolo nei metadati e nelle evidenze. Per ogni revisione hardware importante fotografare fronte e retro, registrare revisione hardware, firmware revision, condizioni del test, strumenti usati e risultati distinguendo ipotesi e osservazioni. Non creare risultati o registrazioni fittizie.

Collocare note degli esperimenti in `experiments/`, fotografie in `hardware/v0_perfboard/photos/` e misure in `hardware/v0_perfboard/measurements/`. Le acquisizioni lunghe restano fuori dal repository: conservare nelle note un riferimento riproducibile all'archivio, senza percorsi personali o credenziali. Soltanto piccoli dataset selezionati possono entrare in `data/examples/`.

Esempio futuro di naming, non file esistenti:

- `2026-09-XX_Molnija3602_DU_001.raw`
- `2026-09-XX_Molnija3602_DU_001.json`
