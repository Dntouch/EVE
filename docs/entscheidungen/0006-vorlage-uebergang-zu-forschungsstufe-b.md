# 0006 – Übergang von A.2 zu Forschungsstufe B

**Status:** Entwurf – gemeinsame Entscheidung ausstehend

**Datum:** 2026-11-18

**Verantwortung:** offen

## Kontext

A.2 hat erstmals sieben Abschlüsse nachgewiesen, deren reale Impulsfolge von
vorausgegangenen Datenkontakten über den M3-Akkumulator bis zum
Abschlusskontakt reicht. Die identische unterbrochene M3-Topologie erzeugte
keinen nachgewiesenen Schaltungsabschluss. M1 und M2 scheiterten in der
Primärmatrix an Refraktärität.

M3 ist reproduzierbar, wenn der Ausgangszustand vor jeder Antwort
wiederhergestellt wird. Bei fortgeführtem Zustand verschwindet die
Motoraktivität nach der ersten Wiederholung. Damit existiert eine kausale
Handlungsgrundlage, aber noch kein stabiler wiederholter Handlungsbetrieb.

## Entscheidungskriterien

- Nur tatsächlich kausal nachgewiesene Handlungen dürfen Grundlage für lokale
  Ereigniszuordnung sein.
- Zufällige Abschlüsse und Abschlüsse ohne Daten bleiben getrennt.
- Forschungsstufe B darf keine versteckte Änderung der Refraktärregel
  enthalten.
- M3 bleibt konfigurierbar und wird nicht vorzeitig als endgültige Architektur
  festgeschrieben.
- Diagnoseinstrumentierung darf neuronales Verhalten nicht verändern.

## Optionen

### Option A – Forschungsstufe B sofort auf M3 aufbauen

Lokale Ereigniszuordnung wird zunächst nur für nachgewiesene M3-Handlungen aus
kontrolliert wiederhergestellten Zuständen untersucht.

Vorteil ist ein klarer kausaler Ausgangspunkt. Nachteil ist, dass Ergebnisse
noch nicht auf fortlaufende Handlungsfolgen übertragbar sind.

### Option B – Vor B ausschließlich die Wiederholungsstabilität lösen

Die nächste Stufe untersucht zunächst nur Antwortphasen, Refraktärzustand und
wiederholte Motoraktivität. Eine lokale Ereigniszuordnung wird verschoben.

Dies reduziert eine wichtige Störvariable, verzögert aber die Untersuchung der
eigentlichen Rückmeldungszuordnung.

### Option C – M1 oder M2 trotz negativem Kausalitätsbefund verwenden

Die einfacheren Schaltungen werden als Grundlage gewählt und ihre
Refraktärprobleme in B mitgeführt.

Diese Option besitzt in den A.2-Daten keinen nachgewiesenen Abschlussweg und
wird daher derzeit nicht empfohlen.

## Vorgeschlagene Entscheidung

Option A wird unter einer klaren Einschränkung zur gemeinsamen Prüfung
empfohlen: Forschungsstufe B beginnt mit kontrolliert wiederhergestellten
M3-Handlungen und verwendet ausschließlich nachgewiesene Schaltungsabschlüsse
für die lokale Ereigniszuordnung. Die Wiederholungsinstabilität wird parallel
als eigene Kontrollfrage geführt und darf nicht durch eine neue Lernregel oder
einen stillen Refraktärreset verdeckt werden.

Falls fortlaufende Handlungsfolgen zwingende Voraussetzung für den ersten
B-Versuch sind, ist stattdessen Option B zu wählen.

## Konsequenzen

### Positiv

- B erhält eine empirisch kausale Handlungsfolge statt zufälliger Abschlüsse.
- Die vorhandene Eigenrückmeldung kann unverändert angeschlossen werden.
- M3-Ausfall und M3-Erfolg bleiben über K2 kontrollierbar.

### Negativ oder begrenzend

- Die bedingte M3-Abschlussrate beträgt erst 31,8 %.
- Fortgeführte Wiederholungen sind noch nicht stabil.
- M1 und M2 bleiben ungeklärte Alternativen unter anderen Zustandsbedingungen.

## Verbindliche Kontrollen für B

- M3/K2 als unterbrochene Kontrolle beibehalten.
- Abschlüsse ohne Daten ausschließen.
- Nur rekonstruierte Schaltungswege einer Rückmeldung zuordnen.
- Wiederhergestellte und fortgeführte Zustände getrennt ausweisen.
- Refraktärzustände vor relevanten Eingängen weiter protokollieren.
- Keine Bewertung oder Gewichtsänderung in die technische Diagnose einbauen.

## Nachweise

- [Entwicklungsauftrag A.2](../experimente/006-entwicklungsauftrag-forschungsstufe-a2.md)
- [Forschungsbericht A.2](../experimente/006-forschungsbericht-forschungsstufe-a2.md)
- [Übergeordnetes Stammhirnkonzept](../konzept/EVE-Stammhirnkonzept.md)
- `research/results/research-stage-a2-causality.csv`
- `research/results/research-stage-a2-repetitions.csv`
- `research/results/research-stage-a2-diagnostics.csv`

## Überprüfung

Die Entscheidung ist nach gemeinsamer Auswertung zu bestätigen. Sie wird
spätestens überprüft, sobald M3 in fortgeführten Antwortphasen untersucht oder
eine erste lokale Ereigniszuordnung spezifiziert wird.
