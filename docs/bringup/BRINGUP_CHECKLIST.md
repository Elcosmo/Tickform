# Tickform — Bring-up checklist

Strumento operativo vivo. Stato aggiornato al 2026-09-26: le voci spuntate attestano gli esiti consolidati della receiving inspection e dei Gate 1/2 riportati nel PROJECT_LOG. Spuntare solo con evidenza reale, data e riferimento al test; disponibilità dichiarata e acquisto non dimostrano ricezione o funzionamento. FASE 0 completa; FASE 1 in corso, Gate 1/2 PASS sulla base dei test fisici comunicati dalla supervisione; clock MCU e USB nativa ancora da verificare.

## FASE 0 — Receiving inspection

Identificare e ispezionare ogni componente ricevuto. Fotografie e verifiche di ricezione sotto riportate appartengono a questa fase; procedere all'alimentazione nelle fasi successive solo dopo i controlli pertinenti. Non assumere lo schema dei breakout o le specifiche AliExpress come certe.

Esiti: supervisione, 2026-09-25; vedere PROJECT_LOG alla stessa data. Le voci PCM1808 fronte/retro attestano ispezione, non fotografie archiviate. La rete DS3231 D2 + R4 (201) conferma la precauzione: NON inserire CR2032 non ricaricabile; batteria non necessaria per V0. Nessun test ID è stato fornito per questi esiti. Ricezione/ispezione piezo registrata nel log: non esiste una corrispondente voce in questa checklist; Piezo connected resta non spuntata.

## FASE 1 — WeAct standalone

- [x] WeAct board received and identified
- [x] STM32G431CBU6 marking verified
- [x] Board inspected for damage
- [ ] 3V3 verified
- [x] ST-Link communication verified
- [x] Minimal firmware flashed
- [ ] MCU clock verified
- [ ] USB enumeration verified

## FASE 2 — DS3231 standalone

- [x] DS3231 module received and identified
- [x] DS3231 board inspected
- [x] Battery/charging topology checked
- [x] INT/SQW pin identified
- [ ] I2C communication verified
- [ ] SQW configured to 1 Hz
- [ ] SQW 1 Hz verified

## FASE 3 — XO standalone

- [x] 24.576 MHz XO received
- [x] XO marking documented
- [ ] XO pinout verified before power-up
- [ ] XO supply requirements verified

## FASE 4 — PCM1808 senza AFE

- [x] PCM1808 breakout received
- [x] PCM1808 front inspected
- [x] PCM1808 rear inspected
- [ ] Breakout topology inspected
- [ ] PCM1808 supply rails verified
- [ ] MD0/MD1/FMT configuration understood
- [ ] MCLK/SCK present
- [ ] LRCK present
- [ ] BCK present

## FASE 5 — PCM1808 → STM32

- [ ] STM32 receives I2S
- [ ] DMA running
- [ ] DMA stability verified
- [ ] Continuous 64-bit sample counter implemented
- [ ] Overflow/error handling implemented

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
