# 0002 – Technologie-Stack bleibt zunächst offen

**Status:** Angenommen  
**Datum:** 2026-11-15  
**Verantwortung:** Projektleitung

## Kontext

Die Initialbeschreibung formuliert funktionale Anforderungen, trifft aber keine
Entscheidung über Programmiersprache, Laufzeit, Speicherformat, Messwerkzeuge
oder Hardware. Eine vorschnelle Wahl würde Annahmen schaffen, bevor Anforderungen
und Bewertungskriterien ausreichend konkretisiert sind.

## Entscheidung

Im organisatorischen Projektaufbau wird kein Technologie-Stack festgelegt und
keine Implementierungsstruktur vorgetäuscht. Eine spätere Auswahl benötigt ein
eigenes Entscheidungsprotokoll mit mindestens folgenden Kriterien:

- deterministische und reproduzierbare Ausführung,
- ereignisgesteuerte Effizienz,
- genaue Beobachtbarkeit zeitlicher Abläufe,
- persistenter und prüfbarer Systemzustand,
- kontrollierbare Parallelität,
- Messbarkeit interner und realer Ressourcen,
- langfristige Wartbarkeit und Verständlichkeit,
- Unabhängigkeit von vortrainierten neuronalen Frameworks.

## Konsequenzen

### Positiv

- Funktionale Anforderungen bestimmen die Werkzeuge, nicht umgekehrt.
- Alternativen können anhand gleicher Kriterien verglichen werden.

### Negativ oder begrenzend

- Technische Ordner, Build-Prozess und Entwicklungsumgebung bleiben offen.
- Eine Implementierung kann erst nach der Auswahl sauber begonnen werden.

## Überprüfung

Vor Beginn von Phase 1 der Roadmap. Die Entscheidung wird dann nicht still
geändert, sondern durch ein neues Protokoll ersetzt.
