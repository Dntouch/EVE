# EVE

EVE ist ein Forschungsprojekt zur Entwicklung einer eigenständig lernenden,
biologisch inspirierten digitalen Intelligenz. Der neuronale Kern wird in C++
entwickelt und kommt ohne vortrainiertes Large Language Model sowie ohne
vorgegebene Semantik aus.

> Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen,
> das eigenständig Intelligenz entwickeln kann.

## Projektstatus

**Stand:** 18. November 2026

**Jüngste abgeschlossene Forschungsstufe:** B.4.3 – lokale Trennung konkurrierender Erfahrungen

**Aktueller Forschungsschwerpunkt:** lokale Koinzidenzbindung ohne globale Reizkennung

EVE ist inzwischen ein ausführbarer Forschungsprototyp. Er besitzt eine
deterministische, ereignisgesteuerte Laufzeit, ein eigenes neuronales Netz,
angeborene Stammhirnfunktionen, persistierbare Zustände und reproduzierbare
Versuchsreihen.

B.4.2 weist den Verlust explorativer Wege durch negative Modulation kausal
nach. Ein allgemeiner Schutz angeborener Explorationssynapsen erhält beide
Handlungsalternativen. Damit lernt EVE einen wiederholten Einzelreiz für Ziel A
wie B und passt die Präferenz nach Umweltumkehr an. Die Zwei-Reiz-Aufgabe bleibt
trotz Verbesserung gegenüber der Nullkontrolle ungelöst.

B.4.3 lokalisiert den verbleibenden Engpass: Im unveränderten G3-Fenster
stammten 30,15 % der sensorisch klassifizierbaren Lernkandidaten aus der
jeweils anderen Erfahrung. Sequenzielles Lernen überschreibt die erste
Beziehung kausal. Engere lokale Fenster reduzieren die Vermischung, genügen
aber noch nicht für gleichzeitiges Mehrfachlernen.

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
- standardmäßig deaktivierte, lokale Drei-Faktor-Plastizität für B.4-Versuche,
- nicht-erregende neuronale Modulationskontakte, die weder Membran noch G3 füllen,
- persistierbarer Plastizitätsschutz für angeborene Explorationssynapsen,
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
| B.3.1 | neuronaler Kontext zur Handlungszuordnung | abgeschlossen, nicht ausreichend |
| B.4 | erste rückmeldungsabhängige Plastizität | abgeschlossen, Lernkriterium verfehlt |
| B.4.1 | methodische Validierung des Lernversuchs | abgeschlossen, kausale Wirkung ja, Lernen nein |
| B.4.2 | Erhalt angeborener Exploration | abgeschlossen, Einzelreiz-Lernen ja, Zwei-Reiz-Lernen nein |
| B.4.3 | lokale Trennung konkurrierender Erfahrungen | abgeschlossen, Interferenz kausal, Mehrfachlernen nein |

Ausführliche Aufträge, Methoden, Ergebnisse und Schlussfolgerungen stehen unter
[`docs/experimente`](docs/experimente/). Die zugehörigen maschinenlesbaren
Messdaten liegen unter [`research/results`](research/results/).

## Schnellstart

Benötigt werden ein C++17-fähiger Compiler und entweder GNU Make oder CMake ab
Version 3.16.

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
./scripts/run-research-stage-b42.sh research/results/b42
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
- [Bericht zur Forschungsstufe B.4.3](docs/experimente/015-forschungsbericht-forschungsstufe-b43.md)
- [Entscheidung nach B.4.3](docs/entscheidungen/0015-keine-freigabe-mehrfachlernen-nach-b43.md)
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
