# EVE

EVE ist ein Forschungsprojekt zur Entwicklung einer eigenständig lernenden,
biologisch inspirierten digitalen Intelligenz. Der neuronale Kern wird in C++
entwickelt und kommt ohne vortrainiertes Large Language Model sowie ohne
vorgegebene Semantik aus.

> Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen,
> das eigenständig Intelligenz entwickeln kann.

## Projektstatus

**Stand:** 18. November 2026

**Jüngste abgeschlossene Forschungsstufe:** B.3.1 – neuronaler Handlungskontext

**Aktueller Forschungsschwerpunkt:** zuverlässigere neuronale Handlungszuordnung vor B.4

EVE ist inzwischen ein ausführbarer Forschungsprototyp. Er besitzt eine
deterministische, ereignisgesteuerte Laufzeit, ein eigenes neuronales Netz,
angeborene Stammhirnfunktionen, persistierbare Zustände und reproduzierbare
Versuchsreihen.

Die Forschungsstufe B.3.1 zeigt, dass EVE lokale neuronale Handlungskontexte
bilden, verteilen und mit Eligibility verbinden kann. Die Zuordnung mehrerer
oder identischer Handlungen ist jedoch noch nicht zuverlässig: In der
Zwei-Handlungs-Matrix waren nur 96 von 2.688 Rückmeldungen eindeutig richtig,
18 eindeutig falsch, 1.447 mehrdeutig und 1.127 nicht zugeordnet. Deshalb ist
B.4 mit rückmeldungsabhängiger Gewichtsänderung ausdrücklich nicht freigegeben.

Die Forschungsstufen bleiben als eigenständige Branches erhalten. Der jüngste
vollständige Implementierungsstand liegt auf
`codex/research-stage-b31-neural-action-context`; `main` bildet den stabilen
Einstieg und das übergeordnete Konzept ab.

EVE ist damit weder eine fertige allgemeine Intelligenz noch ein Nachweis, dass
eigenständige Intelligenz bereits entstanden ist. Der Prototyp dient dazu,
einzelne Mechanismen kontrolliert zu implementieren, zu messen und auch
negative Ergebnisse nachvollziehbar festzuhalten.

## Was bereits implementiert ist

- deterministische Ereignisverarbeitung mit simulierter Zeit,
- Neuronen, gewichtete Synapsen, Feuerereignisse und Erholungsmechanismen,
- konfigurierbare synaptische Verstärkung, Aktivitätsregulation und Wachstum,
- UTF-8-Sensoreingabe sowie angeborene sensorische und motorische Pfade,
- neuronale Ausgaben, motorische Abschlüsse und sensorische Rückkopplung,
- lokales neuronales Ereignisgedächtnis,
- lokale, zeitlich begrenzte Lernberechtigungen (Eligibility),
- abgestufte Bewertungsreize und neuronale Rückmeldungsrouten,
- versionierte Speicherung und Wiederherstellung vollständiger Zustände,
- Diagnostik, Ereignisprotokolle, Zustandsfingerabdrücke und Messdaten,
- automatisierte Kerntests und reproduzierbare Forschungsrunner.

Die einzelnen Versuchsreihen aktivieren bewusst nur ausgewählte Mechanismen.
Insbesondere bleiben Lernen und strukturelles Wachstum in vielen Kontrollen
deaktiviert, damit beobachtete Effekte eindeutig zugeordnet werden können.

## Forschungsweg

| Stufe | Gegenstand | Stand |
|---|---|---|
| Experimente 001–003 | neuronaler Kern und Selbstregulation | abgeschlossen |
| A bis A.2 | neuronale Ausgabe, sensomotorische Grundlage und Handlungskausalität | abgeschlossen |
| B | lokales neuronales Ereignisgedächtnis | abgeschlossen |
| B.1 | ereignisgesteuerte neuronale Erholung | abgeschlossen |
| B.2 | lokale neuronale Lernberechtigung | abgeschlossen |
| B.3 | neuronale Bewertungs- und Rückmeldungswege | abgeschlossen |
| B.3.1 | neuronaler Kontext zur Handlungszuordnung | abgeschlossen |
| B.4 | erste rückmeldungsabhängige Plastizität | noch nicht freigegeben |

Ausführliche Aufträge, Methoden, Ergebnisse und Schlussfolgerungen stehen unter
[`docs/experimente`](docs/experimente/). Die zugehörigen maschinenlesbaren
Messdaten liegen unter [`research/results`](research/results/).

## Schnellstart

Benötigt werden ein C++17-fähiger Compiler und entweder GNU Make oder CMake ab
Version 3.16.

Nach dem Klonen wird für den jüngsten ausführbaren Forschungsstand der
eigenständige B.3.1-Branch ausgecheckt:

```sh
git switch codex/research-stage-b31-neural-action-context
```

### Mit Make

```sh
make all
make test
./_build/eve_experiment --input AB --repeat 10 --seed 1
```

Alle Optionen des Basisexperiments zeigt:

```sh
./_build/eve_experiment --help
```

Ein Zustand kann gespeichert und in einem späteren Lauf fortgesetzt werden:

```sh
./_build/eve_experiment --input AB --repeat 10 --seed 1 --save /tmp/eve.state
./_build/eve_experiment --load /tmp/eve.state --input BA
```

### Mit CMake

```sh
cmake -S . -B _build/cmake -DCMAKE_BUILD_TYPE=Release
cmake --build _build/cmake
ctest --test-dir _build/cmake --output-on-failure
```

## Forschung reproduzieren

Die jüngste abgeschlossene Versuchsmatrix wird mit folgendem Runner neu
erzeugt:

```sh
./scripts/run-research-stage-b31.sh research/results
```

Weitere Runner und die Bedeutung aller Ergebnisdateien sind in der
[`research/README.md`](research/README.md) dokumentiert. Laufzeiten und
Speicherwerte hängen von Rechner und Systemlast ab; Ereignisfolgen, neuronale
Zustände und Fingerabdrücke sind bei identischem Build, Startwert und
Eingabestrom deterministisch.

## Repository-Struktur

```text
.
├── include/eve/               öffentliche C++-Schnittstellen
│   ├── bios/                  Laufzeit und Ereignisverarbeitung
│   ├── brainstem/             angeborene Pfade und Rückmeldungen
│   └── neural/                Neuronen, Synapsen und lokale Zustände
├── src/                       Implementierung und Versuchsprogramme
├── tests/                     automatisierte Kerntests
├── scripts/                   reproduzierbare Forschungsrunner
├── research/                  Parameter und maschinenlesbare Ergebnisse
├── docs/
│   ├── konzept/               Projekt- und Teilkonzepte
│   ├── experimente/           Aufträge und Forschungsberichte
│   ├── entscheidungen/        Architektur- und Projektentscheidungen
│   └── philosophie-und-ethik/ philosophische und ethische Grundlagen
├── CMakeLists.txt             CMake-Build
├── Makefile                   direkter Make-Build
└── README.md                  Einstieg und Projektübersicht
```

## Zentrale Dokumente

- [Projektkonzept](docs/konzept/EVE-Konzept.md)
- [Stammhirnkonzept](docs/konzept/EVE-Stammhirnkonzept.md)
- [Bericht zur Forschungsstufe B.3.1](https://github.com/Dntouch/EVE/blob/codex/research-stage-b31-neural-action-context/docs/experimente/011-forschungsbericht-forschungsstufe-b31.md)
- [Entscheidung gegen eine B.4-Freigabe](https://github.com/Dntouch/EVE/blob/codex/research-stage-b31-neural-action-context/docs/entscheidungen/0011-keine-freigabe-fuer-b4-nach-b31.md)
- [Forschungsfragen](docs/FORSCHUNGSFRAGEN.md)
- [Glossar](docs/GLOSSAR.md)
- [Hinweise für Beiträge](CONTRIBUTING.md)

## Projektgrundsätze

- Biologie dient als funktionale Vorlage, nicht als vollständig zu
  simulierendes Abbild.
- Der neuronale Kern erhält keine vorgegebene Semantik.
- Lernen muss sich als dauerhafte, überprüfbare Veränderung durch Erfahrung
  zeigen.
- BIOS, angeborene Regulation und lernendes Nervensystem bleiben getrennt.
- Experimente müssen reproduzierbar und Fehlschläge nachvollziehbar sein.
- Ein LLM darf Lehrer oder Vergleichssystem sein, aber keine verdeckte
  Entscheidungsinstanz des neuronalen Kerns.
- Ethik, Handlungsgrenzen und Verantwortung werden von Beginn an mitgeführt.
