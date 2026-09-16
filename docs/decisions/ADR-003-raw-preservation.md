# ADR-003 — Preservazione del waveform RAW

Status: Accepted

Date: 2026-09-16

## Context

La sola estrazione di eventi può eliminare informazioni necessarie per analisi, debugging e riproducibilità.

## Decision

Il RAW deve restare sempre disponibile e conservabile. Il sample index è la base temporale; mantenere tracciabilità e metadati delle acquisizioni.

## Alternatives considered

La catena ridotta piezo → comparatore → impulso digitale non è adottata come sostituto prematuro del RAW.

## Consequences

Registrazioni lunghe fuori Git; solo piccoli esempi selezionati in data/examples. Gli algoritmi devono poter essere riesaminati a partire dai dati originali.
