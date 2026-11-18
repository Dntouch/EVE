# Forschungsbericht – Forschungsstufe B.4

## A. Forschungsfrage

Kann eine begrenzte lokale Aktivitätsspur zusammen mit einem späteren
neuronalen Modulationssignal synaptische Gewichte verändern und dadurch das
beobachtbare Verhalten verbessern?

## B. Versuchsaufbau

Der Runner erzeugt zwei neuronale Ausgänge A und B. Eine virtuelle Umgebung
bewertet ausschließlich das tatsächlich beobachtete Ergebnis A, B, beide oder
keines. Sie übermittelt nur die Polarität −1, 0 oder +1 über die reguläre
Bewertungsroute. Der Mechanismus erhält keine Zielhandlung und keine
Synapsenliste.

Untersucht wurden E1–E3, T0–T3, Kapazitäten 2/4/8,
Lebensdauern 4/16/64, positive und negative Änderungen, K0–K15,
Belohnungsumkehr sowie 64 gepaarte Seeds. Nach 128 Trainingserfahrungen folgten
64 Auswertungserfahrungen ohne Modulation. Die 42 zusätzlichen automatisierten
Matrixprüfungen decken die zentralen Kombinationen ab.

## C. Vorprüfung

Beide Ausgänge waren strukturell und funktional erreichbar. Die
Startpräferenzen wurden je Seed gespiegelt, so dass kein Ausgang über alle
Seeds bevorzugt war. Beide- und Keine-Ausgabe wurden explizit protokolliert.

## D. Technische Umsetzung

Die Laufzeit besitzt getrennte Ereignisarten für Modulationsfeuer und
Modulationsimpulse. Nur der lokale Empfänger liest seine maximal acht
Eligibility-Einträge. Grenzen, Budget, Null-zu-eins-Änderungen, verbrauchte und
rückmeldungsursprüngliche Spuren werden gezählt. Persistenzformat 9 erhält
Konfiguration, Metriken und wartende Ereignisse. B.3.1-Kontextsignaturen sind
im Primärversuch deaktiviert.

## E. Primärergebnis

E2 änderte Gewichte (512 Verstärkungen, 256 Abschwächungen über alle Seeds),
verbesserte das Verhalten aber nicht belastbar gegenüber K0:

| Variante | frühe mittlere Belohnung | Auswertung | Delta | Lern-Seeds |
|---|---:|---:|---:|---:|
| K0 | 0,016602 | 0,026367 | 0,009766 | 0,437500 |
| E2 | 0,016602 | 0,026367 | 0,009766 | 0,437500 |
| E1 | −0,103516 | −0,150391 | −0,046875 | 0,453125 |
| E3 | −0,432617 | −0,539551 | −0,106934 | 0,312500 |

Damit ist Gewichtsänderung nachgewiesen, erfahrungsabhängiges Lernen im
geforderten Verhaltenssinn jedoch nicht.

## F. Kontrollen

K0–K15 wurden tatsächlich ausgeführt. Ohne Exploration blieb die Belohnung bei
−1. Zufällige Modulation fiel auf −0,907715, umgekehrte Rückmeldung auf −1.
Die verzögerte Rückmeldung nach Ablauf der Spur erzeugte keine Änderung. Das
alte unmodulierte Lernen verschlechterte die Auswertung auf −0,763184.
Rückmeldungsursprüngliche Spuren bewirkten keine rekursive Änderung.

## G. Verzögerung und Überlappung

Unmittelbare Rückmeldung konnte lokale Änderungen auslösen. Bei Verzögerung 16
und gleichzeitigem Ablauf der Spur wurden null Änderungen beobachtet. Die
Kontrollen bestätigen damit Zeitgebundenheit, zeigen aber noch keine robuste
Zuordnung bei überlappenden Handlungen.

## H. Belohnungsumkehr

Nach Umkehr in der Mitte fiel die Auswertungsbelohnung auf −1,000000. Die
frühere Zuordnung konnte nicht zuverlässig abgeschwächt und neu aufgebaut
werden. Dieses Kriterium ist klar nicht erfüllt.

## I. Stabilität und Ressourcen

Gewichte blieben in [0,3], der Eingangsetat wurde geprüft, Spurkapazitäten
blieben fest und die Netzwerk-Invarianten bestanden. Gemessen wurden 44.944
Byte bei 64 Neuronen (702,25 Byte/Neuron einschließlich Laufzeit-Overhead).
Die lineare Oberabschätzung beträgt rund 7,02 MB für 10.000, 70,23 MB für
100.000 und 702,25 MB für eine Million Neuronen; sie ist keine reale
Großnetzmessung.

## J. Reproduzierbarkeit und Persistenz

Gleiche Seeds erzeugen identische Routen, Ereignisse und CSV-Zeilen. Das
versionierte Format speichert den vollständigen neuen Zustand. Die gesamte
Testsuite umfasst 215 von 215 bestandene Tests, darunter 42 neue B.4-Fälle.

## K. Interpretation

Die drei technischen Faktoren reichen aus, um lokal kausale und begrenzte
Gewichtsänderungen hervorzurufen. Sie reichen in dieser minimalen Umgebung
nicht aus, um einen Vorteil gegenüber unverändertem Verhalten zu erzeugen.
E1 verteilt Modulation zu breit; E3 nimmt zu viele bloße
Membranbeteiligungen auf. E2 ist selektiver, verändert in diesem Aufbau aber
überwiegend bereits verhaltenswirksame Gewichte.

## L. Grenzen

Die Umgebung ist klein, binär und ohne komplexe Sequenzen. Konfidenzintervalle
werden durch die vollständige Seed-Verteilung statt durch ein asymptotisches
Modell beurteilt. Gestufter und aktivitätsabhängiger Zerfall sind als begrenzte
Konfigurationsvarianten geprüft, aber noch kein biologisches Modell. Die
Ressourcenwerte sind überwiegend Projektionen.

## M. Schlussfolgerung

B.4 erfüllt die technische Lokalitäts- und Sicherheitsfrage, verfehlt aber das
zentrale Lernkriterium. Entscheidung 0011 wird nicht aufgehoben. Allgemeine
rückmeldungsabhängige Plastizität bleibt gesperrt.

## N. Nächste Frage

Vor einer Erweiterung muss untersucht werden, wie Exploration nach
Abschwächung alternative Pfade wieder erreichbar macht, ohne Zielhandlung oder
globale Aktionskennung einzuführen.

## O. Nachweise

- `research/results/b4/raw_trials.csv`: 245.760 tatsächliche Episoden plus Kopfzeile
- `research/results/b4/summary.csv`: gepaarte Aggregation aller Varianten
- `research/results/b4/resources.csv`: Messung und gekennzeichnete Projektionen
- `src/research_stage_b4.cpp`: reproduzierbarer Runner
