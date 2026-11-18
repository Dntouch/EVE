# EVE

EVE ist ein Forschungsprojekt zur Entwicklung einer eigenständig lernenden,
biologisch inspirierten digitalen Intelligenz. Der neuronale Kern wird in C++
entwickelt und kommt ohne vortrainiertes Large Language Model sowie ohne
vorgegebene Semantik aus.

> Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen,
> das eigenständig Intelligenz entwickeln kann.

## Projektstatus

**Stand:** 18. November 2026

**Jüngste abgeschlossene Forschungsstufe:** B.4.2 – Erhalt explorativer Handlungsfähigkeit

**Aktueller Forschungsschwerpunkt:** lokale Trennung mehrerer Reiz-Handlungs-Zuordnungen

EVE ist inzwischen ein ausführbarer Forschungsprototyp. Er besitzt eine
deterministische, ereignisgesteuerte Laufzeit, ein eigenes neuronales Netz,
angeborene Stammhirnfunktionen, persistierbare Zustände und reproduzierbare
Versuchsreihen.

Die Forschungsreihe B.4 bis B.4.2 untersucht erstmals lokale
rückmeldungsabhängige Plastizität. B.4 bestätigt die technische Wirksamkeit
der Drei-Faktor-Regel, erreicht aber noch keine Verhaltensverbesserung. B.4.1
weist die kausale Wirkung der Gewichtsänderung auf die Motorausgabe nach und
grenzt den Verlust explorativer Handlungswege als Ursache ein. B.4.2 schützt
angeborene Explorationssynapsen vor negativer Modulation: Damit lernt EVE einen
wiederholten Einzelreiz für beide Zielhandlungen und passt seine Präferenz nach
einer Umweltumkehr an. Die Zwei-Reiz-Aufgabe bleibt trotz einer Verbesserung
gegenüber der Nullkontrolle ungelöst.

Die Forschungsstufen bleiben als eigenständige Branches erhalten. Der jüngste
vollständige Implementierungsstand liegt auf
[`codex/research-stage-b42-exploration-preservation`](https://github.com/Dntouch/EVE/tree/codex/research-stage-b42-exploration-preservation);
`main` bildet den stabilen Einstieg, das übergeordnete Konzept und die
gemeinsame Forschungsübersicht ab.

EVE ist damit weder eine fertige allgemeine Intelligenz noch ein Nachweis, dass
eigenständige Intelligenz bereits entstanden ist. Der Prototyp dient dazu,
einzelne Mechanismen kontrolliert zu implementieren, zu messen und auch
negative Ergebnisse nachvollziehbar festzuhalten.

## Meilenstein 001: Die erste erlernte Fähigkeit

Am 18. November 2026 hat EVE erstmals nachweislich eine Fähigkeit durch eigene
Erfahrungen erworben. In Forschungsstufe B.4.2 lernte das neuronale Netzwerk,
bei einem wiederholten sensorischen Reiz die von seiner Umwelt positiv
bewertete Handlung auszuführen.

- 64 von 64 Ausgangszuständen erlernten erfolgreich Handlung A.
- 64 von 64 erlernten erfolgreich Handlung B, wenn die Umwelt sie bevorzugte.
- Nach einer Umkehr der Bewertung veränderte EVE ihre erworbene Präferenz.
- Der Lernerfolg blieb ohne weitere synaptische Gewichtsänderungen bestehen.
- Kontrollgruppen bestätigten, dass der Effekt erfahrungsabhängig war.

Der Durchbruch entstand durch die Trennung angeborener Explorationsfähigkeit
von erlernbaren neuronalen Verbindungen. EVE erhielt dabei keine vorgegebene
richtige Antwort. Kein vortrainiertes Modell, kein LLM und kein
einprogrammierter Lösungsweg waren Teil des neuronalen Lernprozesses. Die Grenze
bleibt klar: Nachgewiesen ist bislang das Lernen einer einzelnen
Reiz-Handlungs-Beziehung, nicht die zuverlässige Unterscheidung mehrerer
konkurrierender Reize.

**Eine kleine neuronale Struktur, die durch Erfahrung ihr Verhalten
verändert.**

[Meilenstein 001 vollständig lesen](docs/meilensteine/001-erste-erlernte-faehigkeit.md)

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
- nicht-erregende neuronale Modulationskontakte,
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
| [B.4](https://github.com/Dntouch/EVE/blob/codex/research-stage-b4-three-factor-plasticity/docs/experimente/012-forschungsbericht-forschungsstufe-b4.md) | erste rückmeldungsabhängige Plastizität | abgeschlossen, Lernkriterium verfehlt |
| [B.4.1](https://github.com/Dntouch/EVE/blob/codex/research-stage-b41-learning-validation/docs/experimente/013-forschungsbericht-forschungsstufe-b41.md) | methodische Validierung des Lernversuchs | abgeschlossen, kausale Wirkung ja, Lernen nein |
| [B.4.2](https://github.com/Dntouch/EVE/blob/codex/research-stage-b42-exploration-preservation/docs/experimente/014-forschungsbericht-forschungsstufe-b42.md) | Erhalt angeborener Exploration | abgeschlossen, Einzelreiz-Lernen ja, Zwei-Reiz-Lernen nein |

Ausführliche Aufträge, Methoden, Ergebnisse und Schlussfolgerungen stehen in
den jeweils verlinkten Forschungsbranches unter `docs/experimente`. Die
zugehörigen maschinenlesbaren Messdaten liegen dort unter `research/results`.

## Schnellstart

Benötigt werden ein C++17-fähiger Compiler und entweder GNU Make oder CMake ab
Version 3.16.

Nach dem Klonen wird für den jüngsten ausführbaren Forschungsstand der
eigenständige B.4.2-Branch ausgecheckt:

```sh
git switch codex/research-stage-b42-exploration-preservation
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
│   ├── meilensteine/           zentrale nachgewiesene Fortschritte
│   └── philosophie-und-ethik/ philosophische und ethische Grundlagen
├── CMakeLists.txt             CMake-Build
├── Makefile                   direkter Make-Build
└── README.md                  Einstieg und Projektübersicht
```

## Zentrale Dokumente

- [Projektkonzept](docs/konzept/EVE-Konzept.md)
- [Stammhirnkonzept](docs/konzept/EVE-Stammhirnkonzept.md)
- [Meilenstein 001 – Die erste erlernte Fähigkeit](docs/meilensteine/001-erste-erlernte-faehigkeit.md)
- [Bericht zur Forschungsstufe B.4.2](https://github.com/Dntouch/EVE/blob/codex/research-stage-b42-exploration-preservation/docs/experimente/014-forschungsbericht-forschungsstufe-b42.md)
- [Entscheidung zum begrenzten Lernnachweis](https://github.com/Dntouch/EVE/blob/codex/research-stage-b42-exploration-preservation/docs/entscheidungen/0014-begrenztes-einzelreiz-lernen-nach-b42.md)
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
