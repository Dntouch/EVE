# 0009 – Rückmeldungsabhängige synaptische Veränderung nach B.2

**Status:** Entwurf – keine Implementierungsfreigabe

**Datum:** 2026-11-18

## Kontext

B.2 zeigt, dass Feuerentscheidungen lokal, generationsbezogen und zeitlich
begrenzt mögliche Eingangssynapsen markieren können. L2 mit zwei eindeutigen
Kandidaten erfasste in der M3-Matrix alle technisch beobachteten relevanten
Schaltungseinträge und war selektiver als L1.

Der Befund reicht nicht für eine Gewichtsänderung. Selbst L2/K1 erreichte nur
9,79 % technische Selektivität. Außerdem kann L2 Nullgewicht-Synapsen nicht als
Kandidaten führen, obwohl eine spätere Lernregel gerade solche Verbindungen
aufbauen wollen könnte. Eligibility sagt weder Erfolg noch Richtung oder
Stärke einer Änderung voraus.

## Noch zu entscheidende Fragen

1. Wie erreicht ein neuronales Rückmeldungsereignis die betroffenen Neuronen,
   ohne technische M3-Diagnose zurückzuschreiben?
2. Wie bindet es sich an die richtige Handlung und Entscheidungsgeneration?
3. Sollen nur vorhandene positive Verbindungen oder auch Nullgewichte
   veränderbar sein?
4. Wie werden falsche positive Kandidaten begrenzt?
5. Welche lokale Größe bestimmt Verstärkung, Abschwächung und Änderungsbetrag?
6. Welche Gewichtsbudgets, Sättigungen und Konkurrenzregeln bleiben erhalten?
7. Wie werden verzögerte, widersprüchliche oder fehlende Rückmeldungen behandelt?
8. Welche Kontrollen trennen tatsächliches Lernen von Exploration,
   Membranzustand und zufälligem Abschluss?

## Optionen

### A – Direkte technische Änderung markierter Synapsen

Das BIOS verändert bei positiver oder negativer Bewertung alle gerade
markierten Kandidaten. Diese Option ist abzulehnen: Sie vermischt Diagnose und
neuronalen Mechanismus und würde die geringe Selektivität ungeprüft in
Gewichtsänderungen übersetzen.

### B – Neuronale Rückmeldungsroute mit lokaler Eligibility

Ein separates sensorisches Signal erreicht Neuronen über reguläre synaptische
Wege. Nur das Zusammentreffen dieses Signals mit einer gültigen lokalen
Generation kann eine spätere, noch festzulegende Änderung anstoßen.

Diese Option entspricht der Architektur, benötigt aber vor jeder
Gewichtsänderung eine eigene Forschungsstufe für Route, Zuordnung und
Stabilitätsgrenzen.

### C – Weitere Selektivitätsforschung ohne Gewichtsänderung

Zusätzliche lokale Merkmale werden verglichen, etwa Membranbeitrag,
Impulsanzahl oder getrennte Entscheidungsplätze. Diese Option reduziert das
Risiko falscher Änderungen, verschiebt aber die Lernwirkung.

## Vorgeschlagene Entscheidung

Option B vorbereiten und mit C absichern. Zunächst ist eine ausschließlich
diagnostische Forschungsstufe für die neuronale Rückmeldungsroute und die
Generationenzuordnung durchzuführen. Automatische Verstärkung oder
Abschwächung bleibt bis zu einer erneuten ausdrücklichen Entscheidung
deaktiviert.

## Mindestkontrollen vor Freigabe

- Rückmeldung ohne Handlung und nach Ablauf,
- falsche beziehungsweise frühere Handlungsgeneration,
- L1 gegen L2 und Nullgewicht-Kandidaten,
- unterbrochener M3-Pfad und Abschluss ohne Daten,
- R1 gegen R2F sowie 2/8/32/128 fortgeführte Handlungen,
- unveränderte Referenz bei deaktivierter Lernwirkung,
- Gewichtsbudget, Sättigung und Persistenz,
- Nachweis, dass technische Kausalitätsdaten niemals neuronalen Zustand setzen.

## Nachweise

- [Forschungsbericht B.2](../experimente/009-forschungsbericht-forschungsstufe-b2.md)
- [Eligibility-Spezifikation](../konzept/EVE-Lokale-neuronale-Lernberechtigung.md)
- `research/results/research-stage-b2-selection.csv`
- `research/results/research-stage-b2-feedback-timing.csv`
- `research/results/research-stage-b2-repetitions.csv`

