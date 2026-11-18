# Entwicklungsauftrag 015 – Forschungsstufe B.4.3

## Lokale Trennung konkurrierender Erfahrungen

Ausgangsbasis ist Commit
`0f505b0b6a7c579f254cabaa1293af0df2c2b64f` der Branch
`codex/research-stage-b42-exploration-preservation`. B.4.2 bleibt unverändert
reproduzierbar und wird zur Regression.

## Forschungsfrage

Kann EVE die Beziehungen X→A und Y→B durch eigene Erfahrung gleichzeitig
lernen und behalten, ohne Reizkennung, Handlungs-ID, technische Zielauswahl
oder globalen neuronalen Takt?

Getrennt zu messen sind Reizunterscheidung, rückmeldungsabhängiges Lernen und
Erhalt der ersten Beziehung beim Lernen der zweiten. Eine allgemeine
Motorpräferenz zählt nicht als Mehrfachlernen.

## Architekturgrenzen

- BIOS: Zeit, Ereignisse, Persistenz, technische Versuchssteuerung und
  unabhängige Diagnose.
- Stammhirn: geschützte Exploration, lokale Eligibility-Bildung und
  Modulationsverarbeitung.
- Netzwerk: binäre Feuerereignisse, lokale Aktivität, Gewichte und erworbener
  Zustand.
- Keine Reiz-ID oder Episodennummer in Synapse, Eligibility oder Spike.
- Kein globaler Tick, kein vollständiger periodischer Netzdurchlauf, keine
  vorgegebene richtige Handlung und kein Zustandsreset zwischen Lernblöcken.
- Die vorhandene Drei-Faktor-Regel bleibt Ausgangspunkt; neue allgemeine
  Lernalgorithmen sind nicht Teil dieser Stufe.

## Ausgangskonfiguration

64 Neuronen, Feuergrenze 2, Gewichtsgrenze 3, G3-Kapazität 8,
8 Kandidaten, 8 Entscheidungen, Dauer 16, R2F, geschützte symmetrische
Exploration, deaktiviertes Wachstum und mindestens 64 Seeds. 256 Neuronen
dienen als Übertragbarkeitskontrolle.

## Vorab definierte Versuche

1. Reproduktion von Einzelreiz A/B, beiden Umkehrungen und beiden
   Zwei-Reiz-Zielzuordnungen.
2. L0 ohne Plastizität, L1 unverändertes G3, L2 letzter feuerauslösender
   Eingang, L3 lokales Feuerentscheidungsfenster und L4 laufende
   prä-/postsynaptische Beteiligung.
3. Kandidaten- und Entscheidungskapazitäten 1, 2, 4 und 8 sowie Dauern
   4, 8, 16, 32 und 64.
4. Nur X, nur Y, X→Y, Y→X, alternierend, zufällig gemischt und 16er-Blöcke.
5. Separate, verteilte und teilweise überlappende binäre Eingänge.
6. Keine, positive, ternäre, reduzierte, zufällige und vertauschte Modulation.
7. Diagnostische Zustandskopie mit Rücknahme der Änderungen des zweiten
   Lernblocks.
8. 64/256 Neuronen, Ressourcen, Stabilität und Persistenz.

Alle zentralen Gruppen verwenden dieselben vorab erzeugten Aufgaben-,
Explorations- und Rückmeldungsströme je Seed. Rohdaten werden ausschließlich
unter `research/results/b43/` abgelegt.

## Messung und Erfolg

Für X und Y werden A, B, beide und keine getrennt gezählt. Primär ist der
Mittelwert der beiden korrekten eindeutigen Erfolgsquoten; beide Einzelquoten
müssen berichtet werden. Das Vergessensmaß ist die Erfolgsquote vor minus nach
dem Lernen der zweiten Beziehung.

Forschungsziel sind je mindestens 75 % korrekte eindeutige Ausgaben, mindestens
15 Prozentpunkte Vorteil zur gepaarten No-Learning-Kontrolle, ein positives
95-%-Intervall und keine vollständige Auslöschung der ersten Fähigkeit. Erst
nach erfolgreichem Zwei-Beziehungs-Lernen darf die Kapazität auf 4–16 Reize
erweitert werden.

## Lieferumfang

C++17-Runner, lokale Kandidatenauswahl, 42 zusätzliche Tests, vollständige
Kontrollmatrix, Rohdaten, Gewichtsdaten, Statistik, Verwechslungsmatrizen,
Interferenz- und Kausaldiagnose, Ressourcen-/Persistenzdaten, Forschungsbericht
und Entscheidungsvorlage.

