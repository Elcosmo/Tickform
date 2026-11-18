# Tickform

> **Punto d'ingresso per la supervisione e nuovi agenti:** Per avviare una nuova sessione di supervisione (CHAT OVERVIEW) o comprendere rapidamente lo stato attuale, le evidenze consolidate e le regole di continuità, consultare [PROMPT.md](PROMPT.md).

Tickform è un progetto DIY destinato a essere sviluppato e pubblicato come progetto open-source/open-hardware per realizzare un timegrapher avanzato per orologi meccanici. La strategia di licenza è ancora in review; le licenze non sono state formalmente adottate. L'obiettivo è conservare e analizzare il waveform acquisito dal pickup, mantenendo tracciabilità e riproducibilità delle misure.

Stato: **early development / repository bootstrap**. Hardware e software non sono ancora validati; le specifiche richiedono verifica sperimentale. La strategia di licenza è ancora in revisione: vedere [LICENSE](LICENSE), che non è una licenza finale.

Le misure e visualizzazioni previste includono BPH, rate, beat error, amplitude, timegrapher plot e waveform RAW. In futuro potranno essere analizzati gli eventi di scappamento, se fisicamente osservabili nei segnali reali. Queste funzionalità non sono ancora implementate.

## Architettura V0

WATCH → PIEZO → MCP6022 analog front end → PCM1808 stereo @ 96 kHz → I2S → STM32G431CBU6 (WeAct Core Board) → USB → PC.

XO attivo 24,576 MHz → PCM1808 master, 256 × Fs. Nell'architettura V0 è previsto che il BCK venga contato da TIM2 e riferito al DS3231 INT/SQW a 1 Hz per stimare la frequenza effettiva della catena audio. Il DS3231 non genera il sample clock. Il sample index è la base temporale fondamentale; il timing USB non è un riferimento metrologico. V0 non prevede un display: il PC è parte integrante del sistema.

Il RAW deve restare conservabile. Il secondo canale stereo rimane disponibile per un secondo piezo, un altro punto di misura, un sensore ottico, un riferimento o un altro sensore analogico. AFE definitivo e meccanica del pickup sono ancora aperti.

## Documentazione

- [Prompt di supervisione](PROMPT.md) (punto d'ingresso per CHAT OVERVIEW e continuità operativa).
- [Memoria centrale persistente](docs/TICKFORM_PROJECT_MEMORY.md) e [passaggio di consegne completo](docs/TICKFORM_FULL_PROJECT_HANDOFF.md).
- [Stato corrente](docs/PROJECT_STATE.md), [log append-only](docs/PROJECT_LOG.md) e [passaggio di contesto](docs/CHAT_HANDOFF.md).
- [Rapporto di consolidamento C01](docs/TICKFORM_CONSOLIDATION_C01_REPORT.md).
- [Architettura V0](docs/architecture/V0_BASELINE.md) e [ADR accettati](docs/decisions/).
- [Checklist di bring-up](docs/bringup/BRINGUP_CHECKLIST.md).
- [Formato RAW draft v0.1](docs/data-format/RAW_FORMAT_V0_1.md).
- [BOM as purchased e costi](bom/V0.0_AS_PURCHASED.md).

`hardware/`, `firmware/`, `software/`, `protocol/`, `data/`, `experiments/`, `tests/` e `tools/` riservano gli spazi di lavoro futuri. I README locali ne definiscono il perimetro e mantengono le directory in Git senza `.gitkeep` ripetitivi.

Prima documentiamo lo stato e costruiamo una pipeline affidabile; poi misuriamo; solo dopo ottimizziamo.
