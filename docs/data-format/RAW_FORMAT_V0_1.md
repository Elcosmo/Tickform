# Tickform RAW Recording Format — Draft v0.1

**DRAFT — NON CONGELATO.** Proposta documentale iniziale; nessun writer, reader o dataset è implementato in M0. Il formato di registrazione su disco non definisce il protocollo USB né congela il packet framing firmware.

Una registrazione è composta dalla coppia `<recording>.raw` e `<recording>.json`: waveform e metadati con lo stesso nome base. Il RAW originale deve restare conservabile anche quando verranno applicati filtri o analisi.

## Metadati iniziali

Esempio di schema concettuale richiesto, non metadati di un test eseguito. `null` significa non disponibile/non misurato, non zero. L'ID mostrato non assegna un test reale.

```json
{
  "format_version": "0.1",
  "test_id": "TG-V0-TEST-001",
  "hardware_revision": "V0.0",
  "firmware_revision": null,
  "adc": "PCM1808",
  "sample_rate_nominal_hz": 96000,
  "sample_rate_corrected_hz": null,
  "sample_format": "s24le",
  "channels": 2,
  "sample_counter_start": 0,
  "clock_source": "24.576MHz_XO",
  "clock_reference": "DS3231_SQW",
  "watch": null,
  "movement": null,
  "position": null,
  "lift_angle_deg": null,
  "pickup": null,
  "gain": null,
  "duration_s": null,
  "notes": ""
}
```

| Campo | Significato concettuale |
| --- | --- |
| `format_version` | Versione del formato proposto; `0.1` identifica questo draft. |
| `test_id` | ID del test che collega registrazione, hardware, condizioni ed evidenze. |
| `hardware_revision` | Revisione della configurazione hardware realmente impiegata. |
| `firmware_revision` | Identificatore della revisione firmware utilizzata; non noto in M0. |
| `adc` | Modello dell'ADC previsto/utilizzato. |
| `sample_rate_nominal_hz` | Frequenza nominale dei campioni per canale, in Hz. |
| `sample_rate_corrected_hz` | Stima della frequenza effettiva riferita alla metrologia; `null` fino a disponibilità di una misura valida. |
| `sample_format` | Formato proposto `s24le`: interi con segno a 24 bit, little-endian. Packing e ordine dei canali vanno precisati prima dell'implementazione. |
| `channels` | Numero di canali, due nella baseline stereo. |
| `sample_counter_start` | Indice iniziale nella base temporale dei campioni; previsto contatore continuo a 64 bit. Definizione formale per frame stereo e rappresentazione JSON dei grandi interi da congelare. |
| `clock_source` | Origine del clock audio: XO 24,576 MHz. |
| `clock_reference` | Riferimento indipendente per la stima della frequenza: DS3231 SQW; non implica misura già effettuata. |
| `watch` | Identificazione dell'orologio in prova, quando disponibile. |
| `movement` | Movimento dell'orologio, quando noto. |
| `position` | Posizione fisica durante l'acquisizione; convenzioni da documentare. |
| `lift_angle_deg` | Angolo di levata in gradi, se noto; non inventarlo per il calcolo dell'amplitude. |
| `pickup` | Identificazione/configurazione del pickup realmente impiegato. |
| `gain` | Guadagno/configurazione AFE usati; unità e rappresentazione da definire. |
| `duration_s` | Durata in secondi; criterio di derivazione da campioni e frequenza da precisare, non timing USB. |
| `notes` | Note e condizioni utili alla riproducibilità, senza dati personali o secret. |

## Questioni aperte

Packing esatto su disco, interleaving/ordine dei canali, semantica formale del contatore stereo, grandi interi JSON, gestione delle discontinuità e dei campioni persi, rappresentazione della correzione clock e sua validità nel tempo richiedono definizione successiva. Non inferire che campi nominali o `null` dimostrino continuità o accuratezza.

Conservare le registrazioni lunghe fuori Git. Piccoli esempi scelti potranno essere versionati in `data/examples/` insieme ai JSON e alle condizioni del test. Vedere la [checklist](../bringup/BRINGUP_CHECKLIST.md) per test ID e tracciabilità. Nessun dato fittizio viene creato con questo draft.
