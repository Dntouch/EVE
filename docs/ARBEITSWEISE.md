# Arbeitsweise

## Ziel

Die Projektarbeit soll Entscheidungen, Annahmen und Versuchsergebnisse dauerhaft
nachvollziehbar machen. Dokumentation ist Teil des Forschungsprozesses und keine
nachträgliche Zusammenfassung.

## Von der Frage zur Änderung

1. **Frage erfassen:** Forschungsfrage, Aufgabe oder Risiko als Issue anlegen.
2. **Hypothese formulieren:** Erwartung und mögliche Widerlegung festhalten.
3. **Entscheidung dokumentieren:** Grundlegende oder schwer umkehrbare Wahl als
   Entscheidungsprotokoll erfassen.
4. **Experiment planen:** Aufbau, Messgrößen, Kontrollbedingungen und
   Abbruchkriterien vor Beginn definieren.
5. **Änderung prüfen:** Ergebnisse und Dokumente in einem Pull Request prüfen.
6. **Erkenntnisse einordnen:** Bestätigung, Widerlegung oder offene Unsicherheit
   dokumentieren; Folgefragen verlinken.

## Branches

Der Hauptbranch heißt `main` und soll jederzeit den nachvollziehbaren,
freigegebenen Projektstand enthalten.

Empfohlene Präfixe:

- `docs/` für Dokumentationsänderungen
- `research/` für Literatur- und Forschungsarbeit
- `decision/` für Entscheidungsprotokolle
- `experiment/` für Versuchsplanung und -auswertung
- `chore/` für organisatorische Pflege

## Commits

Empfohlenes Format:

```text
<Typ>: <kurze Änderung im Imperativ>

Motivation: ...
Auswirkungen: ...
Bezug: #Issue oder Dokument
```

Jeder Commit soll fachlich verständlich und unabhängig prüfbar sein. Ergebnisse
und Behauptungen dürfen nicht ohne ihre Bedingungen oder Quellen eingecheckt
werden.

## Pull Requests

Ein Pull Request beschreibt:

- das gelöste Problem oder die bearbeitete Frage,
- den fachlichen Zusammenhang,
- die konkrete Änderung,
- mögliche Risiken und offene Punkte,
- die Prüfung der Änderung,
- betroffene Entscheidungen, Experimente und Forschungsfragen.

## Dokumentstatus

Dokumente können einen der folgenden Zustände tragen:

- **Entwurf:** wird erarbeitet und ist nicht verbindlich.
- **In Prüfung:** bereit für fachliche Rückmeldung.
- **Angenommen:** aktuell verbindlicher Projektstand.
- **Ersetzt:** durch ein verlinktes neueres Dokument abgelöst.
- **Verworfen:** bewusst nicht weiterverfolgt, mit Begründung.

## Quellen und Evidenz

Quellenangaben enthalten nach Möglichkeit Autor, Titel, Jahr, dauerhafte URL oder
DOI und die konkrete für EVE relevante Aussage. Eine Quelle belegt nicht mehr,
als tatsächlich aus ihr hervorgeht.

## Daten und Artefakte

Große, lokale, sensible oder automatisch erzeugte Daten gehören nicht ohne
Prüfung in Git. Für jedes Experiment ist festzulegen:

- welche Rohdaten dauerhaft erforderlich sind,
- welche Metadaten Reproduktion ermöglichen,
- welche Daten nicht veröffentlicht werden dürfen,
- wie Integrität und Herkunft nachgewiesen werden,
- wann Daten archiviert oder gelöscht werden.
