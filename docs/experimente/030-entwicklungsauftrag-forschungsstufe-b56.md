# Entwicklungsauftrag 030 – Forschungsstufe B.5.6

## Kausale Analyse der verbleibenden nichtrobusten Lernverläufe

**Ausgangsbasis:** B.5.5 / S4-d16
**Forschungsbranch:** `codex/research-stage-b56-nonrobust-learning-causality`
**Status:** umgesetzt

## Ziel

Die unter S4-d16 verbleibenden nichtrobusten Netze werden ohne neue vorab
freigegebene Lern-, Hemmungs- oder Gedächtnisregel kausal zerlegt. Zu trennen
sind unvollständiger Erwerb, sensorische Speicherung, inhibitorische
Konkurrenz, transiente Abrufzustände, Zeitgrenzen und Erfahrungsfolgen.

## Präregistrierter Umfang

- historische Entdeckung: 64 Seeds × 3 Familien × 2 Zuordnungen,
- historische Bestätigung: 64 Seeds × 3 Familien × 2 Zuordnungen,
- frische Bestätigung: 64 Seeds × 3 Familien × 2 Zuordnungen,
- S4-d16 als Basis; S2, S5 und No-Learning als Kontrollen,
- N1–N8 als überlappende Fehlerklassen,
- erste vier robuste und erste vier nichtrobuste Seeds je Zelle als
  Detailstichprobe,
- vollständige Lernbiografien, Checkpoints 128/256/384/512 und
  Stabilitätsoffsets 1/4/8/16/32/64/128,
- sensorische, inhibitorische und transiente Zustandskopien,
- acht S/I/T-Faktorkopien V0–V7,
- natürliche Fortsetzungen ab identischem Zustand mit unabhängigen Cue- und
  Explorationsströmen,
- feste Abstände 8/12/16/20/24/32, ein unabhängiger unregelmäßiger Zeitplan und
  ein kontinuierlicher Ereignisstrom,
- Grenzproben bei `delay−1`, `delay` und `delay+1`,
- Retention, Umweltumkehr, Ressourcen, Persistenz und Determinismus.

Historische Rohdaten bleiben unverändert und werden referenziert statt
dupliziert. Diagnostische Kopien dürfen keine technischen Zielregeln in den
Lernzustand einführen. Zeit und Queue sind keine isolierbaren Faktoren.

## Erfolg und Entscheidung

Wissenschaftlicher Erfolg besteht in einer reproduzierbaren kausalen
Einordnung, nicht in einer höheren Bestleistung. Eine neue Funktion ist nur bei
einem lokal beobachtbaren, notwendigen und hinreichenden sowie unabhängig
bestätigten Eingriff zulässig. Der Abschluss muss Bericht, technische Analyse,
B.5.7-Vorlage und eine explizite Merge-Empfehlung enthalten.

Die vollständigen Resultate und Entscheidungen stehen in:

- [Technische Kausalanalyse](030-technische-analyse-nichtrobuste-lernverlaeufe.md)
- [Forschungsbericht](030-forschungsbericht-forschungsstufe-b56.md)
- [Entscheidungsvorlage B.5.7](030-entscheidungsvorlage-forschungsstufe-b57.md)
