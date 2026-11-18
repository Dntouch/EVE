# EVE – Forschungsstufe B.3
## Neuronale Rückmeldungswege und Zuordnung von Lernerfahrungen

**Ausgangsbasis:** `codex/research-stage-b2-local-eligibility`  
**Status:** umgesetzt und ausgewertet  
**Implementierung:** C++17

## Forschungsfrage

Kann eine abgestufte technische Bewertung über normale neuronale Feuer- und
Synapsenereignisse Neuronen mit gültigen lokalen Eligibility-Markierungen
erreichen, ohne dass technische Kausalitätsdaten Ziele auswählen oder
Synapsengewichte verändern?

Getrennt untersucht werden Erreichbarkeit, zeitliche Verfügbarkeit,
Selektivität und die Zuordnung mehrerer Handlungen.

## Verbindliche Varianten

- F0: technische Bewertung ohne neuronales Ereignis.
- F1: eigener sensorischer Bewertungseingang.
- F2: vorab deterministisch erzeugte allgemeine Direktverschaltung.
- F3: vorab deterministisch erzeugte allgemeine Relaisverschaltung.
- Z0: nur aktuell gültige Eligibility.
- Z1: neuronale Abschnittsgrenze ohne technische Beteiligtenliste.
- Z2: Hypothese eines lokalen Handlungskontexts; keine versteckte technische
  Handlungs-ID.
- Bewertungen 0, 25, 50, 75 und 100 Prozent sowie ein einfaches Signal.
- Eligibility-Dauern 4, 16 und 64; R2F/16 als Standard.
- 2, 8, 32 und 128 fortgeführte Handlungen mit unmittelbaren, verzögerten,
  überlappenden, vertauschten, fehlenden und widersprüchlichen Rückmeldungen.

## Forschungsgrenzen

Die Bewertung enthält weder Zielbits noch richtige Handlungsfolgen. Routen
werden unabhängig von M3-Diagnose und letzter Handlung erzeugt. Empfang darf
nur Diagnosezähler verändern. Rückmeldungsabhängige Verstärkung,
Abschwächung, STDP und Synapsenschwund bleiben deaktiviert und B.4 vorbehalten.

