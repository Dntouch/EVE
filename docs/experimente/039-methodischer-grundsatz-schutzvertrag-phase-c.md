# Methodischer Grundsatz – Schutzvertrag in Forschungsphase C

## Geltungsbereich

Dieser Grundsatz gilt verbindlich für jede Forschungsstufe der Phase C, die
Growth, strukturelle Rekrutierung oder eine andere Änderung der neuronalen
Topologie untersucht. Er übernimmt den Hinweis aus
[GitHub-Issue #9](https://github.com/Dntouch/EVE/issues/9) und konkretisiert
[Architekturentscheidung 0037](../entscheidungen/0037-schutzvertrag-ist-voraussetzung-nicht-lernziel.md).

## Freigabereihenfolge

1. Schutzrollen und erlaubte Richtungen werden außerhalb des neuronalen
   Lernzustands konfiguriert.
2. Der eigenständige strukturelle Sicherheits-Preflight wird ausgeführt.
3. Nur wenn sämtliche Negativ- und Positivprüfungen bestehen, werden die
   allgemeinen Regressionstests und anschließend die Lernexperimente
   gestartet.
4. A – Integrität, B – Selbstorganisation und C – Lernen werden getrennt
   berichtet.

Ein Fehler in Schritt 2 beendet den Runner mit Fehlerstatus. Vorhandene
positive Lernergebnisse ändern diese Entscheidung nicht.

## Technische Referenz

Der wiederverwendbare Preflight wird als
`_build/eve_structural_safety_preflight` gebaut. C.0 ruft ihn im Runner vor
allen Lernexperimenten auf und speichert das Ergebnis als
`research/results/c0/structural-safety-preflight.csv`.

Die Prüfung enthält gezielte unerlaubte Growth-Versuche. Damit ist sie nicht
von der empirischen Beobachtung abhängig, dass in natürlichen C.0-Läufen null
Schutzverletzungen auftraten.

## Bewertungsgrenzen

Der Vertrag bestimmt ausschließlich den zulässigen Veränderungsraum. Er darf
weder Zielstruktur noch richtige Aufgabe, Motorhandlung oder erfolgreiche
Gewichte codieren. Insbesondere folgt aus dem Schutz angeborener
Schnittstellen kein Schutz bereits erfolgreicher lernfähiger Synapsen.

Die verbindliche Ergebnisstruktur lautet:

| Bewertung | Leitfrage | Kompensation zulässig? |
|---|---|---:|
| A – Integrität | Sind alle Sicherheitsbedingungen unabhängig bestanden? | nein |
| B – Selbstorganisation | Welche Struktur und Funktion entstand? | nicht anwendbar |
| C – Lernen | Welche Fähigkeit wurde kausal neu oder besser? | nicht anwendbar |

Eine gemeinsame Gesamtpunktzahl wird nicht gebildet.

