# ADR-004 — Metrologia BCK / DS3231 tramite TIM2

Status: Accepted

Date: 2026-09-16

## Context

Il clock audio nominale non dimostra la frequenza reale. Il timing del trasporto USB non è un riferimento metrologico.

## Decision

Contare PCM1808 BCK tramite TIM2 external clock su PA0/TIM2_ETR; acquisire DS3231 INT/SQW 1 Hz su PA1/TIM2_CH2. Stimare la frequenza effettiva della catena audio dai conteggi fra fronti SQW.

## Alternatives considered

Non utilizzare il timing USB. DS3231 come riferimento indipendente, non generatore diretto del sample clock. Nessun altro riferimento è selezionato per V0.

## Consequences

Verificare conteggi, capture, overflow e accuratezza del riferimento. Rendere disponibile la correzione nei metadati. Target indicativo V0 circa ±0,5 s/day o meglio sul rate, non prestazione ottenuta; risoluzione e accuratezza restano distinte.
