# EVE

EVE ist ein Forschungsprojekt zur Entwicklung einer eigenständig lernenden,
biologisch inspirierten digitalen Intelligenz. Der neuronale Kern wird in C++
entwickelt und kommt ohne vortrainiertes Large Language Model sowie ohne
vorgegebene Semantik aus.

> Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen,
> das eigenständig Intelligenz entwickeln kann.

## Projektstatus

**Stand:** 18. November 2026

**Jüngste abgeschlossene Forschungsstufe:** B.4.8 – frühe Erfahrungsbindung und adaptive Exploration

**Aktueller Forschungsschwerpunkt:** lokale Korrektur fehlbewerteter autonomer Entscheidungen

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

B.4.4 zeigt, dass zusätzliches Synapsenwachstum diesen Engpass nicht löst:
Rund 104–112 neue Verbindungen je zentralem Lauf bleiben bei Gewicht null und
ändern das Verhalten nicht. Die bestehende `active_phase`-Auswahl erhält zwei
sequenziell trainierte Beziehungen auch ohne Wachstum deutlich besser, bleibt
aber bei alternierenden und gemischten Erfahrungen unzureichend.

B.4.5 schließt ältere Eligibility-Spuren und bloße Kandidatenkapazität als
dominante Ursache des Mischfehlers aus. Doppelte Motoraktivierung blockiert
dagegen wirksame Korrektur. Gezielte negative Ambiguitätsrückmeldung verbessert
die Mischleistung stark, bleibt aber asymmetrisch und weit unter dem
Robustheitsziel. Eine technische Winner-take-all-Auswahl wird nicht eingeführt.

B.4.6 weist allgemeine lokale Hemmung technisch nach. Eine getrennte
synaptische Wirkungsart und gleichzeitige lokale Integration bleiben bei
vertauschter Queue-Reihenfolge deterministisch. Lernbare Hemmung verbessert
die Mischleistung auf 83,85 %, erreicht aber nur 35/64 robuste Seeds und in
der unabhängigen Bestätigung 30/64. Die vertauschte Motorzuordnung legt einen
verbleibenden Handlungsbias offen; die allgemeine Lernfreigabe bleibt gesperrt.

B.4.7 widerlegt eine technische Nummernpräferenz: 1.088/1.088 konsistente
Sensor-, Motor-, Explorations-, Modulations-, Neuronen- und
Synapsenabbildungen sind logisch exakt gleich. Der verbleibende Fehler hängt
stattdessen stark vom konkreten Explorationsstrom ab. E1 ohne zusätzliche
Exploration erreicht nur 68,41 %, die inverse Zielzuordnung mit unverändertem
Strom 59,00 %. Lernbare Hemmung wirkt kausal, ist fester Hemmung aber weiterhin
nicht statistisch überlegen. Zugleich wird eine feste 67:61-Kopplung der
historischen binären Zufallsströme entdeckt. Nach ihrer isolierten Korrektur
fallen normale und inverse Mischleistung auf 70,07 % und 73,78 % bei höchstens
16/64 robusten Seeds. Das Robustheitsziel bleibt verfehlt.

B.4.8 zeigt mit vollständigen Zustandskopien, dass autonome Bindung bereits
nach 4–8 Erfahrungen entsteht, danach aber nicht stabil bleibt. Gelernte
Reizgewichte tragen E1 kausal; falsche eindeutige Antworten verhindern jedoch
weitere geschützte Exploration. Eine minimale negative-rückmeldungsabhängige
Zusatzexploration erreicht höchstens 13/64 robuste Seeds und reproduziert
ihren kleinen Entdeckungsvorteil weder über drei Stromfamilien noch in der
Bestätigung. Sie wird nicht als Stammhirnfunktion freigegeben.

EVE ist damit weder eine fertige allgemeine Intelligenz noch ein Nachweis, dass
eigenständige Intelligenz bereits entstanden ist. Der Prototyp dient dazu,
einzelne Mechanismen kontrolliert zu implementieren, zu messen und auch
negative Ergebnisse nachvollziehbar festzuhalten.

## Was bereits implementiert ist

- deterministische Ereignisverarbeitung mit simulierter Zeit,
- Neuronen, gewichtete Synapsen, Feuerereignisse und Erholungsmechanismen,
- getrennte erregende und experimentelle inhibitorische Synapsenwirkung,
- optionale lokale Integration technisch gleichzeitiger Eingänge,
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
| B.4.4 | strukturelles Lernen und synaptische Interferenz | abgeschlossen, Struktur für Sequenzen ausreichend, Wachstum wirkungslos |
| B.4.5 | lokale Entscheidungsbindung und motorische Konkurrenz | abgeschlossen, H2 kausal, robustes Mischlernen nein |
| B.4.6 | lokale neuronale Hemmung und erfahrungsabhängige Konkurrenz | abgeschlossen, technische Hemmung ja, robustes biasfreies Lernen nein |
| B.4.7 | neuronale Symmetrie und robuste Entscheidungsbildung | abgeschlossen, technische Rollen invariant, Erfahrungsstromabhängigkeit bleibt |
| B.4.8 | frühe Erfahrungsbindung und adaptive Exploration | abgeschlossen, frühe instabile Bindung nachgewiesen, adaptive Exploration nicht freigegeben |

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
./scripts/run-research-stage-b46.sh research/results/b46
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
- [Bericht zur Forschungsstufe B.4.4](docs/experimente/016-forschungsbericht-forschungsstufe-b44.md)
- [Entscheidung nach B.4.4](docs/entscheidungen/0016-strukturelle-kapazitaet-ausreichend-nach-b44.md)
- [Bericht zur Forschungsstufe B.4.5](docs/experimente/017-forschungsbericht-forschungsstufe-b45.md)
- [Entscheidung nach B.4.5](docs/entscheidungen/0017-motorische-konkurrenz-nach-b45.md)
- [Bericht zur Forschungsstufe B.4.6](docs/experimente/018-forschungsbericht-forschungsstufe-b46.md)
- [Entscheidung nach B.4.6](docs/entscheidungen/0018-inhibitorische-grundfunktion-bleibt-experimentell.md)
- [Bericht zur Forschungsstufe B.4.7](docs/experimente/019-forschungsbericht-forschungsstufe-b47.md)
- [Entscheidung nach B.4.7](docs/entscheidungen/0019-konkurrenzlernen-bleibt-erfahrungsstromabhaengig.md)
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
