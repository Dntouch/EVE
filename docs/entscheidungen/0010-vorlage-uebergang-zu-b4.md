# 0010 – Übergang zu B.4: erste rückmeldungsabhängige Plastizität

**Status:** Entwurf – keine Freigabe zur Gewichtsänderung

**Datum:** 2026-11-18

## Kontext

B.3 weist nach, dass abgestufte Bewertungen über allgemeine neuronale Wege
gültige Eligibility erreichen können. F2/F3 erreichen jedoch wesentlich mehr
Neuronen ohne vorausgehende Markierung und erzeugen selbst zahlreiche neue
Eligibility-Entscheidungen. Lokale Generationen bilden keine gemeinsame
Handlungs-ID. Z0/Z1 erlauben daher noch keine zuverlässige Zuordnung, Z2 ist
offen.

## Optionen

### A – Alle erreichten Markierungen verändern

Abzulehnen. Die geringe Selektivität aus B.2 und die unselektive Verteilung aus
B.3 würden miteinander multipliziert. Rückmeldungsinduzierte Markierungen und
falsche Handlungskontexte könnten Gewichte ändern.

### B – B.3.1: neuronalen Handlungskontext erforschen

Vor Plastizität wird ein kleiner, regulär neuronaler Kontextweg untersucht,
der mehrere lokale Entscheidungen einer Handlung zuordnen kann, ohne
technische Handlungs-ID oder Kausalitätsliste.

### C – Streng begrenzte B.4-Sicherheitsprobe

Nur vorausgehende, nicht rückmeldungsinduzierte Eligibility darf teilnehmen.
Mehrdeutige lokale Zustände, abgelaufene Markierungen, fehlende Handlung und
überlappende Rückmeldungen werden ausgeschlossen. Gewichtsänderung bleibt
minimal, budgetiert und vollständig reversibel untersucht.

## Empfehlung

Option B wird bevorzugt. Option C ist erst nach ausdrücklicher Freigabe und
mit B als Pflichtkontrolle vertretbar. B.4 darf insbesondere nicht „alle
markierten Synapsen belohnen“.

## Mindestanforderungen an eine spätere B.4-Freigabe

- neuronale statt technische Handlungskontextinformation,
- Ausschluss von `feedback_origin`-Markierungen,
- eindeutige Regel für keine/eine/mehrere gültige Generationen,
- getrennte Behandlung von L1-Nullgewichten und L2-Positivgewichten,
- Gewichtsbudget, Sättigung, Abschwächung und Persistenz,
- F0/S0/S3/S4/S5/S6 sowie fehlende und vertauschte Rückmeldungen,
- Nachweis, dass Diagnoseinformationen keine Synapse auswählen,
- Stabilitätsläufe über mindestens 128 Handlungen.

## Nachweise

- [Forschungsbericht B.3](../experimente/010-forschungsbericht-forschungsstufe-b3.md)
- [Technische Rückmeldungsspezifikation](../konzept/EVE-Neuronale-Bewertungsrueckmeldung.md)
- `research/results/research-stage-b3-routes.csv`
- `research/results/research-stage-b3-delays.csv`
- `research/results/research-stage-b3-sequences.csv`

