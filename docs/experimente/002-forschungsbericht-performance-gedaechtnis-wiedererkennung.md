# EVE – Experiment 001
## Forschungsbericht 002: Performance, Gedächtnis und Wiedererkennung

**Stand:** 17. November 2026  
**Ausgangsbasis:** Branch `codex/experiment-001-neural-core`, Commit `f6c85e4`  
**Forschungs-Seed:** `20261117`, soweit nicht anders angegeben  
**Status:** abgeschlossene erste Messreihe

## Kurzfassung

Der bestehende C++17-Kern unterstützt die geforderten Größen bis zu einer
Million Neuronen und vier Millionen initialen Synapsen. Auf der Messmaschine
verarbeitet der statische Benchmark je nach Netzgröße durchschnittlich etwa
2,39 bis 2,99 Millionen synaptische Ereignisse pro Sekunde auf einem einzelnen
CPU-Thread. Das größte Netzwerk benötigt näherungsweise 136 MB für die
Kernstrukturen und rund 140 MB maximalen Resident Set Size.

Wiederholtes `AB` verändert Gewichte, neuronale Zustände und Verbindungsstruktur
dauerhaft und reproduzierbar. Nach acht Trainingsdurchgängen reagiert das
trainierte Netzwerk auf einen weiteren Eingang ungefähr 18,5-mal so stark wie
das untrainierte Kontrollnetzwerk.

Diese Verstärkung ist jedoch nicht `AB`-spezifisch. Die Zahl der Feuerereignisse
bei `AB` liegt nur 1,02 % über dem Mittel der Testfolgen `BA`, `AC`, `AA` und
`BB`. Die bisherigen Daten belegen daher Speicherung und eine breit erhöhte
Erregbarkeit, aber keine wiederverwendbare Wiedererkennung von `AB`.

Das Netz stabilisiert sich außerdem nicht. Beim verwendeten Seed kippen sowohl
die Variante mit Wachstum als auch die Variante nur mit Gewichtslernen während
des zwölften `AB`-Durchgangs in eine selbsttragende Ereigniskaskade. Die
Beständigkeitsprüfung nach neuen Erfahrungen kann deshalb nicht abgeschlossen
werden.

## Forschungsfrage

> Kann EVE mit ihren bisherigen neuronalen Mechanismen wiederholte Erfahrungen
> nicht nur intern speichern, sondern bei späteren Eingaben auch nachweisbar
> wiederverwenden?

**Antwort dieser Messreihe:** Interne Speicherung und veränderte spätere
Verarbeitung sind nachweisbar. Eine musterspezifische, wiederverwendbare
Wiedererkennung ist noch nicht nachgewiesen.

## Unverändertes neuronales Modell

Die neuronalen Regeln wurden nicht verändert. Unverändert blieben:

- Neuronenmodell, Schwellenwert und Membran-Reset,
- synaptische Verstärkung und Wachstumswahrscheinlichkeit,
- Aktivitätsphasen,
- sensorische Bitcodierung,
- FIFO-Ereignisverarbeitung,
- Persistenzformat und Zufallszahlengenerator.

Ergänzt wurden ausschließlich:

- ein optionaler, zustandsloser Beobachter für Feuerereignisse,
- ein statischer Performance-Benchmark,
- automatisierte Forschungsabläufe für A bis E,
- getrennte Zustands- und Aktivitätsfingerabdrücke,
- CSV-Ausgabe und externe Vergleichsmaße.

Der Beobachter ist im Performance-Benchmark deaktiviert. Dateiausgabe und
Aktivitätsauswertung liegen außerhalb der gemessenen neuronalen Verarbeitung.

## Messumgebung

- CPU: AMD Ryzen 7 5700X, 8 Kerne/16 Threads
- RAM: 67.325.906.944 Byte
- Betriebssystem: Bazzite 45, Linux 7.2.15, x86-64
- Compiler: GCC 16.2.1
- Build: C++17, `-O2 -Wall -Wextra -Wpedantic -Wshadow`
- Benchmark: ein Thread, kein Logging, drei Prozesse je Netzgröße

Die vollständige Umgebung steht in
`research/results/research-002-environment.csv`.

## Messmethodik

### Performance

Für jede Netzgröße wurde ein separater Prozess dreimal mit identischem Seed
gestartet. Initialisierung und Verarbeitung wurden getrennt gemessen.

Um ausschließlich den Ereigniskern zu messen, wurden die bereits vorhandenen
Konfigurationsschalter für Lernen und Wachstum deaktiviert. Pro Lauf wurden
250.000 technische Feuerereignisse in Batches von 4.096 eingespeist. Bei vier
Ausgängen je Neuron entstanden genau eine Million vollständig abgearbeitete
synaptische Ereignisse. Es gab kein Logging und keine Dateiausgabe im
Messintervall.

CPU-Auslastung ist Prozess-CPU-Zeit geteilt durch Wandzeit. Maximaler Speicher
wurde über `getrusage` erfasst; der aktuelle Resident Set Size stammt aus
`/proc/self/status`. Die internen Speicherwerte verwenden die tatsächlichen
Kapazitäten der zusammenhängenden Neuronen- und Synapsenfelder.

### Lern- und Wiedererkennungsversuche

Die Lernversuche verwenden die unveränderte Standardkonfiguration. Für jeden
Vergleich startet ein eigenes Netz mit identischem Seed. Jeder Test in
Experiment C lädt den gespeicherten Vorzustand erneut, sodass Tests einander
nicht beeinflussen.

Die externe Beobachtung erfasst:

- Zahl und Reihenfolge feuernder Neuronen,
- eindeutige feuernde Neuronen,
- häufigste Neuronen und Übergänge,
- Ereignis-, Neuronen-, Gewichts- und Strukturzustände,
- Änderungen während jedes Tests.

Als einfaches, reproduzierbares Spezifitätsmaß gilt:

`Reaktion auf bekanntes AB / mittlere Reaktion auf BA, AC, AA und BB`

Ein Wert nahe 1 bedeutet keine bevorzugte Reaktion auf `AB`. Zusätzlich wird
die Verstärkung gegenüber dem untrainierten Kontrollnetz gemessen.

## Technische Ergebnisse

### Performance und Skalierung

Alle verlangten Netzgrößen wurden erfolgreich initialisiert und verarbeitet.

| Neuronen | Synapsen | Init. s | Verarbeitung s | Synaptische Ereignisse/s | Peak RSS MiB |
|---:|---:|---:|---:|---:|---:|
| 256 | 1.024 | 0,000042 ± 0,000005 | 0,3360 ± 0,0025 | 2.976.068 ± 21.972 | 4,34 |
| 1.024 | 4.096 | 0,000155 ± 0,000031 | 0,3346 ± 0,0038 | 2.988.495 ± 33.908 | 4,32 |
| 10.000 | 40.000 | 0,001328 ± 0,000266 | 0,3399 ± 0,0031 | 2.942.625 ± 27.148 | 5,50 |
| 100.000 | 400.000 | 0,014214 ± 0,000492 | 0,3902 ± 0,0041 | 2.562.757 ± 26.662 | 17,15 |
| 1.000.000 | 4.000.000 | 0,404640 ± 0,006927 | 0,4176 ± 0,0057 | 2.394.919 ± 32.603 | 133,63 |

Zeitangaben sind Mittelwert ± Stichprobenstandardabweichung aus drei Läufen.
Die mittlere CPU-Auslastung während der Verarbeitung betrug 99,52 % eines
logischen Kerns.

### Speicherbedarf

Gemessene Strukturgrößen:

- Neuron: 40 Byte
- Synapse: 24 Byte
- Ereignis: 8 Byte

Bei vier initialen Synapsen je Neuron ergibt sich für die dynamischen
Kernfelder:

`40 + 4 × 24 = 136 Byte pro Neuron`

Der interne Schätzwert beträgt beim größten Netz 136.000.336 Byte. Der
RSS-Zuwachs konvergiert dort auf etwa 136,14 Byte pro Neuron. Kleine Netze
werden durch Prozess-, Bibliotheks-, Seiten- und Allocator-Overhead dominiert
und eignen sich nicht zur Ableitung eines Pro-Neuron-Werts.

### Beobachtung

Der Durchsatz fällt zwischen 256 und einer Million Neuronen um ungefähr 19,5 %.
Der Kern bleibt jedoch über den gesamten untersuchten Bereich funktionsfähig.
Der Benchmark ist single-threaded; die annähernd vollständige Auslastung eines
Kerns entspricht der Implementierung.

### Interpretation

Die zusammenhängenden Indexstrukturen skalieren speicherseitig nahezu linear.
Der Durchsatzverlust beim großen Netz ist mit größeren Arbeitsmengen und
Cache-Effekten vereinbar. Der Benchmark misst bewusst eine statische
Ereignislast und nicht die deutlich teureren, instabilen Lernkaskaden.

## Experiment A – Wiederholtes Lernen

### Messwerte

| Variante | Stufe | Abgeschlossen | Ereignisse gesamt | Feuerungen | Verstärkungen | Neue Synapsen |
|---|---:|---:|---:|---:|---:|---:|
| Lernen und Wachstum | 1 | 1 | 20 | 4 | 0 | 0 |
| Lernen und Wachstum | 8 | 8 | 798 | 145 | 134 | 24 |
| Lernen und Wachstum | 32 | 11 | 1.002.989 | 202.780 | 15.355 | 9.038 |
| Nur Lernen | 1 | 1 | 20 | 4 | 0 | 0 |
| Nur Lernen | 8 | 8 | 660 | 132 | 118 | 0 |
| Nur Lernen | 32 | 11 | 1.003.045 | 428.881 | 86.994 | 0 |
| Statische Kontrolle | 512 | 512 | 10.240 | 2.048 | 0 | 0 |

Beide lernenden Varianten scheiterten während der zwölften Präsentation am
Limit von einer Million verarbeiteten Ereignissen pro Byte-Kaskade. Zu diesem
Zeitpunkt warteten noch:

- 4.724.726 Ereignisse bei Lernen und Wachstum,
- 1.247.981 Ereignisse bei reinem Gewichtslernen.

Die statische Kontrolle blieb bis 512 Wiederholungen linear. Ihr neuronaler
Zustand und ihre 1.024 Gewichte blieben unverändert.

### Beobachtung

Die Aktivität stabilisiert sich nicht. Wachstum ist nicht die alleinige
Ursache: Auch bei fester Verbindungsstruktur werden positive Gewichte in einem
rekurrenten Netz so stark, dass selbsttragende Kaskaden entstehen. Wachstum
vergrößert zusätzlich die Struktur von 1.024 auf 10.062 Synapsen.

### Interpretation

Die aktuellen Regeln besitzen ausschließlich positive Verstärkung und keine
Abschwächung, Normalisierung, Erholungszeit oder globale Aktivitätsregulation.
Die gemessene Eskalation ist damit funktional erklärbar. Sie ist ein
experimentelles Ergebnis, keine im Bericht vorgenommene Modelländerung.

## Experiment B – Unterschiedliche Sequenzen

### Messwerte nach acht Präsentationen

| Folge | Ereignisse | Feuerungen | Verstärkungen | Neue Synapsen | Synapsen gesamt |
|---|---:|---:|---:|---:|---:|
| `AB` | 798 | 145 | 134 | 24 | 1.048 |
| `BA` | 803 | 146 | 137 | 25 | 1.049 |
| `AA` | 1.257 | 229 | 222 | 41 | 1.065 |
| `BB` | 1.268 | 229 | 219 | 48 | 1.072 |

Alle vier Läufe erzeugten unterschiedliche Neuronen-, Gewichts-, Struktur- und
Aktivitätsfingerabdrücke. `AB` und `BA` sind in den aggregierten Zählern sehr
ähnlich. `AA` und `BB` erzeugen deutlich mehr Aktivität und Wachstum.

### Interpretation

Die Regeln können Folgen in unterschiedliche interne Zustände überführen. Das
zeigt Zustandsabhängigkeit, aber noch keine Wiedererkennung. Insbesondere kann
die unterschiedliche Reihenfolge der FIFO-Ereignisse bereits unterschiedliche
Wachstumsentscheidungen und Phasengrenzen verursachen.

## Experiment C – Wiedererkennung

Das trainierte Netz erhielt achtmal `AB`. Das Kontrollnetz erhielt keine
Lernerfahrung. Vor jeder Testeingabe wurde der jeweilige Zustand neu geladen.

### Messwerte pro einzelner Testeingabe

| Netz | Test | Ereignisse | Feuerungen | Eindeutige Neuronen | Verstärkungen | Wachstum |
|---|---|---:|---:|---:|---:|---:|
| trainiert | `AB` | 408 | 74 | 60 | 66 | 12 |
| trainiert | `BA` | 404 | 73 | 60 | 66 | 12 |
| trainiert | `AC` | 424 | 77 | 61 | 67 | 15 |
| trainiert | `AA` | 392 | 71 | 57 | 62 | 12 |
| trainiert | `BB` | 401 | 72 | 55 | 61 | 9 |
| Kontrolle | `AB` | 20 | 4 | 3 | 0 | 0 |
| Kontrolle | `BA` | 20 | 4 | 3 | 0 | 0 |
| Kontrolle | `AC` | 25 | 5 | 3 | 0 | 0 |
| Kontrolle | `AA` | 20 | 4 | 2 | 0 | 0 |
| Kontrolle | `BB` | 20 | 4 | 2 | 0 | 0 |

### Reproduzierbares Vergleichsmaß

| Messgröße | `AB` trainiert | Andere trainiert, Mittel | Spezifität | `AB` Kontrolle | Verstärkung trainiert/Kontrolle |
|---|---:|---:|---:|---:|---:|
| Ereignisse | 408 | 405,25 | 1,0068 | 20 | 20,40 |
| Feuerungen | 74 | 73,25 | 1,0102 | 4 | 18,50 |
| Eindeutige Neuronen | 60 | 58,25 | 1,0300 | 3 | 20,00 |

### Beobachtung

Das Training verändert die spätere Reaktion massiv. Alle Testfolgen lösen im
trainierten Netz jedoch ähnlich starke Kaskaden aus. `AC` erzeugt sogar mehr
Ereignisse und Feuerungen als das trainierte Muster `AB`.

Alle Feuerfolgen besitzen unterschiedliche exakte Hashes. Das ist kein
Erkennungsnachweis, weil bereits verschiedene technische Eingangskanäle und
Eingabereihenfolgen unterschiedliche Hashes erzeugen.

### Interpretation

Nach acht Trainingsdurchgängen ist eine breit erhöhte Erregbarkeit
nachgewiesen. Die Spezifitätswerte nahe 1 liefern keinen belastbaren Hinweis
darauf, dass `AB` bevorzugt wiedererkannt oder funktional wiederverwendet wird.
Die Daten trennen damit interne Speicherung klar von Wiedererkennung.

Die Testeingaben selbst verändern das Netz erheblich. Diese Änderungen sind in
den Delta-Spalten der CSV-Datei separat erfasst; durch das erneute Laden des
Vorzustands beeinflussen sie keine anderen Tests.

## Experiment D – Beständigkeit

Unmittelbar nach acht `AB`-Trainingsdurchgängen war die `AB`-Reaktion identisch
mit Experiment C: 408 Ereignisse, 74 Feuerungen, 66 Verstärkungen und zwölf neue
Synapsen.

Die geplante Interferenz aus je acht Wiederholungen von `CD`, `EF` und `GH`
konnte nicht beendet werden. Bereits während der vierten `CD`-Präsentation
erreichte das Netz das Ereignislimit. Nach drei vollständig abgeschlossenen
Interferenzmustern standen noch 1.774.745 Ereignisse aus; die Struktur war zu
diesem Zeitpunkt auf 7.658 Synapsen angewachsen.

### Interpretation

Die Beständigkeit einer charakteristischen `AB`-Reaktion nach anderen
Erfahrungen ist in dieser Konfiguration nicht entscheidbar. Der Versuch scheitert
nicht an nachgewiesenem Vergessen, sondern an der zuvor beobachteten
Aktivitätsinstabilität.

## Experiment E – Reproduzierbarkeit

Zwei unabhängige Läufe mit Seed `20261117` waren in allen geprüften
Fingerabdrücken, Zuständen und Zählern bitgenau identisch. Beide erzeugten beim
Test 408 Ereignisse, 74 Feuerungen, 66 Verstärkungen und zwölf neue Synapsen.

Andere Seeds führten zu stark abweichendem Verhalten:

| Seed | Training abgeschlossen | Testereignisse | Testfeuerungen | Testverstärkungen | Testwachstum |
|---:|---:|---:|---:|---:|---:|
| 7 | 8 | 653 | 115 | 99 | 32 |
| 42 | 4 | nicht ausführbar | nicht ausführbar | – | – |
| 99 | 8 | 1.187 | 213 | 148 | 33 |

Seed 42 kippte bereits während der fünften Trainingspräsentation in die
Ereignisgrenze. Damit ist das System bei identischen Regeln stark von der
initialen Zufallsvernetzung abhängig.

## Gesamtbewertung

### Belastbar beobachtet

- Der Kern unterstützt alle geforderten Netzgrößen bis 1.000.000 Neuronen.
- Speicherverbrauch skaliert für vier Synapsen je Neuron nahezu linear mit
  etwa 136 Byte je Neuron.
- Gleiche Seeds und Eingaben liefern identische Ergebnisse.
- Wiederholte Erfahrung verändert Gewichte, Zustände und Struktur dauerhaft.
- Trainierte Netze reagieren später deutlich anders als untrainierte Kontrollen.
- Verschiedene Eingabefolgen erzeugen verschiedene interne Zustände.
- Lernende Netze werden nach wenigen Wiederholungen instabil.
- Die Stärke und der Zeitpunkt der Instabilität hängen stark vom Seed ab.

### Nicht nachgewiesen

- spezifische Wiedererkennung von `AB`,
- bevorzugter Abruf eines trainierten Aktivitätsmusters,
- Generalisierung oder funktionale Wiederverwendung,
- Stabilisierung bei fortgesetztem Lernen,
- Beständigkeit nach konkurrierenden Erfahrungen.

### Schlussfolgerung

Die bisherigen Regeln besitzen eine wirksame Speicherwirkung, aber noch keinen
nachgewiesenen Mechanismus für selektiven Abruf. Das beobachtete „Gedächtnis“
äußert sich derzeit primär als positive Verstärkung und allgemeine
Übererregbarkeit. Unterschiedliche Zustände und Reaktionshashes dürfen daher
nicht als Wiedererkennung interpretiert werden.

## Offene Forschungsfragen für Experiment 002

1. Welche lokale Regulation kann Dauerkaskaden verhindern, ohne erworbene
   Unterschiede zu löschen?
2. Reicht eine Gewichtsobergrenze, oder sind Abschwächung, Normalisierung und
   hemmende Wirkung erforderlich?
3. Wie kann eine Testreaktion nichtinvasiv gemessen werden, ohne dass der Test
   selbst sofort weiterlernt und wächst?
4. Welche Kontrollgruppe mit gleicher Eingabemenge trennt Musterspezifität von
   allgemeiner Aktivitätssteigerung?
5. Welche Aktivitätsdistanz korreliert mit späterem Verhalten statt nur mit
   unterschiedlichen Eingangskanälen?
6. Existiert bei anderen Seeds ein stabiles Lernfenster mit höherer
   Musterspezifität?
7. Wie verteilen sich Kippzeitpunkt, Durchsatz und Speicherwachstum über eine
   größere Seed-Stichprobe?
8. Bleiben gelernte Unterschiede bei deaktiviertem Wachstum, begrenztem Gewicht
   oder veränderter Reset-Variante abrufbar?
9. Welche Folgeaufgabe kann funktionale Wiederverwendung messen, ohne Semantik
   in den neuronalen Kern einzubauen?
10. Wie verändert sich die aktive Lernperformance bei größeren Netzen, bevor
    eine Kaskade entsteht?

## Reproduzierbarkeit und Daten

```sh
make all
make test
scripts/run_research_002_experiments.sh
scripts/run_research_002_benchmarks.sh
```

Rohdaten und Spaltenbeschreibung liegen unter `research/results` und in
`research/README.md`. Performance-Zeiten sind maschinenabhängig. Neuronale
Zustände und Ereignisfolgen sind bei gleichem Build, Seed und Eingabestrom
deterministisch.
