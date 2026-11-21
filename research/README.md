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

## Forschungsstufe B.5.0 – lokale Kreditzuordnung

Build und vollständige Referenz-, Ereignis- und Kausalmatrix:

```sh
scripts/run-research-stage-b50.sh research/results/b50
```

Der Runner reproduziert K0–K3 und die B.4.8-Explorationskontrollen über drei
Stromfamilien sowie normale und inverse Zuordnung. E1 und E2 starten aus
getrennten fingerprint-identischen Zustandskopien. Die prospektive Population
umfasst alle 49.152 Trainingsereignisse; je Seed-Cluster wird das erste
positive Explorationsereignis nach autonomer Stille kausal untersucht.

`eligibility-inventory.csv.gz`, `weight-changes.csv.gz` und
`event-chains.csv.gz` enthalten lokale Generationen, Kandidaten, Phasen,
Gewichte und Ereignisse. `causal-copies.csv`,
`single-weight-ablations.csv.gz`, `repeated-weight-ablations.csv`,
`timing-credit.csv` und `interference-ablations.csv` dokumentieren die
diagnostischen K4–K8-Eingriffe. Diese selektiven Eingriffe sind keine
Lernregeln.

`diagnostic-summary.csv` enthält clusterbewusste Bootstrapintervalle.
Bestätigung, Aufgabenmatrix, Persistenz und allgemeine Erfolgskriterien werden
separat ausgewiesen. K9/K10 wurden mangels eines fehlenden lokalen
Zuordnungsmechanismus nicht implementiert. Historische Ergebnisordner bleiben
unverändert.

## Forschungsstufe B.5.1 – natürliche Lernwiederholung und Stabilität

Build und vollständige Reproduktions-, Längsschnitt- und Kausalmatrix:

```sh
scripts/run-research-stage-b51.sh research/results/b51
```

Der Runner reproduziert B.5.0 in `b50-reproduction` bytegenau und führt danach
768 natürliche 512-Episoden-Läufe aus: 64 Entdeckungs- und 64 unbenutzte
Bestätigungs-Seeds, drei Stromfamilien sowie normale und inverse Zuordnung.
E1 und E2 werden nach 128, 256 und 512 Episoden aus getrennten identischen
Zustandskopien ausgewertet.

`natural-trajectories.csv.gz`, `eligibility-events.csv.gz` und
`natural-weight-changes.csv.gz` enthalten die vollständigen episodischen
Aktivitäts-, Eligibility- und Gewichtsrohdaten. `weight-transitions.csv.gz`,
`threshold-causal-copies.csv.gz`, `ability-events.csv.gz`,
`loss-causal-copies.csv` und `loss-continuations.csv` dokumentieren
Schwellenübergänge, Erwerb, Verlust, Wiedererwerb und kontrollgleiche
Ablationen.

`stability-checkpoints.csv`, `controlled-continuations.csv`,
`path-lifecycle.csv`, `checkpoint-evaluation.csv`,
`confusion-matrices.csv`, `cluster-statistics.csv` und
`success-criteria.csv` enthalten Retention, alternative Erfahrungsfolgen,
zensierte Lebenszyklusdaten, Verwechslungsmatrizen und clusterbewusste
Bootstrapintervalle. `validated-repetition-comparison.csv` weist nach, dass
die explizite Handlungsprüfung die historischen B.5.0-Werte nicht verändert.
`task-matrix.csv` erhält Einzelreiz, beide Sequenzrichtungen, Alternierung,
Mischung, inverse Zuordnung, Umweltumkehr und Retention.

K9/K10 bleiben deaktiviert. Historische Ergebnisordner werden nicht
überschrieben; Laufzeit und RSS in `performance.csv` sind maschinenabhängig.

## Forschungsstufe B.5.2 – lokale Gedächtnisstabilität

Build und vollständige Reproduktions-, Faktor- und Zustandsmatrix:

```sh
scripts/run-research-stage-b52.sh research/results/b52
```

Der Runner reproduziert B.5.1 getrennt in `b51-reproduction` und führt 768
Netze mit insgesamt 393.216 Episoden aus. Für alle Verluste ohne
Gewichtsänderung sowie den ersten und zweiten Verlust jedes Pfades werden
vollständige Zustandskopien und die acht S×I×T-Varianten ausgewertet.

`loss-events.csv.gz`, `factorial-ablations.csv.gz` und
`transient-state-differences.csv.gz` trennen synaptische Speicherung,
funktionalen Abruf und Faktorinteraktionen. `probe-controls.csv.gz` vergleicht
identische Kopien, sequenzielle Proben und ereignisfreie Zeitabstände.

`inhibitory-eligibility.csv.gz`, `inhibitory-followup.csv.gz` und
`sensor-candidate-overlap.csv.gz` enthalten Kandidaten, Einzelablationen,
isolierte Änderungen, natürliche Fortsetzungen und gemeinsame Empfänger.
`simultaneous-stability.csv` verfolgt ersten, späteren und länger stabilen
gemeinsamen Erfolg über 1 bis 64 weitere Erfahrungen.

`diagnostic-summary.csv`, `causal-summary.csv`, `cluster-statistics.csv`,
`success-criteria.csv`, `control-groups.csv`, `task-matrix.csv`,
`checkpoint-evaluation.csv` und `confusion-matrices.csv` enthalten die
zusammengefasste Kausal-, Leistungs- und Kontrollauswertung. 64/64 Zustände
werden in `persistence.csv` exakt wiederhergestellt. K9/K10 bleiben mangels
einer einzelnen lokal gerechtfertigten Produktionsintervention deaktiviert.

## Forschungsstufe B.5.3 – lokale Ursachen transienter Abrufstörungen

Build und vollständige Reproduktions-, Komponenten- und Probematrix:

```sh
scripts/run-research-stage-b53.sh research/results/b53
```

Der Runner reproduziert B.5.2 getrennt in `b52-reproduction` und wertet
dasselbe Korpus von 768 Netzen, 393.216 Episoden und 15.194 Detailverlusten
aus. Isolierte Vorwärts- und Rückwärtsinterventionen trennen Membran,
Eingangsaktivität, G3, Eligibility, Synapsenaktivität, Recovery, Kontext,
Zufallszustand und Diagnosemetriken. Zeit und vollständige Queue werden wegen
ihrer Kopplung an alle zeitgebundenen Zustände bewusst nicht isoliert.

`component-interventions.csv.gz`, `combination-interventions.csv.gz` und
`membrane-neuron-interventions.csv.gz` enthalten die Komponenten-,
Kombinations- und Einzelneuronenablationen. `weight-interactions.csv.gz`
verknüpft alte und neue Sensor-, Hemm- und Membranzustände.
`probe-sequences.csv.gz` dokumentiert X/Y-Folgen über Abstände von 0 bis 64.

`component-summary.csv`, `membrane-neuron-summary.csv`,
`minimal-membrane-summary.csv`, `weight-interaction-summary.csv`,
`probe-summary.csv`, `cluster-statistics.csv` und `diagnostic-summary.csv`
enthalten die zusammengefassten Kausal- und Bestätigungsergebnisse. 64/64
Zustände werden in `persistence.csv` exakt wiederhergestellt. K11/K12 bleiben
deaktiviert, weil der diagnostische Membranrollback keine natürliche lokale
Regel und keine gemeinsame Stabilisierung beider Beziehungen ist.

## Forschungsstufe B.5.4 – lokale Membran-Recovery

Build und vollständige Audit-, Recovery- und Lernmatrix:

```sh
scripts/run-research-stage-b54.sh research/results/b54
```

Der Runner reproduziert B.5.3 getrennt in `b53-reproduction`, auditiert den
tatsächlichen Reset- und Recoverypfad und führt anschließend 6.912 Netze mit
3.538.944 natürlichen Episoden aus. Entdeckung und unabhängige Bestätigung
verwenden je 64 Seeds, drei Stromfamilien sowie normale und inverse
Zuordnung. R0, No-Learning, kontrollierter Refraktärzustand, R1, R2-0 und
vorab festgelegte R2-Stärken und Delays werden gepaart verglichen.

`micro-recovery-events.csv`, `recovery-audit.csv`, `reset-audit.csv` und
`event-order-controls.csv` dokumentieren Aktivierung, Generationen,
Subthreshold-Grenze und gleiche Zeitstempel. `temporal-integration.csv.gz`
enthält AA, BB, AB, BA, ABA und BAB für Abstände 0 bis 64.

`natural-trajectories.csv.gz` und `recovery-events.csv.gz` enthalten die
vollständigen Episoden- und Recoveryrohdaten. `seed-results.csv`,
`variant-summary.csv`, `paired-comparisons.csv`, `cluster-statistics.csv`,
`success-criteria.csv`, `checkpoint-evaluation.csv` und
`confusion-matrices.csv` bilden Lernen, gemeinsame Verfügbarkeit,
Bestätigung, Umkehr und Retention ab. `causal-copies.csv.gz`,
`persistence.csv` und `resources.csv` dokumentieren Zustandskopien,
deterministische Wiederherstellung und Ereigniskosten.

Die großen CSV-Dateien sind mit `gzip -n` deterministisch komprimiert.
`performance.csv` und die Laufzeitspalten in `resources.csv` sind
maschinenabhängig; alle Zustands-, Lern- und Ereignisdaten sind bei gleichem
Build und Seed deterministisch. K12/K13 bleiben deaktiviert.

## Forschungsstufe B.5.8 – Lernen, Nichtlernen und kausale Stabilität

Build, Kerntests und vollständige Reproduktions-, Stabilitäts-, Umkehr- und
Kausalmatrix:

```sh
scripts/run-research-stage-b58.sh research/results/b58
```

Der Runner reproduziert die B.5.7-Referenzen exakt und wertet 4.224 zentrale
Netze aus. Entdeckung und unabhängige Bestätigung verwenden je 64 getrennte
Seeds, drei Stromfamilien sowie normale und inverse Zuordnung. Festabstände 1,
12, 16 und 32, ein unregelmäßiger Zeitplan, R2, S5 und No-Learning bilden die
Zeit- und Mechanismuskontrollen.

`stability-checkpoints.csv` vergleicht fortgesetzte aktive Plastizität (P0)
mit identischen Kopien, in denen nur Gewichtsschreibvorgänge unterdrückt
werden (P1). `reversal-checkpoints.csv` enthält aktive Umkehr (P2), weiterhin
unterdrückte Umkehr (P3) und reaktivierte Plastizität (P4).
`feedback-counterfactuals.csv` trennt natürliche, fehlende, neutrale,
selektive, verzögerte und schreibunterdrückte Rückmeldung. Die
Motorinterventionen stehen in `no-action-causality.csv`; die zwölf offenen
B.5.6-Fälle in `historical-open-cases.csv`.

`network-classifications.csv.gz`, `stable-experiences.csv.gz` und
`plasticity-events.csv.gz` enthalten die großen deterministischen Rohdaten.
`local-information-inventory.csv` und `local-indistinguishability.csv`
dokumentieren, welche lokale Information bei gemischten globalen Folgen
tatsächlich verfügbar war. Clusterbewusste Intervalle, Erfolgskriterien,
Persistenz und Ressourcen stehen in den jeweiligen Zusammenfassungen.

Der experimentelle Schreib-Gate konsumiert Eligibility und erhält die
Modulationsroute, verhindert aber die Gewichtsänderung. Er ist standardmäßig
deaktiviert und keine Produktionsregel. Das Persistenzformat 15 speichert den
Gate, seine Metrik und tatsächlich ausstehende Modulationsereignisse.
Laufzeit und RSS in `performance.csv` sind maschinenabhängig; alle fachlichen
Ergebnisse sind bei identischem Build, Seed und Eingabestrom deterministisch.

## Forschungsstufe B.5.9 – lokale Entscheidungsmarge und Plastizitätskausalität

Build, Kerntests und vollständige Reproduktions-, Integrations- und
Kausalmatrix:

```sh
scripts/run-research-stage-b59.sh research/results/b59
```

Der Runner reproduziert B.5.7 und B.5.8 exakt und wertet 4.224 zentrale Netze
aus. Discovery und unabhängige Bestätigung verwenden getrennte 64er-Seeds,
drei Stromfamilien und beide Zuordnungen. Die B.5.8-Kopien P0 bis P4 sowie
Fest-1, Fest-12, Fest-16, Fest-32, unregelmäßige Abstände, R2, S5 und
No-Learning bleiben als Kontrollen erhalten.

`integration-events.csv.gz` enthält die lokale Membran vor und am Entscheid,
erregende, hemmende und Netto-Beiträge, Schwelle, Marge, Refraktärzustand und
Impulsbündel. `nonfire-decisions.csv.gz` klassifiziert alle
Nichtfeuerentscheidungen. `eligibility-integration.csv.gz` verbindet die
Feuerentscheidung mit Kandidat, Modulation und späterem Ergebnis.

`information-level-summary.csv`, `local-indistinguishability.csv`,
`prediction-summary.csv` und `paired-statistics.csv` vergleichen L0 bis L4
mit seed-geclusterten Intervallen. `causal-input-ablations.csv` enthält 288
erregende, hemmende und kombinierte Zustandskopien.
`historical-positive-ablations.csv` und `historical-case-summary.csv` verfolgen
die zwölf B.5.6-Fälle ab dem ersten gemeinsamen Erwerb und prüfen einzelne
sowie kombinierte Rücknahmen positiver Änderungen.

Die großen CSV-Dateien werden mit `gzip -n` deterministisch komprimiert.
`resources.csv`, `persistence.csv`, `success-criteria.csv` und
`mechanism-decision.csv` dokumentieren Aufwand, exakte Fortsetzung,
Erfolgskriterien und die Nichtfreigabe einer neuen Lernregel. Laufzeit und RSS
sind maschinenabhängig; fachliche Ereignisse und Zustände sind bei identischem
Build, Seed und Eingabestrom deterministisch.

## Forschungsstufe B.5.10 – prospektive Einzel-Schreibkausalität

Build, 726 Kerntests und vollständige Referenz-, Zeit-, Kausal- und
Bestätigungsmatrix:

```sh
scripts/run-research-stage-b510.sh research/results/b510
```

Der Runner reproduziert B.5.7 und B.5.8/B.5.9 exakt. Die neuen Populationen
verwenden getrennte Seedbereiche 70001–70064 und 80001–80064 mit drei
Stromfamilien und beiden Zuordnungen. Fest-1, Fest-12, Fest-16, Fest-32,
unregelmäßige Abstände und No-Learning bilden die Zeitkontrollen. Die
B.5.8-Stabilitäts- und Umkehrkopien bleiben erhalten.

`prospective-write-events.csv.gz` enthält den tatsächlichen Schreibzeitpunkt,
Vorher- und Zielgewicht, Delta, Mechanismus, Eligibility und aktuelle lokale
Integration. `write-interventions.csv` dokumentiert W0 bis W4 einschließlich
Fingerabdruckgleichheit und Zahl ausgelassener Operationen.
`natural-followups.csv.gz` verfolgt W0 und W1 nach 1, 4, 8, 16, 32, 64 und
128 natürlichen Erfahrungen mit eigener Motorantwort und tatsächlicher
Rückmeldung.

`historical-write-events.csv` und `historical-event-summary.csv` zerlegen die
zehn bekannten Problemverbindungen in konkrete Schritte und kombinierte
Interventionen. `write-history.csv`, `information-availability.csv`,
`information-model-signatures.csv` und `information-model-predictions.csv`
trennen direkt lokale A/B-Information von extern rekonstruierter C-Historie
und zukünftiger D-Wirkung.

Große Tabellen werden mit `gzip -n` deterministisch komprimiert.
`reference-reproduction.csv`, `persistence.csv`, `resources.csv`,
`success-criteria.csv` und `mechanism-decision.csv` dokumentieren Reproduktion,
Fortsetzung, Aufwand und die Nichtfreigabe einer neuen Lernregel. Laufzeit und
RSS sind maschinenabhängig; Ereignisse, Zustände und Fingerabdrücke sind bei
identischem Build, Seed und Eingabestrom deterministisch.

## Forschungsstufe B.5.11 – Repräsentation und Lerninterferenz

Build, 756 Kerntests, historische Reproduktion und vollständige
Mehrfachlernmatrix:

```sh
scripts/run-research-stage-b511.sh research/results/b511
```

Discovery (90001–90064) und Bestätigung (100001–100064) verwenden je drei
Stromfamilien und beide Zuordnungen. `task-definition.csv` präregistriert
A=P→Q, B=P→R, C=Q→R sowie unabhängige S/T- und U/V-Kontrollen.

`network-results.csv.gz` enthält getrennte bedingte A-/B-/C-Abrufraten,
Erwerb, Wiedererwerb, Schreibungen und Ressourcen. `response-matrix.csv.gz`
protokolliert jede Aufgabe in sechs Kontexten einschließlich fehlender,
doppelter, anderer und früher Antworten. `v1-v10-dimensions.csv.gz` hält die
zehn Dimensionen getrennt; es gibt keinen globalen Lernscore.

`component-overlap.csv.gz` beschreibt aktive Neuronen und Synapsen.
`causal-ablations.csv` trennt einzelne, kombinierte und allgemeine
Ausgabeengpässe. `productive-reorganization.csv` enthält W0/W1/W2-Verläufe
für konkrete B-Schreibungen mit unmittelbarer A-Änderung.
`transfer-c.csv.gz` vergleicht C ohne Vortraining, nach A, B, A+B und nach
unabhängigen Kontrollaufgaben.

`historical-reproduction.csv` und die zugehörigen Rohdateien rechnen B.5.7
bis B.5.10 frisch nach. `aggregate-statistics.csv` enthält seedbasierte
Bootstrap-Intervalle, `paired-statistics.csv` den gepaarten Vergleich
geteilter und unabhängiger Pfade. `persistence.csv` prüft einen tatsächlich
ausstehenden S4-Termin. S4, strukturelle Plastizität und neue Lernregeln
bleiben deaktiviert.
