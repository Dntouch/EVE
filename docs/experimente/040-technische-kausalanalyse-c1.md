# Technische Kausalanalyse C.1

## Status der Analyse

Dieses Dokument ersetzt die zu weitgehende ursprüngliche Interpretation. Die
H0–H3-Analyse, Alternativpfad-Ablationen und vollständige Zeitleiste sind eine
am 21. November 2026 ergänzte post-hoc Methodenkorrektur; sie waren kein Teil
der ursprünglichen Präregistrierung.

## Drei getrennte Kausalfragen

1. **Aktuelle Motorentscheidung:** H0 gegen H1, Intervention vor der
   Erfahrung.
2. **Spätere Gewichtsschreibung:** W0 gegen W1 und die nächste Entscheidung.
3. **Unmittelbare Zielaktivierung:** diagnostische Senderprobe mit und ohne
   ausgewählte Synapse.

Diese Fragen dürfen nicht aus demselben Vergleich beantwortet werden.

## H0–H3 vor der natürlichen Erfahrung

| Variante | Intervention | Ergebnis über 38 Fälle |
|---|---|---|
| H0 | unveränderte Synapse, natürliche Erfahrung | 38 falsche Doppelantworten |
| H1 | ausgewählte Synapse vorher deaktiviert | 38 korrekte Einzelantworten |
| H2 | unveränderte Kontrollkopie | 38/38 exakt identisch zu H0 |
| H3 | konkurrierende Eingänge kombiniert entfernt | separater Konkurrenznachweis |

Alle Varianten beginnen aus identischem Zustand. `b511_episode` berechnet die
Rückmeldung aus der tatsächlich entstandenen Handlung; H1 erhält daher in
allen Fällen positive statt negativer Modulation. Neben Handlung und
Korrektheit werden frühe Aktivierung, Doppelaktivierung, direkte Zielaktivität
sowie die Zahl aktiver Neuronen und Synapsen gespeichert.

Ergebnis: Der ausgewählte Pfad ist in 38/38 Fällen handlungskausal und trägt
zur falschen Doppelantwort bei. Eine vor der Erfahrung wirksame Unterdrückung
wäre situativ nützlich. Dieser Befund sagt nicht, dass die erst danach
ausgeführte Schreibung die bereits vergangene Handlung verändert.

## W0–W9 und Schreibkausalität

Die bisherigen W0–W9-Gegenproben bleiben erhalten. W1 unterdrückt nur die
kritische 2→1-Schreibung; W2 reproduziert W0 deterministisch; W3–W5 bestätigen
die Rolle von Eligibility, Schreibung und lokaler Modulationswirkung. W6–W9
prüfen Konkurrenz, Growth und Exploration.

W0 und W1 zeigen bei der aktuellen Handlung definitionsgemäß keinen
Unterschied, weil ihre Intervention erst beim späteren Feedback greift. Auch
die unmittelbar nächste Entscheidung unterscheidet sich in 0/38 Fällen. Die
Schreibung ist dagegen in 38/38 Fällen kausal für den Verlust der direkten
Zielaktivierung.

## Alternative Eingänge

Die Korrektur verwendet fünf Evidenzstufen:

- **strukturell:** anderer Eingang zum Ziel vorhanden,
- **potenziell funktional:** Gewicht erreicht die Feuerschwelle,
- **tatsächlich funktional:** isolierter kontrollierter Senderimpuls aktiviert
  das Ziel,
- **kausal relevant:** Einzelablation verändert eine neuronale Beobachtung,
- **fähigkeitsrelevant:** Einzel- oder Kombinationsablation verändert den
  kontrollierten Abruf von Aufgabe A oder B.

Über alle 38 ausgewählten Pfade bestehen je zwei kontrolliert aktivierbare
Alternativen. Eine Alternative pro Fall ist fähigkeitskausal; ihre zusätzliche
Ablation bei bereits entferntem ursprünglichem Pfad reduziert das Repertoire.
Die kombinierte Ablation bestätigt diesen Übernahmebefund in 38/38 Fällen.

Keine Alternativablation verändert dagegen die neuronale Antwort der
kritischen natürlichen Erfahrung. Die Alternativen übernehmen somit eine
kontrolliert nachgewiesene Abruffunktion, ohne dass ihre Beteiligung am
ursprünglichen Fehler behauptet wird. Nur für diesen Fähigkeitsabruf wird von
funktionaler Reorganisation gesprochen.

## Zeitlich vollständige Funktionsdiagnose

W0 und W1 werden nach jedem Erfahrungsschritt 0–256 in einer diagnostischen
Kopie geprüft. Die Lerntrajektorie selbst bleibt dadurch unverändert.

- erster erneuter Funktionsnachweis: Schritt 10–47,
- Fälle mit Wiederkehr: 38/38,
- funktionale Schritte nach dem ersten Verlust: 388 insgesamt, 9–15 je Fall,
- erneute Verlustereignisse: 356,
- direkte Funktion bei Schritt 256: 0/38.

Der frühere Kontrollpunktbefund `0/38 bei 256` bleibt korrekt. Die daraus
abgeleitete Aussage einer durchgehend fehlenden Funktion war falsch und ist
korrigiert.

## Langzeitklassifikation

Die unveränderten Schwellen ergeben weiter 16 nützliche, zehn schädliche,
sieben neutrale und fünf kontextabhängige W0/W1-Verläufe. Jede Datenzeile
weist nun aus:

- geprüfte Fähigkeit A/B,
- nachgewiesenen Unterschied in korrekten Erfahrungen oder Endrepertoire,
- fähigkeitsrelevante Rolle alternativer Pfade,
- Abhängigkeit von der durch W0/W1 auseinanderlaufenden Lerntrajektorie.

Die Klassifikation ist keine isolierte Bewertung der 2→1-Schreibung und keine
Freigabe einer Lernregel.

## Datenquellen

- `action-causality.csv`: H0–H3,
- `situational-appropriateness.csv`: korrigierte situative Bewertung,
- `alternative-path-evidence.csv`: Einzel- und Kombinationsablationen,
- `direct-function-timeline.csv`: Diagnose jedes Schritts,
- `long-term-assessment.csv`: begrenzte W0/W1-Langzeitaussage,
- `causal-matrix.csv.gz`: unveränderte W0–W9-Gegenproben.

Alle Dateien liegen unter `research/results/c1/method-correction`.
