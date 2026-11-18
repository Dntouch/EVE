# 0004 – Folgestufe nach der neuronalen Ausgabeschnittstelle

**Status:** Entwurf

**Datum:** 2026-11-18

**Verantwortung:** offen

## Kontext

Forschungsstufe A hat die technische motorische Schnittstelle erfolgreich
bereitgestellt. In 249 Versuchen entstand jedoch keine neuronale
Abschlussaktivität. Mit den bestehenden Lernregeln feuerten in 10 von 96
Versuchszeilen einzelne Datenkontakte, aber keine Antwort war gültig
abgeschlossen.

Ohne zuverlässig erzeugte und abgeschlossene Handlungen kann eine spätere
Rückmeldung keiner vollständigen Antwort zugeordnet werden.

## Entscheidungskriterien

- Keine fertigen Antworten oder Zielwerte im Stammhirn.
- Weiterhin vollständig ereignisgesteuerte Verarbeitung.
- Messbare Trennung von Exploration, Antwortabschluss und Lernen.
- Reproduzierbarkeit bei identischem Seed und Ausgangszustand.
- Keine Vorwegnahme der rückmeldungsabhängigen Lernregel aus Forschungsstufe B.

## Betrachtete Optionen

### Option A – Unmittelbar mit lokaler Ereigniszuordnung fortfahren

Forschungsstufe B wird wie ursprünglich geplant begonnen. Unvollständige
motorische Aktivierungen dienen bereits als mögliche Handlungen.

Vorteil ist die direkte Fortsetzung der Forschungsreihenfolge. Nachteil ist,
dass eine Rückmeldung weiterhin keiner gültig abgeschlossenen Antwort
zugeordnet werden kann. Fehlende Exploration und fehlender Abschluss würden die
Auswertung von B überlagern.

### Option B – Begrenzte Zwischenstufe für Exploration und Abschluss

Vor B wird isoliert untersucht, wie das neuronale System ohne Lösungswissen
unterschiedliche motorische Aktivitätsmuster und einen Abschluss erzeugen kann.
Zufall darf neuronale Aktivität anregen, aber keine fertigen Bytes im Stammhirn
erzeugen.

Vorteil ist eine prüfbare Handlungsgrundlage für spätere Rückmeldung. Nachteil
ist eine zusätzliche Forschungsstufe und die Gefahr, einen zu starken
angeborenen Mechanismus einzuführen.

### Option C – Abschluss technisch erzwingen

Das BIOS beendet jede Antwort nach einem festen Ereignislimit und wertet die
bis dahin gesetzten Kanäle als gültig.

Dies vereinfacht Versuche, widerspricht aber der in Forschungsstufe A
festgelegten Unterscheidung zwischen Nullbyte, fehlender und neuronaler
Antwort. Die Option würde fehlende neuronale Abschlussfähigkeit verdecken.

## Vorgeschlagene Entscheidung

Option B wird zur fachlichen Prüfung empfohlen. Die Zwischenstufe muss
Exploration und Abschluss getrennt messen und darf weder Zielmuster noch
Bewertungen einführen. Option C wird verworfen, weil sie eine technische
Beendigung fälschlich als neuronale Antwort ausgeben würde.

## Konsequenzen

### Positiv

- Forschungsstufe B erhält klar abgegrenzte, vollständige Handlungen.
- Fehlende Exploration wird als eigenes Problem sichtbar.
- Die technische Ausgabeschnittstelle aus A bleibt unverändert.

### Negativ oder begrenzend

- Zusätzlicher experimenteller Aufwand.
- Ein angeborener Explorationsreiz muss eng begrenzt und kontrolliert werden.
- Ein Abschlusskontakt kann weiterhin zufällig statt funktional feuern.

### Offene Folgearbeit

- Varianten für spontane, stochastische neuronale Aktivierung definieren.
- Datenaktivität und Abschluss getrennt auswerten.
- Kontrollen ohne Explorationsreiz vorsehen.
- Erst danach lokale Ereignisspuren und Rückmeldung untersuchen.

## Nachweise und Verknüpfungen

- [Entwicklungsauftrag Forschungsstufe A](../experimente/004-entwicklungsauftrag-forschungsstufe-a.md)
- [Forschungsbericht Forschungsstufe A](../experimente/004-forschungsbericht-forschungsstufe-a.md)
- [Übergeordnetes Stammhirnkonzept](../konzept/EVE-Stammhirnkonzept.md)
- research/results/research-stage-a-results.csv

## Überprüfung

Die Entscheidung ist zu überprüfen, sobald mindestens eine
Explorationsvariante reproduzierbar Datenkanäle und den Abschlusskontakt
aktiviert, ohne Eingaben oder Zielwerte direkt zu kopieren.
