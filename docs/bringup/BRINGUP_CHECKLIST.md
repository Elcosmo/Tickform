# Tickform — Bring-up checklist

Strumento operativo vivo. Tutte le voci sono inizialmente non verificate. Spuntare solo con evidenza reale, data e riferimento al test; disponibilità dichiarata e acquisto non dimostrano ricezione o funzionamento. M0 non esegue queste fasi.

## FASE 0 — Receiving inspection

Identificare e ispezionare ogni componente ricevuto. Fotografie e verifiche di ricezione sotto riportate appartengono a questa fase; procedere all'alimentazione nelle fasi successive solo dopo i controlli pertinenti. Non assumere lo schema dei breakout o le specifiche AliExpress come certe.

## FASE 1 — WeAct standalone

- [ ] WeAct board received and identified
- [ ] STM32G431CBU6 marking verified
- [ ] Board inspected for damage
- [ ] 3V3 verified
- [ ] ST-Link communication verified
- [ ] Minimal firmware flashed
- [ ] MCU clock verified
- [ ] USB enumeration verified

## FASE 2 — DS3231 standalone

- [ ] DS3231 module received and identified
- [ ] DS3231 board inspected
- [ ] Battery/charging topology checked
- [ ] INT/SQW pin identified
- [ ] I2C communication verified
- [ ] SQW configured to 1 Hz
- [ ] SQW 1 Hz verified

## FASE 3 — XO standalone

- [ ] 24.576 MHz XO received
- [ ] XO marking documented
- [ ] XO pinout verified before power-up
- [ ] XO supply requirements verified

## FASE 4 — PCM1808 senza AFE

- [ ] PCM1808 breakout received
- [ ] PCM1808 front photographed
- [ ] PCM1808 rear photographed
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
