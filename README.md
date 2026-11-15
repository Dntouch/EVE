# EVE

EVE ist ein Forschungsprojekt zur Entwicklung einer eigenständig lernenden,
biologisch inspirierten digitalen Intelligenz. Der neuronale Kern soll ohne
vortrainiertes Large Language Model und ohne vorgegebene Semantik auskommen.

## Projektstatus

**Phase:** Projektaufbau und Forschungskonzeption  
**Konzeptstand:** 15. November 2026  
**Prototyp-Horizont:** zwölf Wochen ab freigegebenem Projektstart

Dieses Repository enthält derzeit bewusst **keinen Programmcode**. Es schafft
die organisatorische, wissenschaftliche und dokumentarische Grundlage für die
spätere Entwicklung. Ein Technologie-Stack ist noch nicht festgelegt.

## Leitgedanke

> Wir wollen nicht eine Intelligenz programmieren, sondern ein System schaffen,
> das eigenständig Intelligenz entwickeln kann.

## Verbindliche Projektgrundsätze

- Biologie dient als funktionale Vorlage, nicht als vollständig zu simulierendes
  Abbild.
- Der neuronale Kern erhält keine vorgegebene Semantik.
- Lernen muss sich als dauerhafte, überprüfbare Veränderung durch Erfahrung
  zeigen.
- Entscheidungen und ihre Konsequenzen bilden die Grundlage des Lernens.
- BIOS, angeborene Regulation und lernendes Nervensystem werden begrifflich und
  technisch getrennt.
- Experimente müssen reproduzierbar und Fehlschläge nachvollziehbar sein.
- Energieverbrauch ist eine messbare Rahmenbedingung, nicht das oberste Ziel.
- Ein LLM darf Lehrer oder Vergleichssystem sein, aber keine verdeckte
  Entscheidungsinstanz des neuronalen Kerns.
- Ethik, Handlungsgrenzen und Verantwortung werden von Beginn an mitgeführt.

## Repository-Struktur

```text
.
├── .github/                    Issue- und Pull-Request-Vorlagen
├── docs/
│   ├── entscheidungen/         Architektur- und Projektentscheidungen
│   ├── experimente/            Regeln und Vorlage für Versuchsprotokolle
│   ├── konzept/                Vollständige Projektbeschreibung
│   ├── ARBEITSWEISE.md         Git- und Dokumentationsprozess
│   ├── FORSCHUNGSFRAGEN.md     Offene Forschungsfragen
│   ├── GLOSSAR.md              Gemeinsame Begriffe
│   ├── RISIKEN.md              Wissenschaftliche und ethische Risiken
│   └── ROADMAP.md              Zwölf-Wochen-Zielbild
├── CHANGELOG.md                Nachvollziehbare Projektänderungen
├── CONTRIBUTING.md             Regeln für Beiträge
└── README.md                   Einstieg und Projektübersicht
```

## Einstieg

1. Das [vollständige Konzept](docs/konzept/EVE-Konzept.md) lesen.
2. Offene Punkte in den [Forschungsfragen](docs/FORSCHUNGSFRAGEN.md) prüfen.
3. Neue Grundsatzentscheidungen vor der Umsetzung im
   [Entscheidungsprotokoll](docs/entscheidungen/README.md) festhalten.
4. Versuche vor ihrer Durchführung anhand der
   [Experimentvorlage](docs/experimente/VORLAGE.md) beschreiben.
5. Änderungen über ein passendes Issue und einen nachvollziehbaren Pull Request
   einbringen.

## Noch nicht entschieden

- Programmiersprache und Technologie-Stack
- konkrete neuronale Minimalarchitektur
- zuerst umzusetzende Plastizitätsmechanismen
- erste nichtsemantische Lernumgebung
- Lizenz und Veröffentlichungsmodell
- Hardware- und Messumgebung

Diese Punkte werden nicht stillschweigend vorweggenommen, sondern als
nachvollziehbare Entscheidungen dokumentiert.
