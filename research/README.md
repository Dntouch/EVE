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

## Forschungsstufe B – lokales neuronales Ereignisgedächtnis

Build, alle Kerntests und die vollständigen Gedächtnis-, Kausalitäts-,
Haltbarkeits-, Unterbrechungs-, Wiederholungs- und Performancematrizen:

~~~sh
scripts/run_research_stage_b.sh research/results
~~~

Der Runner verwendet die 32 A.2-Seeds und 32 zusätzliche Seeds. Er erzeugt
ausschließlich Dateien mit dem Präfix `research-stage-b-`:

- `parameters.csv`: Architektur- und Versuchsparameter,
- `primary.csv`: G0 bis G4 mit aktiver und unterbrochener M3-Schaltung,
- `fire-snapshots.csv`: lokale Momentaufnahmen am Feuerentscheid,
- `retention.csv`: Zustand nach Abschluss, weiteren Eingängen und
  Eigenrückmeldung,
- `interruptions.csv`: K4-Vergleiche aus identischem Ausgangszustand,
- `repetitions.csv`: wiederhergestellte und fortgeführte Zustände,
- `performance.csv`: Speicher- und Diagnoseaufwand.

Neuronale Zustände, Antworten, Ereigniszähler und Fingerabdrücke sind bei
identischem Build reproduzierbar. Laufzeiten und Durchsatz sind
maschinenabhängig. Der lokale Ringspeicher ist Teil des persistierten
neuronalen Zustands; die technische Impulsdiagnose bleibt temporär und wird
nicht persistiert.

## Forschungsstufe B.1 – eigenständige neuronale Erholung

Build, alle Kerntests und die vollständigen Zeit-, Erholungs-, Wiederholungs-,
Integritäts-, Rückmeldungs- und Performancematrizen:

~~~sh
scripts/run_research_stage_b1.sh research/results
~~~

Der Runner verwendet alle 64 B-Seeds und erzeugt ausschließlich Dateien mit
dem Präfix `research-stage-b1-`:

- `parameters.csv`: gemeinsame Architektur- und Versuchsparameter,
- `timing.csv`: R0, R1, R2H und R2F mit T1 bis T4 und fünf Abständen,
- `repetitions.csv`: K0 bis K3, fünf Eingabefolgen sowie 2, 8, 32 und 128
  Antwortphasen,
- `neuron-diagnostics.csv`: relevante Impuls- und Erholungszustände,
- `integrity.csv`: Synapsen-, Gedächtnis- und Persistenzkontrollen,
- `feedback.csv`: F0 bis F2 über acht fortgeführte Phasen,
- `performance.csv`: tatsächliche Läufe mit 256, 1.024 und 10.000 Neuronen
  sowie Speicherextrapolationen.

Zustände, Ereignisfolgen und simulierte Zeit sind deterministisch. Laufzeit
und Durchsatz bleiben maschinenabhängig. Ein Antwortende verarbeitet keine
zukünftigen Erholungen; Zeit zwischen Handlungen wird ausschließlich durch den
Runner explizit fortgeschrieben.

## Forschungsstufe B.4 – lokale Drei-Faktor-Plastizität

Build, 215 Kerntests und die gepaarte B.4-Matrix mit 64 Seeds:

```sh
scripts/run-research-stage-b4.sh research/results/b4
```

Der Runner überschreibt nur die drei B.4-Dateien:

- `raw_trials.csv`: jede reale Trainings- und Auswertungsepisode,
- `summary.csv`: K0–K15, E1–E3 und Belohnungsumkehr je 64 Seeds,
- `resources.csv`: eine Messung bei 64 Neuronen und klar markierte lineare
  Projektionen.

Die Auswertungsphase sendet keine Modulation; damit werden dauerhafte
Gewichtsstände und nicht fortgesetztes Lernen gemessen. Alle Verhaltenswerte
werden aus tatsächlichen neuronalen Ausgaben A, B, beide oder keine berechnet.

## Forschungsstufe B.4.1 – methodische Validierung

Build, 260 Tests und die vollständige Validierungsmatrix:

```sh
scripts/run-research-stage-b41.sh research/results/b41
```

Die B.4-Dateien werden nicht verändert. B.4.1 erzeugt eigene Dateien für:

- historischen Audit und bytegenaue Reproduktion,
- unabhängige Aufgaben-, Explorations- und Rückmeldungspläne,
- Episodenzeit und vollständige Rohversuche,
- einzelne synaptische Änderungen und finale Motorpfade,
- gepaarte Bootstrapvergleiche,
- 512/2.048-Episoden-Läufe und Umweltumkehr,
- 64-/256-Neuronen-Erreichbarkeit und Ressourcen,
- kontrollierte Gewichtskausalität mit diagnostischer Ablation.

Maschinenabhängige Laufzeiten sind nicht Teil des deterministischen
Vergleichs. Aufgaben, Gewichte, Ereigniszähler und Ergebniszeilen sind bei
identischem Build und Seed reproduzierbar.

## Forschungsstufe B.4.2 – geschützte Exploration

Build und vollständige Versuchsmatrix:

```sh
scripts/run-research-stage-b42.sh research/results/b42
```

Die Stufe führt einen persistierbaren, allgemeinen Plastizitätsschutz für
angeborene Explorationssynapsen ein. Sie führt K0–K24 tatsächlich aus und
vergleicht zusätzlich A0–A2, N0–N5, X0–X4, beide Einzelreiz-Zielrichtungen,
beide Umweltumkehrungen und beide Zwei-Reiz-Zuordnungen.

Die Ergebnisdateien enthalten:

- vollständige Aufgabenpläne und Rohversuche,
- jede beobachtete Gewichtsänderung einschließlich Schutzstatus,
- Erreichbarkeit bei 0, 8, 16, 32, 64, 128 und 512 Erfahrungen,
- diagnostische Wiederherstellung verlorener Explorationswege,
- gepaarte Bootstrapvergleiche über 64 Seeds,
- Langläufe mit 32, 128, 512 und 2.048 Erfahrungen,
- 64-/256-Neuronen-Vergleiche sowie Ressourcen- und Laufzeitdaten.

B.4- und B.4.1-Daten werden nicht verändert.

## Forschungsstufe B.4.3 – lokale Trennung konkurrierender Erfahrungen

Build und vollständige 64-Seed-Matrix:

```sh
scripts/run-research-stage-b43.sh research/results/b43
```

Der Runner lässt die B.4.2-Daten unverändert und erzeugt getrennt:

- Aufgaben- und Parameterpläne mit separaten Zufallsströmen,
- vollständige Rohversuche und synaptische Änderungen,
- L0–L4-, Kapazitäts-, Dauer-, Reihenfolge-, Repräsentations- und
  Modulationsvergleiche,
- X/Y-Verwechslungsmatrizen, Einzelseedwerte und gepaarte
  Bootstrapintervalle,
- Fremdreiz-Anteile in lokalen Eligibility-Kandidaten,
- diagnostische Kausalablationen und Vergessensmaße,
- Persistenz-, Zeit-, Stabilitäts-, Ressourcen- und 64/256-Vergleiche.

Die Ablation verändert ausschließlich Zustandskopien. Sie ist kein
Lernmechanismus. Da keine Variante beide Beziehungen erfolgreich beherrscht,
wird der bedingte Kapazitätsversuch mit 4–16 Reizen nicht ausgeführt.

## Forschungsstufe B.4.4 – strukturelles Lernen

Build und vollständige S0–S4-Matrix mit 64 gepaarten Seeds und unabhängigen
Bestätigungs-Seeds:

```sh
scripts/run-research-stage-b44.sh research/results/b44
```

Der Runner erzeugt getrennte Parameter-, Trial-, Gewichts- und
Wachstumsprotokolle, Seed- und Gruppenzusammenfassungen, gepaarte
Bootstrapvergleiche, die retrospektive Analyse erfolgreicher B.4.3-Seeds,
Bestätigungsläufe, Kausaldiagnosen, Persistenz-, Stabilitäts-, Ressourcen- und
Performancewerte. S0–S4 sowie plastizitätsfreie Kontrollen werden für Einzel-,
Sequenz-, Alternierungs- und Mischaufgaben tatsächlich ausgeführt.

Wachstum wird vor der Auswertung abgeschaltet. Neue Verbindungen werden strikt
von nachgewiesenen funktionalen Gewichtsbeiträgen unterschieden.

## Forschungsstufe B.4.5 – lokale Entscheidungsbindung

Build und vollständige K0–K4-Matrix:

```sh
scripts/run-research-stage-b45.sh research/results/b45
```

Der Runner führt 143 Varianten mit je 64 gepaarten Seeds aus und bestätigt die
zentralen Ambiguitätsvarianten mit Seeds 1001–1064. Er erzeugt vollständige
Trial-, Feuer-, Kandidaten-, Modulations- und Gewichtsprotokolle sowie
Verwechslungsmatrizen, Seedwerte, gepaarte Bootstrapvergleiche,
Kausalvergleiche, Hypothesen- und Erfolgskriterien, Regressionen, Persistenz,
Stabilität, Ressourcen und Performance.

Wachstum ist in allen B.4.5-Varianten deaktiviert. Laufzeit- und RSS-Werte sind
maschinenabhängig; alle übrigen Ergebnisdateien sind bei identischem Build
und Seed deterministisch.

## Forschungsstufe B.4.6 – lokale neuronale Hemmung

Build und vollständige K0–K7-Matrix:

```sh
scripts/run-research-stage-b46.sh research/results/b46
```

Der Runner führt 100 Varianten mit je 64 gepaarten Seeds aus. Der beste
lernbare Kandidat, eine feste Kontrolle und die No-Learning-Referenz werden
mit Seeds 1001–1064 bestätigt. Enthalten sind isolierte Hemmungs- und
Reihenfolgeversuche, alle acht Lernaufgaben, vertauschte Zuordnungen,
Seedwerte, Bootstrapvergleiche, Erfolgskriterien, Fähigkeitskontrollen,
Abschaltvergleiche, 64 Persistenzproben, Stabilität, Ressourcen und
Performance.

Die großen unveränderlichen Detaildateien `trials.csv.gz`,
`inhibitory-events.csv.gz` und `weight-changes.csv.gz` werden deterministisch
mit Gzip komprimiert. Kleine Zusammenfassungen bleiben als direkt lesbare CSV
erhalten. Zum Lesen ohne Entpackdatei genügt beispielsweise:

```sh
gzip -cd research/results/b46/trials.csv.gz | less
```

Wachstum ist deaktiviert. Laufzeit, Durchsatz und RSS sind maschinenabhängig;
alle Ergebnisinhalte und die komprimierten Bytes sind bei identischem Build
und Seed reproduzierbar.

## Forschungsstufe B.4.7 – neuronale Symmetrie

Build und vollständige Diagnosematrix:

```sh
scripts/run-research-stage-b47.sh research/results/b47
```

Der Runner reproduziert sechs B.4.6-Referenzen, führt I0–I6 in einer
gemeinsamen Umgebung aus und prüft 17 reine Rollenabbildungen, normale und
inverse Zielbeziehungen, neun Lernaufgaben, E1/E2, längeres Umkehrtraining und
kausale Zustandskopien. Eine einzelne Phase-G-Gruppe entkoppelt die
nachgewiesene historische RNG-Kopplung und vergleicht lernbare, feste und
deaktivierte Plastizität für beide Zielzuordnungen. Zentrale Gruppen werden
mit Seeds 1001–1064 bestätigt.

Maschinenlesbar enthalten sind Architektur- und Umgebungsvergleich,
Vorlernzustand, Zufallsstromanalyse, Verwechslungsmatrizen,
Symmetrieäquivalenz, Explorations- und Modulationsbilanz,
Inhibitionskontrollen, Lernverläufe, Kausalablationen, Umkehranalyse,
Persistenz, Stabilität, Ressourcen und Erfolgskriterien.

Die Detaildateien `learning-trajectories.csv.gz`, `event-traces.csv.gz`,
`weight-changes.csv.gz` und `evaluation-trials.csv.gz` sind deterministisch
komprimiert. Beispiel:

```sh
gzip -cd research/results/b47/event-traces.csv.gz | less
```

Wachstum und globale Handlungswahl sind deaktiviert. Laufzeit, Durchsatz und
RSS sind maschinenabhängig; alle übrigen Inhalte und komprimierten Bytes sind
bei identischem Build und Seed reproduzierbar.

## Forschungsstufe B.4.8 – frühe Erfahrungsbindung

Build und vollständige Ursachen-, Checkpoint- und Explorationsmatrix:

```sh
scripts/run-research-stage-b48.sh research/results/b48
```

Der Runner reproduziert die entkoppelten B.4.7-Referenzen, prüft drei
unabhängige Zufallsstromfamilien, erzeugt Zustandskopien nach 0, 4, 8, 16, 32,
64, 96 und 128 Erfahrungen und führt alternative Fortsetzungen sowie
Gewichts-, Hemmungs- und Einfrierinterventionen aus. Referenz, feste Hemmung,
No-Learning, allgemeine Zufallsexploration, adaptive Exploration und eine
zeitlich umverteilte Mengenkontrolle werden für normale und inverse Aufgaben
mit 64 Entdeckungs- und 64 Bestätigungs-Seeds ausgewertet.

Die Detaildateien `training-episodes.csv.gz`, `negative-events.csv.gz` und
`evaluation-trials.csv.gz` sind deterministisch komprimiert. Lesebeispiel:

```sh
gzip -cd research/results/b48/negative-events.csv.gz | less
```

Die kleinen CSV-Dateien enthalten Stromtests, E1/E2-Zusammenfassungen,
vollständige Seedwerte und Verwechslungsmatrizen, Bootstrapvergleiche,
Lernkurven, Kausalinterventionen, Aufgabenmatrix, Persistenz,
Erfolgskriterien und Ressourcen. Historische Ergebnisordner werden nicht
verändert.

## Forschungsstufe B.4.9 – lokale Fehlerkorrektur

Build und vollständige Referenz-, Kontroll-, Fehler- und Kausalmatrix:

```sh
scripts/run-research-stage-b49.sh research/results/b49
```

Der Runner reproduziert K0–K6 über drei unabhängige Stromfamilien und normale
wie inverse Zielzuordnung. E1 und E2 starten aus getrennten Kopien desselben
vollständig trainierten Zustands; `b48-reference-reproduction.csv` weist die
exakte Reproduktion der historischen E2-Verwechslungsmatrizen und Lerngewichte
für 384 Referenzläufe aus. Die korrigierte E1-Auswertung ist frei von
temporaler Beeinflussung durch E2-Exploration.

`single-error-diagnostics.csv`, `competition-ablations.csv`,
`timing-diagnostics.csv`, `repeated-negative.csv` und
`alternative-binding.csv` enthalten die Diagnose realer autonomer
Fehlentscheidungen, lokale Gewichtsänderungen, Zustandskopie-Eingriffe und die
Bindung positiv bewerteter Alternativen. `event-chains.csv.gz` protokolliert
ausgewählte vollständige neuronale Ereignisketten. Die große
`evaluation-trials.csv.gz` ist deterministisch komprimiert.

Die übrigen Dateien enthalten vollständige Seedwerte, Verwechslungsmatrizen,
Bootstrapvergleiche, unabhängige Bestätigung, Aufgabenmatrix, Persistenz,
Ressourcen und vorab definierte Erfolgskriterien. K7/K8 wurden nach der
Kausaldiagnose bewusst nicht implementiert; es existiert kein neuer
persistenter Zustand. Historische Ergebnisordner bleiben unverändert.
