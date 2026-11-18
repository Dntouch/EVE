# EVE – Forschungsstufe B.2
## Lokale neuronale Lernberechtigung und verzögerte Rückmeldungszuordnung

**Ausgangsbasis:** `codex/research-stage-b1-neural-recovery`  
**Implementierung:** C++17  
**Status:** umgesetzt und ausgewertet

## Forschungsfrage

Kann ein Neuron beim eigenen Feuern aus seinem unveränderten lokalen
G3-Ereignisgedächtnis eine kurzlebige, eigenständige Markierung möglicher
Lernkandidaten erzeugen, die bei einer späteren technischen Rückmeldung noch
verfügbar sein kann?

Eine Eligibility-Markierung behauptet weder Ursache noch Erfolg. Sie hält nur
fest, welche lokalen Eingänge bei einer konkreten Feuerentscheidung für eine
spätere Veränderung infrage kommen könnten.

## Verbindliche Varianten

- L0: deaktiviert.
- L1: alle zuletzt in G3 enthaltenen Eingänge, einschließlich Gewicht 0.
- L2: nur Eingänge mit zum Entscheidungszeitpunkt positivem Gewicht.
- Kandidatenkapazität: 0, 2, 4 oder 8 Synapsen-IDs.
- Darstellung: wiederholte IDs oder jede ID einmal.
- mehrere Entscheidungen: Überschreiben, begrenztes Beibehalten oder
  Zusammenführen gleicher Kandidaten.
- Dauer: D0 = nur im Feuerturn, D1 = 4, D2 = 16, D3 = 64
  Simulationseinheiten.

Die Standardvariante verwendet eindeutige Synapsen-IDs, zwei Kandidaten je
Entscheidung und zwei getrennte Entscheidungsmarkierungen je Neuron.

## Architekturgrenzen

- G3 und Eligibility sind getrennte Zustände.
- Kandidaten entstehen unmittelbar bei der neuronalen Feuerentscheidung.
- Die technische M3-Diagnose darf sie nur lesen und vergleichen.
- Ablauf erfolgt ereignisgesteuert und ohne globalen Netzwerkdurchlauf.
- Es gibt keine Zeitstempel pro Synapse.
- Ablauf verändert weder Feueraktivität, Membran, G3, Synapsen noch Gewichte.
- Lernwirkung, Belohnung, Bestrafung, STDP und Rückwärtsdiagnose bleiben
  ausdrücklich deaktiviert.

## Versuchsgrundlage und Lieferumfang

Die Versuche verwenden 256 Neuronen, 1.024 reguläre Startsynapsen, V2 mit
Gewicht 3, M3 mit Gewichten 1/3, Schwelle 3, G3/8, R2F/16, E4-neu sowie alle
64 etablierten Seeds. Enthalten sind Auswahl-, Dauer-, Rückmeldungs-,
Fortbetriebs-, Kontroll-, Persistenz- und Ressourcenmatrizen einschließlich
2, 8, 32 und 128 fortgeführten Antwortphasen.

Die vollständigen Anforderungen sind im Forschungsbericht, in der technischen
Spezifikation, in 25 neuen automatisierten Tests und in den maschinenlesbaren
Dateien `research/results/research-stage-b2-*.csv` nachvollziehbar umgesetzt.

