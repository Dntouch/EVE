# Entscheidung 0018: Inhibitorische Grundfunktion bleibt experimentell

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0017

## Kontext

B.4.6 führt eine getrennte synaptische Wirkungsart und lokale gleichzeitige
Integration ein. Hemmung wirkt reproduzierbar, bleibt bei vertauschter
Queue-Reihenfolge invariant, ist persistent und kann durch die bestehende
Drei-Faktor-Plastizität verändert werden.

Der beste Kandidat verbessert die gemischte Leistung gegenüber B.4.5 von
76,82 % auf 83,85 %. Entdeckung und Bestätigung überschreiten beide
Einzelschwellen, erreichen aber nur 35/64 beziehungsweise 30/64 robuste Seeds
statt 48/64. Eine vertauschte Motorzuordnung weist zusätzlich einen
signifikanten verbleibenden Handlungsbias nach. Lernbare Hemmung ist gegenüber
fester Hemmung nicht eindeutig kausal überlegen.

## Entscheidung

Die getrennte inhibitorische Wirkungsart, sichere Membranbegrenzung,
versionierte Persistenz und optionale lokale Batch-Integration bleiben als
allgemeine **experimentelle** neuronale Fähigkeiten im Kern.

Nicht freigegeben werden:

- inhibitorische Konkurrenz als allgemeiner Lernmechanismus,
- eine konkrete angeborene Motor-Gegenschaltung,
- Winner-take-all- oder globale Motorselektion,
- die beitragsbewusste inhibitorische Lernrichtung als Standardregel.

Bestehende Synapsen und Versuche behalten standardmäßig rein erregende,
sequenzielle Verarbeitung. B.4.6 aktiviert die neuen Funktionen explizit.

## Auftrag für B.4.7

B.4.7 soll den verbleibenden Zuordnungsbias mit symmetrischen Sensor-, Motor-,
Explorations- und Feedbackrollen isolieren. Erst wenn vertauschte Zuordnungen
gepaarte Gleichwertigkeit und mindestens 48/64 robuste Seeds in Entdeckung und
Bestätigung erreichen, darf eine breitere Freigabe erneut geprüft werden.

Die Leitlinie bleibt: Hemmung darf neuronale Konkurrenz verändern, aber keine
technisch ausgewählte richtige Handlung kodieren.
