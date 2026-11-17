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
