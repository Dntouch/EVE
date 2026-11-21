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

**Jüngste abgeschlossene Forschungsstufe:** C.0 – geschützte autonome
neuronale Selbstorganisation

**Nächster möglicher Forschungsschwerpunkt:** C.1 – Stabilisierung seltener
natürlicher S4-Pfade und Übergang zu S5

C.0 schließt die Schutzlücke aus B.5.13: semantische Rollen schützen
Sensorik, Motorik, Bewertung und Stammhirn vor Growth, während freie Neuronen
rekrutierbar bleiben. Interne Exploration wird nun durch lokale neuronale
Aktivität statt durch den Forschungsrunner ausgelöst. Die sichere autonome
Strukturbildung ist bestätigt. Seltene neue Pfade werden natürlich funktional
und kausal wirksam, sind aber in der untersuchten unmittelbaren Fortsetzung
schon nach einer weiteren Erfahrung wieder verloren. Der Verlust geht in allen
19 Fällen auf die konkrete Abschwächung der neuen Verbindung von 2 auf 1
zurück; 29 weitere Schreibungen waren isoliert ohne Funktionseffekt. Ein
Lernvorteil ist im untersuchten K3/K0-Vergleich nicht bestätigt.

| C.0-Befund | Ergebnis |
|---|---:|
| zentrale Discovery-/Bestätigungsläufe | 12.800 |
| aktuelle Kontrollläufe K0–K7 | 2.048 |
| K8 | ausschließlich historische B.5.13-Referenz |
| Netzgrößen | 32 · 128 · 256 · 512 · 2.048 |
| autonome interne Aktivierungen | 102.400 |
| geschützte Growth-Ereignisse | 166.261 |
| Schutzverletzungen | 0 |
| positive natürliche Growth-Pfade | 1.802 |
| natürliche funktionale und W1-kausale Pfade | 19/19 |
| davon unabhängige Bestätigung | 8 |
| nach unveränderter W0-Folgeerfahrung erhalten | 0/19 |
| isoliert kausale funktionsmindernde Schreibungen | 19/48 |
| Lernvorteil K3 gegenüber K0 im untersuchten Vergleich | keiner |
| Regressionstests | 832/832 |

Vollständige Rekrutierbarkeit und tatsächliche funktionale Nutzung werden
künftig getrennt bewertet. S4 bleibt standardmäßig deaktiviert; weder eine
neue Lernregel noch dauerhaftes automatisches Growth ist freigegeben.

- [Forschungsbericht C.0](docs/experimente/038-forschungsbericht-forschungsphase-c0.md)
- [C.0-Schutzvertrag](docs/experimente/038-schutzvertrag-autonome-strukturbildung.md)
- [Wissenschaftliche Analyse](docs/experimente/038-wissenschaftliche-analyse-c0.md)
- [Architekturentscheidung 0036](docs/entscheidungen/0036-c0-schutz-bestaetigt-funktionale-autonomie-offen.md)
- [Entscheidungsvorlage C.1](docs/experimente/038-entscheidungsvorlage-forschungsphase-c1.md)

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
- semantischer Schutzvertrag für autonome Strukturbildung,
- lokale, budgetierte und persistierbare autonome interne Exploration,
- versionierte Speicherung und Wiederherstellung vollständiger Zustände,
- Diagnostik, Ereignisprotokolle und Zustandsfingerabdrücke,
- 832 automatisierte Kerntests und reproduzierbare Forschungsrunner.

Die Forschungsvarianten aktivieren bewusst nur ausgewählte Mechanismen. Lernen,
inhibitorische Plastizität, Wachstum und S4 sind keine pauschal eingeschalteten
Produktionsfunktionen.

## Nachgewiesene Meilensteine

### 002 – Erste funktionale neuronale Rekrutierung

Am 21. November 2026 band EVE in Forschungsstufe B.5.13 erstmals nachweislich
ein zuvor unverbundenes Neuron über interne Exploration, Growth und vorhandene
lokale Plastizität in einen funktionalen Verarbeitungspfad ein.

- 3.840/3.840 technische Rekrutierungsversuche erzeugten neue Struktur.
- 768/768 kontrollierte Pfade wurden bis zur kausalen Zielaktivierung
  entwickelt.
- Die gezielte Ablation der neuen Verbindung entfernte den Effekt in 768/768
  Fällen.
- Persistenz und deterministische Fortsetzung blieben erhalten.

Der Nachweis bestätigt die technische Rekrutierbarkeit, noch nicht die autonome
Auswahl nützlicher neuer Struktur. Growth-Schutz und natürlicher funktionaler
Nutzen bleiben Gegenstand der Forschungsphase C.

[Meilenstein 002 vollständig lesen](docs/meilensteine/002-erste-funktionale-neuronale-rekrutierung.md)

### 001 – Erste erlernte Fähigkeit

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
| B.5.13 | vollständige neuronale Rekrutierbarkeit, Growth und interne Exploration | technisch bestätigt; schutzbereinigter Lernvorteil nicht bestätigt, Growth-Schutz offen |
| C.0 | geschützte autonome neuronale Selbstorganisation | Schutz, autonome Struktur und 19 natürliche S3-Pfade bestätigt; W0-Fortsetzung 0/19, kein Vorteil im untersuchten K3/K0-Vergleich |
| C.1 | Stabilisierung S4→S5 | als mögliche Folgestufe vorbereitet, noch nicht beauftragt |

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

Der C.0-Runner baut die benötigten Programme, führt alle Kerntests aus und
erzeugt Schutz-, Autonomie-, Growth-, Stufen-, Kausal-, Größen- und
Kontrollmatrizen neu:

```sh
./scripts/run-research-stage-c0.sh
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
