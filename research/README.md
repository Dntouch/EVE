# Forschungsdaten

Dieser Ordner enthält reproduzierbare, maschinenlesbare Ergebnisse der
EVE-Experimente. Generierte Binärzustände werden nur während eines Laufs als
temporäre Vergleichszustände verwendet und anschließend entfernt.

## Forschungsauftrag 002

Build und Kerntests:

```sh
make all
make test
```

Experimente A bis E erneut ausführen:

```sh
scripts/run_research_002_experiments.sh
```

Performance-Messreihe erneut ausführen:

```sh
scripts/run_research_002_benchmarks.sh
```

Die Skripte überschreiben die zugehörigen CSV-Dateien in `research/results`.
Performance-Zeiten und RSS-Werte sind maschinen- und lastabhängig. Zustände,
Zähler und Aktivitätshashes sind bei gleichem Build, Startwert und Eingabestrom
deterministisch.

### Ergebnisdateien

- `research-002-performance.csv`: alle 15 Performance-Einzelläufe.
- `research-002-performance-summary.csv`: Mittelwerte und
  Stichprobenstandardabweichungen.
- `research-002-experiment-a.csv`: kumuliertes `AB`-Training und Kontrollen.
- `research-002-experiment-b.csv`: Vergleich von `AB`, `BA`, `AA` und `BB`.
- `research-002-experiment-c.csv`: wiederhergestellte Erkennungstests.
- `research-002-recognition-summary.csv`: extern berechnete Spezifitätsmaße.
- `research-002-experiment-d.csv`: Beständigkeit und Interferenz.
- `research-002-experiment-e.csv`: Reproduzierbarkeit und Seed-Abhängigkeit.
- `research-002-environment.csv`: Messumgebung und Build-Konfiguration.

Fingerabdrücke verwenden FNV-1a als reproduzierbares Vergleichsmaß. Sie sind
keine kryptografischen Integritätsnachweise.

## Experiment 002 – neuronale Selbstregulation

Build, Tests und vollständige 512er-Matrix:

```sh
scripts/run_experiment_002.sh research/results 512
```

Ein einzelner Parameter kann für schnelle Reproduktionen ausgewählt werden:

```sh
scripts/run_experiment_002.sh /tmp/eve-experiment-002 512 C5
```

Die Parameter stehen in `experiment-002-parameters.csv`. Hauptläufe,
Wiedererkennung, einzelne Interferenzerfahrungen, Interferenztests, statische
Performance und Aggregation liegen als `experiment-002-*.csv` unter `results`.
Zeitwerte sind maschinen- und lastabhängig; Ereignisse, Zustände und
Aktivitätshashes sind bei identischem Build deterministisch.

## Experiment 003 – kombinierte Selbstregulation

Der vollständige Lauf einschließlich Kerntests, Langzeitmatrix,
Wiedererkennung, Interferenz, Performance und Aggregation lautet:

```sh
scripts/run_experiment_003.sh research/results
```

Für einen gezielten Diagnoselauf kann eine einzelne Variante ausgewählt werden:

```sh
scripts/run_experiment_003.sh /tmp/eve-experiment-003 C5+A2
```

Der vollständige Runner führt die unabhängigen Seed-/Eingabefälle parallel aus
und schreibt sie anschließend in fester Reihenfolge. Die Parameter stehen in
`experiment-003-parameters.csv`. Rohdaten und Zusammenfassung liegen als
`experiment-003-*.csv` unter `results`. Zeitwerte sind maschinen- und
lastabhängig; Ereignisse, Zustände und Aktivitätshashes bleiben bei identischem
Build deterministisch.

## Forschungsstufe A – neuronale Ausgabe

Build, alle Kerntests und die vollständigen Versuche A1 bis A5:

~~~sh
scripts/run_research_stage_a.sh research/results
~~~

Der Runner erzeugt:

- research-stage-a-assignments.csv: vollständige, reproduzierbare Zuordnung
  der Varianten M1, M2 und M3,
- research-stage-a-results.csv: Rohdaten der isolierten und getrennt
  gekennzeichneten Lernläufe.

Die Laufzeitspalte ist maschinenabhängig. Zuordnung, Zustände, Ereigniszähler
und neuronale Ausgaben sind bei identischem Build deterministisch.

## Forschungsstufe A.1 – sensomotorische Grundlage

Build, alle Kerntests und die vollständige Matrix aus vier Phasen,
Integrationsstufen und Abschaltkontrollen:

~~~sh
scripts/run_research_stage_a1.sh research/results
~~~

Der Runner verwendet alle sechs festgelegten Seeds und erzeugt:

- `research-stage-a1-parameters.csv`: 28 vollständig dokumentierte
  Konfigurationen,
- `research-stage-a1-paths.csv`: angeborene Pfade der Varianten V1 bis V3,
- `research-stage-a1-results.csv`: 168 Einzelmessungen.

Laufzeit und Durchsatz sind maschinenabhängig. Pfade, Aktivierungen, Antworten,
Ereigniszähler und Zustandsfingerabdrücke sind bei identischem Build
deterministisch. Vorhandene Forschungsdaten werden nicht überschrieben; der
A.1-Runner schreibt ausschließlich Dateien mit dem Präfix
`research-stage-a1-`.

## Forschungsstufe A.2 – motorische Abschlusskausalität

Build, alle Kerntests und die vollständigen Explorations-, Kausalitäts-,
Wiederholungs-, Rückkopplungs- und Performancematrizen:

~~~sh
scripts/run_research_stage_a2.sh research/results
~~~

Der Runner erzeugt ausschließlich Dateien mit dem Präfix
`research-stage-a2-`:

- `parameters.csv`: vollständige gemeinsame Versuchsparameter,
- `exploration.csv`: E0, E4-alt und E4-neu mit 8, 16 und 32 Aktivierungen,
- `causality.csv`: K0/K1/K2 über 32 Seeds,
- `diagnostics.csv`: geordnete reale Feuer- und Impulsfolge eines
  nachgewiesenen Abschlusses,
- `repetitions.csv`: wiederhergestellte und fortgeführte Antwortphasen,
- `feedback.csv`: nachgelagerter R0/R1-Vergleich,
- `performance.csv`: gepaarte Messungen mit und ohne Detaildiagnose.

Laufzeit und Durchsatz sind maschinenabhängig. Alle übrigen Felder sind bei
identischem Build reproduzierbar. Temporäre Zustandsdateien der
Wiederherstellungsprüfung werden nach dem Lauf entfernt.
