# EVE

EVE ist ein Forschungsprojekt zur Entwicklung einer eigenständig lernenden,
biologisch inspirierten digitalen Intelligenz. Der neuronale Kern entsteht in
C++17 und arbeitet ohne vortrainiertes Large Language Model und ohne
vorgegebene Semantik.

> Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen,
> das eigenständig Intelligenz entwickeln kann.

EVE ist ein ausführbarer Forschungsprototyp: Eine deterministische,
ereignisgesteuerte Laufzeit verbindet ein eigenes neuronales Netz mit
angeborenen Stammhirnfunktionen, vollständiger Zustandspersistenz und
reproduzierbaren Versuchsreihen.

## Projektstatus

**Stand:** 21. November 2026

**Jüngste abgeschlossene Forschungsstufe:** B.5.13 – vollständige neuronale
Rekrutierbarkeit, Growth und interne Exploration

**Nächster Forschungsschwerpunkt:** Forschungsphase C – geschützte autonome
Strukturbildung unter natürlicher Konsequenz

B.5.13 zeigt, dass jedes nicht geschützte lernfähige Neuron intern exploriert
und durch Growth rekrutiert werden kann. Neu gewachsene Nullgewicht-Synapsen
erreichen mit den vorhandenen lokalen Regeln Eligibility, Gewicht 2 und
kausale Wiederverwendung. Auf den natürlichen historischen Aufgaben entsteht
jedoch kein unabhängig bestätigter Größenvorteil. Growth respektiert zudem die
Schutzreservierungen noch nicht; eine Produktionsaktivierung bleibt gesperrt.

| B.5.13-Befund | Ergebnis |
|---|---:|
| Rekrutierungsnetze | 3.840 |
| kontrollierte funktionale Growth-Pfade | 768/768 |
| gezielte Ablation entfernt den neuen Effekt | 768/768 |
| Netzgrößen | 32 · 128 · 256 · 512 · 2.048 |
| Growth-Ziele in geschützten Schnittstellen | 221/3.840 |
| schutzbereinigter natürlicher Repertoirevorteil | keiner |
| deterministische B.5.12-Dateien reproduziert | 15/15 |
| Regressionstests | 807/807 |

Vollständige Rekrutierbarkeit und tatsächliche funktionale Nutzung werden
künftig getrennt bewertet. S4 bleibt standardmäßig deaktiviert; weder eine
neue Lernregel noch dauerhaftes automatisches Growth ist freigegeben.

- [Forschungsbericht B.5.13](docs/experimente/037-forschungsbericht-forschungsstufe-b513.md)
- [Technische Architekturanalyse](docs/experimente/037-technische-architekturanalyse-rekrutierbarkeit.md)
- [Architekturentscheidung 0035](docs/entscheidungen/0035-b513-rekrutierbarkeit-bestaetigt-produktionsgrowth-gesperrt.md)
- [Entscheidungsvorlage Forschungsphase C](docs/experimente/037-entscheidungsvorlage-forschungsphase-c.md)

EVE ist weder eine fertige allgemeine Intelligenz noch ein Nachweis von
Bewusstsein oder bereits entstandener eigenständiger Intelligenz. Der Prototyp
dient dazu, einzelne Mechanismen kontrolliert zu implementieren, kausal zu
prüfen und auch negative Ergebnisse nachvollziehbar festzuhalten.

## Was bereits implementiert ist

- deterministische Ereignisverarbeitung mit simulierter Zeit,
- Neuronen, gewichtete Synapsen, Feuerereignisse und Erholungsmechanismen,
- getrennte erregende und experimentelle inhibitorische Synapsenwirkung,
- UTF-8-Sensoreingabe sowie angeborene sensorische und motorische Pfade,
- neuronale Ausgaben, motorische Abschlüsse und sensorische Rückkopplung,
- lokales neuronales Ereignisgedächtnis,
- lokale, zeitlich begrenzte Lernberechtigungen (Eligibility),
- abgestufte Bewertungsreize und lokale Drei-Faktor-Plastizität,
- Schutz angeborener Explorationssynapsen vor Plastizität,
- konfigurierbare Aktivitätsregulation und strukturelles Wachstum,
- versionierte Speicherung und Wiederherstellung vollständiger Zustände,
- Diagnostik, Ereignisprotokolle und Zustandsfingerabdrücke,
- 807 automatisierte Kerntests und reproduzierbare Forschungsrunner.

Die Forschungsvarianten aktivieren bewusst nur ausgewählte Mechanismen. Lernen,
inhibitorische Plastizität, Wachstum und S4 sind keine pauschal eingeschalteten
Produktionsfunktionen.

## Nachgewiesener Meilenstein

Am 18. November 2026 lernte EVE in Forschungsstufe B.4.2 erstmals nachweislich
eine Fähigkeit durch eigene Erfahrung: Bei einem wiederholten sensorischen Reiz
führte das Netz die von seiner Umwelt positiv bewertete Handlung aus.

- 64/64 Ausgangszustände erlernten Handlung A.
- 64/64 erlernten Handlung B, wenn die Umwelt sie bevorzugte.
- Nach Umkehr der Bewertung änderte EVE seine erworbene Präferenz.
- Der Erfolg blieb ohne weitere Gewichtsänderungen bestehen.
- Kontrollgruppen bestätigten den erfahrungsabhängigen Effekt.

Nachgewiesen ist damit das Lernen einer einzelnen Reiz-Handlungs-Beziehung,
nicht der dauerhaft robuste Erhalt mehrerer konkurrierender Beziehungen.

[Meilenstein 001 vollständig lesen](docs/meilensteine/001-erste-erlernte-faehigkeit.md)

## Forschungsweg

| Stufe | Schwerpunkt | Ergebnis |
|---|---|---|
| Experimente 001–003 | neuronaler Kern und Selbstregulation | technische Grundlage |
| A bis A.2 | Ausgabe, Sensorik, Motorik und Handlungskausalität | abgeschlossen |
| B bis B.3.1 | Ereignisgedächtnis, Erholung, Eligibility und Bewertung | lokale Lernvoraussetzungen |
| B.4 bis B.4.1 | Drei-Faktor-Plastizität und methodische Validierung | kausale Wirkung, Lernkriterium verfehlt |
| B.4.2 | Erhalt angeborener Exploration | erstes Einzelreiz-Lernen |
| B.4.3 bis B.4.7 | Interferenz, Struktur, Konkurrenz, Hemmung und Symmetrie | Mehrfachlernen bleibt instabil |
| B.4.8 bis B.5.1 | frühe Bindung, Fehlerkorrektur, Kredit und Wiederholung | Erwerb und Wiedererwerb vollständig, Erhalt instabil |
| B.5.2 bis B.5.4 | Gedächtnisverlust, Membranursachen und Recovery | transiente Abrufursachen lokalisiert |
| B.5.5 | Subthreshold-Membrandynamik | S4 hilft teilweise, bleibt experimentell |
| B.5.6 | Restfehler unter S4-d16 | sensorische Gewichtsverluste dominieren |
| B.5.7 | kausale sensorische Kreditzuordnung | zeitlich korrekt, kausal mehrdeutig |
| B.5.8 | Lernen, Nichtlernen und kausale Stabilität | Fähigkeitserhalt braucht keine Dauerplastizität; Umlernen schon |
| B.5.9 | lokale Entscheidungsmarge und positive Konkurrenz | Diagnose verbessert; zehn kausale Verlustverstärkungen; keine neue Regel |
| B.5.10 | einzelne Schreibkausalität und lokale Vorgeschichte | 13 historische Schritte kausal; 119 neue Ereignisse; keine neue Regel |
| B.5.11 | gemeinsame Repräsentation und Bewertungsvalidität | strenge Zeitwertung zeigt Interferenz, negativen Transfer und produktive Einzelfälle |
| B.5.12 | Teillernen, Verfestigung und Lernstadien | einfache latente Vorstrukturierung bestätigt; komplexe Übertragung und zuverlässige Beherrschung nicht bestätigt |
| B.5.13 | vollständige neuronale Rekrutierbarkeit, Growth und interne Exploration | technisch bestätigt; natürlicher Lernvorteil und Growth-Schutz noch offen |
| Forschungsphase C | geschützte autonome Strukturbildung | als nächster, noch nicht nummerierter Forschungsschwerpunkt vorbereitet |

Alle Aufträge, Methoden, Ergebnisse und Schlussfolgerungen stehen im
[Experimentverzeichnis](docs/experimente/README.md). Maschinenlesbare
Messdaten und Reproduktionshinweise liegen unter
[`research`](research/README.md).

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

Ein Zustand kann gespeichert und später deterministisch fortgesetzt werden:

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

## Jüngste Forschung reproduzieren

Der B.5.13-Runner baut die benötigten Programme, führt die Kerntests aus,
reproduziert B.5.12 und erzeugt Inventar, Rekrutierungs-, Kausal-, Größen- und
historische Aufgabenmatrizen neu:

```sh
./scripts/run-research-stage-b513.sh research/results/b513
```

Der Lauf verarbeitet 64 Seeds je Familie und Zuordnung in getrennten
Discovery-/Bestätigungsbereichen. Große Tabellen werden deterministisch als
gzip-Dateien gespeichert. Laufzeit und Speicherwerte sind maschinenabhängig;
Ereignisfolgen, neuronale Zustände und Fingerabdrücke sind bei identischem
Build, Seed und Eingabestrom deterministisch.

Runner älterer Forschungsstufen und die Bedeutung der Ergebnisdateien sind in
der [Forschungsdaten-Dokumentation](research/README.md) beschrieben.

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
│   ├── meilensteine/          zentrale nachgewiesene Fortschritte
│   └── philosophie-und-ethik/ philosophische und ethische Grundlagen
├── CMakeLists.txt             CMake-Build
├── Makefile                   direkter Make-Build
└── README.md                  Einstieg und Projektübersicht
```

## Dokumentation

- [Projektkonzept](docs/konzept/EVE-Konzept.md)
- [Stammhirnkonzept](docs/konzept/EVE-Stammhirnkonzept.md)
- [Forschungsfragen](docs/FORSCHUNGSFRAGEN.md)
- [Roadmap](docs/ROADMAP.md)
- [Glossar](docs/GLOSSAR.md)
- [Forschungsberichte und Aufträge](docs/experimente/README.md)
- [Entscheidungsprotokoll](docs/entscheidungen/README.md)
- [Philosophie und Ethik](docs/philosophie-und-ethik/README.md)
- [Änderungsverlauf](CHANGELOG.md)
- [Hinweise für Beiträge](CONTRIBUTING.md)
- [Sicherheitsrichtlinie](SECURITY.md)

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
