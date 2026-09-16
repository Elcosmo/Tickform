# Tickform — Architettura V0 consolidata

Baseline della specifica di avvio del 2026-09-16, non validazione sperimentale. Modifiche alle decisioni consolidate richiedono CHANGE REQUEST e decisione della supervisione.

## Catena e clock

WATCH → PIEZO → MCP6022 AFE → PCM1808 → I2S → STM32G431CBU6 → USB → PC.

PCM1808 stereo a Fs nominale 96 kHz. XO attivo 24,576 MHz; master mode MD1 = HIGH e MD0 = HIGH, 256 × Fs. Relazioni nominali: 24.576 MHz / 256 = 96 kHz; BCK = 64 × Fs = 6,144 MHz. Configurazione FMT e topologia reale del breakout vanno ispezionate prima del collegamento.

Il sample index è la base temporale fondamentale. BCK → TIM2 external clock; DS3231 INT/SQW 1 Hz → TIM2 input capture. I conteggi BCK tra fronti SQW consentiranno di stimare la frequenza audio effettiva rispetto al riferimento indipendente. Il DS3231 non genera il sample clock. Non utilizzare il timing USB come riferimento temporale.

| Segnale | Destinazione prevista |
| --- | --- |
| PCM1808 LRCK | PB12 / I2S2_WS |
| PCM1808 BCK | PB13 / I2S2_CK e PA0 / TIM2_ETR |
| PCM1808 DOUT | PB15 / I2S2_SD |
| DS3231 INT/SQW | PA1 / TIM2_CH2 |
| USB STM32 | PA11 / PA12 |

Il mapping è quello previsto dalla baseline; configurazione periferiche, alternate function, collegamenti e funzionamento reale dovranno essere verificati nel bring-up. Non è uno schema di cablaggio già validato. Il protocollo USB non è definito in M0; capacità di trasporto e continuità senza perdita saranno verificate nelle fasi dedicate.

## Metrologia e RAW

Target indicativo V0: circa ±0,5 s/day di errore strumentale sul rate o meglio. Target futuro V1: avvicinamento a circa ±0,1 s/day. Nessun target è una prestazione raggiunta; la risoluzione del conteggio non dimostra l'accuratezza del riferimento o del sistema.

Il waveform RAW deve restare sempre conservabile. Non ridurre prematuramente la catena a piezo → comparatore → impulso digitale. Il secondo canale non è inutile: può ospitare secondo piezo, secondo punto di misura, sensore ottico, riferimento o altro sensore analogico.

## AFE: baseline non congelata

MCP6022 dual op-amp: A per amplificazione piezo, B per buffer VREF circa metà alimentazione. Alimentazione iniziale probabilmente 5 V; VREF circa 2,5 V. Elementi disponibili/ipotizzati: 4,7 MΩ, 100 pF C0G/NP0, 1N4148 per esperimenti di protezione, feedback circa 100 kΩ, rete 10 kΩ / 1 kΩ, 33 kΩ, 100 kΩ, 220 kΩ e condensatori per shaping.

Non è uno schema definitivo e non fissa guadagno, protezione, accoppiamento ADC o valori finali. Servono ispezione del breakout e misure reali. Meccanica pickup ancora sperimentale.

## PC e sviluppo

PC parte integrante della V0, nessun display. Futura GUI: RAW waveform, timegrapher plot, BPH, rate, beat error, amplitude, FFT, waterfall, statistiche, sessioni, confronto fra posizioni, filtri software e debugging. Funzioni previste, non implementate.

Ordine: RAW acquisition → waveform viewer → event detection → BPH → rate → beat error → amplitude → timegrapher plot → analisi avanzata. I dati sintetici non dimostrano che gli algoritmi reali siano risolti.
