# Technische Analyse B.5.11 – Repräsentation und Lerninterferenz

## Forschungsumgebung

Der Runner `eve_research_stage_b511` verwendet die unveränderte Runtime und
die bestehenden Drei-Faktor-, Eligibility-, Explorations- und
Modulationspfade. Sieben externe Elemente sind sieben normale Sensorneuronen.
Jedes Element besitzt symmetrische nullgewichtete Verbindungen zu beiden
Entscheidungsneuronen. Nur die Umwelt ordnet eine vollständige zeitliche Folge
einer Zielhandlung zu.

Eine Episode feuert das erste Element, lässt vier simulierte Zeiteinheiten
vergehen und feuert das zweite. Nur eine eindeutige Motorhandlung nach dem
zweiten Element gilt als autonomer Abruf. Jede frühe Handlung macht die
Episode auch dann falsch, wenn später der Zielmotor feuert. Eine frühe,
fehlende, doppelte oder andere eindeutige Handlung bleibt in der
Antwortmatrix separat sichtbar.
Falls keine späte Antwort entsteht, aktiviert der bestehende geschützte Pfad
die vorab erzeugte Explorationsfolge. Die Planung selbst zählt nicht als
Handlung: Erst ein tatsächlich beobachtetes eindeutiges Motorfeuer bestimmt
positive Modulation. Unterdrückte, ausgebliebene, falsche, frühe oder doppelte
Motoraktivität wird negativ bewertet. Fünf Regressionstests sichern diese
Trennung gegen erneute Verwechslung von Explorationsplan und Motorausführung.

## Bewertungsisolation

Jeder Kontextabruf arbeitet auf einer vollständigen Runtime-Kopie. Die sechs
Kontexte sind direkt, nach A, nach B, nach kurzer ereignisfreier Zeit, nach
langer ereignisfreier Zeit und nach einer A/B-Folge. Die Trainingsruntime wird
durch diese Diagnosen nicht verändert. Die Rohdaten speichern erwartete
äußere Bewertung, tatsächliche Antwortklasse, frühe Ausgabe und Korrektheit.

V1 bis V10 werden als eigene Spalten und Statistiken ausgegeben. Es existiert
kein gewichteter Gesamtscore. Das Repertoire zählt nur für diese Aufgabe, wie
viele bedingte Abrufraten mindestens 0,5 erreichen. Historische E1/E2-Werte
bleiben über die frisch ausgeführten B.5.7–B.5.10-Runner erhalten.

## Komponenten- und Kausaldiagnose

Fire- und Impulsbeobachter erfassen aktive Neuronen und Synapsen je Aufgabe.
Schnittmengen sind zunächst nur Überlappung. Für die kleine vorab begrenzte
Detailmenge werden nichtnullgewichtete gemeinsame Synapsen einzeln auf null
gesetzt und A/B auf Kopien erneut geprüft. Geschützte allgemeine Ausgabewege
werden ausdrücklich als Engpässe markiert. Eine kombinierte Ablation prüft
Redundanz, ersetzt aber nicht den Einzelbeleg gemeinsamer Repräsentation.

Die Reorganisationsanalyse kopiert den exakten Zustand vor einer B-Erfahrung:

- W0 führt alle natürlichen Schreibungen aus.
- W1 unterdrückt genau den ersten identifizierten Schreibkontakt.
- W2 wiederholt W0 ohne Eingriff.

Kandidat, Ereignisindex, Vorher- und Zielgewicht müssen am tatsächlichen
Kontakt übereinstimmen. Danach erhalten W0, W1 und W2 dieselben äußeren
A/B-Pläne, erzeugen aber ihre eigenen Handlungen und Rückmeldungen. Die
langfristige Klasse wird ausschließlich aus dem Repertoirevergleich gebildet:
W0 > W1 ist produktiv, W0 < W1 schädlich und W0 = W1 unentschieden.
Gewichtserhöhung und -senkung werden getrennt ausgewiesen. Ebenso getrennt
bleiben unmittelbare A-Beeinträchtigung, deren tatsächliche Verhinderung durch
W1 und ein bloßes Fortbestehen der Beeinträchtigung trotz Auslassung.

## Membran- und Kapazitätskontrollen

R0 nutzt inputabhängige Erholung ohne Refraktärzeit, R2 unabhängige Recovery
mit 50-%-Membranrelaxation, S4 zusätzlich lokale Subthreshold-Relaxation nach
16 Zeiteinheiten. Aufgaben, Seeds, Budgets und Kontexte sind identisch. S4
bleibt außerhalb des Experiments aus.

Die Kapazitätskontrolle erhöht die Neuronenzahl von 32 auf 64, fügt aber keine
zielkodierten Pfade hinzu. Das identische Ergebnis zeigt lediglich, dass
ungenutzte Kapazität nicht genügt. Es beweist weder eine harte
Topologiegrenze noch die Nützlichkeit strukturellen Wachstums.

K25 sperrt auf der neuen Aufgabe ausschließlich synaptische Schreibungen,
während Aktivität, Eligibility und Modulation weiterlaufen. K26 reaktiviert
die Schreibungen nach 128 Erfahrungen. Damit bleiben No-Learning,
Schreibsperre und reaktivierte Plastizität technisch getrennte Kontrollen.

## Persistenz und Ressourcen

Die neue Umgebung fügt keinen persistenten Zustand hinzu. Ein S4-Zustand mit
`future_event_count == 1` wird gespeichert und geladen; Fingerabdruck und
Fortsetzung sind identisch. Beobachter und Interceptor werden weiterhin nicht
persistiert. Die Produktionstopologie bleibt während der zentralen Versuche
konstant und valide.

Große Rohdaten werden mit `gzip -n` deterministisch komprimiert. Maßgebliche
Dateien sind `response-matrix.csv.gz`, `network-results.csv.gz`,
`v1-v10-dimensions.csv.gz`, `causal-ablations.csv`,
`productive-reorganization.csv` und `transfer-c.csv.gz` unter
`research/results/b511`.

## Technische Entscheidung

Die Infrastruktur greift nicht in neuronales Lernen ein und kann integriert
werden. Die Korrektur betrifft ausschließlich Umweltbewertung und
Forschungsklassifikation; sie fügt keine neuronale Produktionsregel hinzu.
Nicht freigegeben sind Aufgabenlabels im Netz, automatische
Repräsentationszusammenführung, ein Schreibselektor, Rollback, globaler Reset,
strukturelles Wachstum und S4 als Standard.
