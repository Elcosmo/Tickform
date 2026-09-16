# ADR-006 — XO V0 e possibile riferimento migliore V1

Status: Accepted

Date: 2026-09-16

## Context

Sono previsti XO economici attivi 24,576 MHz, package tipo DIP/DIP-8 footprint; le caratteristiche reali sono da verificare.

## Decision

Usare XO 24,576 MHz in V0. Considerare possibile TCXO o riferimento migliore in V1; il futuro PCB V1 dovrà considerare EXT REF/PPS.

## Alternatives considered

TCXO/riferimento migliore rinviati alla valutazione V1; nessun componente alternativo specifico selezionato.

## Consequences

Identificare pinout e alimentazione prima di alimentare il componente. Non progettare ora il PCB. Il target futuro di circa ±0,1 s/day sul rate è indicativo e non raggiunto.
