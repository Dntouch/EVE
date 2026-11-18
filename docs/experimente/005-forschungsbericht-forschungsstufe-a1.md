# Forschungsstufe A.1 – Angeborene Organisation und sensomotorische Rückkopplung

## Kurzfassung

Forschungsstufe A.1 ergänzt vier voneinander abschaltbare Mechanismen: angeborene
gerichtete Pfade, begrenzte Exploration, motorische Abschlussgruppen und eine
sensorische Rückmeldung tatsächlich beobachteter Motorereignisse. Alle
Mechanismen benutzen die vorhandene Ereigniswarteschlange, normale
Feuerereignisse und kompakte Synapsen. Das Stammhirn erzeugt weder Zielbytes
noch eine direkte Eingabe-Ausgabe-Kopie.

Die vollständige Matrix enthält 168 Läufe aus 28 Konfigurationen und sechs
festen Seeds. Positive angeborene Pfade mit Gewicht 3 erzeugten in 5 von 6
Läufen Motoraktivität; die Referenz, rein strukturelle Pfade und funktionale
Pfade mit Gewicht 1 erzeugten keine. Ein zuverlässiger neuronaler Abschluss
entstand dennoch nicht: In der Integrationsstufe IV war nur 1 von 6 Antworten
abgeschlossen. Einzelne Abschlüsse ohne motorische Grundschaltung zeigen, dass
Exploration den Abschlusskontakt auch zufällig treffen kann.

Die Eigenrückmeldung ist technisch nachgewiesen. R1 führte nach dem einzigen
gültigen Abschluss ein Sensorereignis zurück, R2 führte alle zehn beobachteten
Datenmotorereignisse einzeln zurück, und R3 erzeugte fünf getrennte
Umweltbeobachtungsereignisse. Diese Ereignisse erhöhten die neuronale Aktivität,
veränderten Gewichte aber nicht direkt. Kein Lauf überschritt sein Ereignisbudget.

Das Ergebnis ist eine funktionsfähige, reproduzierbare sensomotorische
Forschungsgrundlage, aber noch kein Nachweis einer zuverlässigen
Handlungsentscheidung, eines erlernten Abschlusses oder von Sprachfähigkeit.

## A. Technische Umsetzung

### A.1 Angeborene Grundverschaltung

Die Varianten V1 bis V3 erzeugen für jeden der acht Datenkontakte und den
Abschlusskontakt einen gerichteten Pfad aus einer reproduzierbar gewählten
Quelle, zwei allgemeinen Relais und dem Motorziel. Als Quellen dienen die acht
sensorischen Kontakte sowie der unabhängige Antwortreiz. Quellen, Relais und
Ziele werden getrennt ausgewählt; es gibt insbesondere keine feste Zuordnung
von Eingangsbit n zu Ausgangsbit n.

Vorhandene gerichtete Synapsen werden wiederverwendet. Deshalb benötigen die
neun Pfade mit jeweils drei Kanten abhängig vom Netz-Seed 26 oder 27 zusätzliche
Synapsen. Selbstverbindungen und Duplikate bleiben ausgeschlossen. V1 verwendet
Gewicht 0, V2 und V3 ein konfigurierbares positives Gewicht. V3 variiert die
Topologie über einen eigenen Seed.

### A.2 Explorative Aktivität

E0 bis E4 wählen keine, eine, mehrere, räumlich lokale oder über Regionen
verteilte Neuronenaktivierungen. Die Auswahl besitzt einen eigenen lokalen
Zufallszustand und verändert den Zufallszustand der Netzinitialisierung nicht.
Jede Auswahl wird ausschließlich als normales Feuerereignis eingereiht. Anzahl
und Budget sind explizite Versuchsparameter.

### A.3 Motorische Grundschaltungen

M1 verbindet Datenkontakte neuronisch mit dem Abschlusskontakt. M2 verwendet
vier reproduzierbar gewählte Relais als Abschlussgruppe. M3 führt
Motoraktivität in einem Akkumulator zusammen und benötigt mehrere Eingänge,
bevor dieser über eine weitere Synapse den Abschluss erreicht. In allen drei
Fällen ist ein Feuerereignis des Abschlusskontakts erforderlich; das BIOS
beendet keine Antwort als Ersatz.

### A.4 Sensorische Rückmeldung

Die Rückmeldung benutzt die reservierten Neuronen 9 bis 17, während
Motorzuordnungen für A.1 erst ab Neuron 18 beginnen. Überschneidungen werden
abgewiesen.

- R1 codiert nach einem tatsächlichen Abschluss das beobachtete Byte und einen
  eigenen Abschlussmarker. Dadurch bleibt auch ein gültiges Nullbyte
  wahrnehmbar.
- R2 führt jedes tatsächlich beobachtete Datenmotorereignis in seiner
  ursprünglichen Reihenfolge zurück.
- R3 wendet die Ausgabe auf einen minimalen deterministischen Umweltzustand an
  und codiert anschließend den unabhängig berechneten Folgezustand.

Die Verarbeitung läuft über die reguläre Ereigniswarteschlange und besitzt ein
offenes Ereignisbudget. Sie greift nicht direkt auf Synapsengewichte zu.

## B. Versuchsdesign

Der Runner `scripts/run_research_stage_a1.sh` baut das Programm, führt alle
Tests aus und schreibt drei neue CSV-Dateien:

- `research-stage-a1-parameters.csv`: vollständige Konfigurationsmatrix,
- `research-stage-a1-paths.csv`: alle 216 angeborenen Pfade,
- `research-stage-a1-results.csv`: 168 Einzelläufe mit Zustands-, Aktivitäts-,
  Ressourcen- und Zeitmessungen.

Gemeinsame Ausgangsparameter:

| Parameter | Wert |
|---|---:|
| Neuronen | 256 |
| reguläre Startsynapsen | 1.024 |
| Geometrie | 8 × 8 × 4 |
| reguläres Startgewicht | 0 |
| Feuergrenze | 3 |
| Refraktärereignisse | 16 |
| Ereignisbudget | 100.000 |
| Kontakte je Motorkanal | 1 |
| Eingabe | `AB` |
| Seeds | 20261117, 7, 42, 99, 2026, 314159 |

Netzinitialisierung, Motorkontaktzuordnung, Pfadtopologie, Exploration und
Motorschaltung verwenden voneinander getrennte Seeds beziehungsweise Salts.
Primärläufe deaktivieren Lernen und Wachstum und erlauben angeborene Gewichte
bis 3. Die gesonderte Performance-Konfiguration mit vorhandenen Regeln benutzt
Refraktärphase 16, Lernen und Wachstum sowie maximales Gewicht 1.

## C. Ergebnisse der vier Phasen

### Phase I – Grundverschaltung

| Variante | strukturell erreichbare Kontakte | Läufe mit Datenmotoraktivität | Datenereignisse | Abschlüsse |
|---|---:|---:|---:|---:|
| V0 | 9 von 9 | 0 von 6 | 0 | 0 |
| V1, Gewicht 0 | 9 von 9 | 0 von 6 | 0 | 0 |
| V2, Gewicht 1 | 9 von 9 | 0 von 6 | 0 | 0 |
| V2, Gewicht 3 | 9 von 9 | 5 von 6 | 5 | 0 |
| V3, Gewicht 3 | 9 von 9 | 5 von 6 | 5 | 0 |

Bereits die zufällige Graphstruktur V0 enthielt für alle neun Motorkontakte
irgendeinen gerichteten Pfad. Diese rein strukturelle Erreichbarkeit war wegen
Gewicht 0 nicht funktional. V1 änderte daran erwartungsgemäß nichts. Auch
Gewicht 1 blieb unter der Feuergrenze wirkungslos. Erst Gewicht 3 erzeugte
reproduzierbare Signalweiterleitung bis in Motorbereiche. V2 und V3 zeigten in
dieser Stichprobe dasselbe Aggregat; die Topologievariation allein ergab keinen
messbaren Vorteil. Keine Variante erzeugte einen Abschluss.

### Phase II – Exploration

| Variante | Explorationsereignisse je Lauf | Läufe mit Datenmotoraktivität | Datenereignisse | Abschlüsse |
|---|---:|---:|---:|---:|
| E0 | 0 | 5 von 6 | 5 | 0 |
| E1 | 1 | 5 von 6 | 5 | 0 |
| E2 | 8 | 4 von 6 | 5 | 1 |
| E3 | 8 | 5 von 6 | 7 | 0 |
| E4 | 8 | 6 von 6 | 8 | 0 |

E4 vergrößerte die beobachtete Datenaktivität gegenüber E0, erzeugte aber
keinen zuverlässigen Abschluss. Der einzelne E2-Abschluss war ein gültiges
Nullbyte und entstand durch ein tatsächliches Feuerereignis. Er ist als
zufälliger explorativer Treffer, nicht als erlernte Entscheidung zu werten.
Kein Lauf eskalierte bis zum Ereignislimit.

### Phase III – Motorische Grundschaltungen

| Variante | Läufe mit Datenmotoraktivität | Datenereignisse | vollständige Antworten | Werte |
|---|---:|---:|---:|---|
| M0 | 4 von 6 | 8 | 3 von 6 | 0x00, 0x00, 0x70 |
| M1 | 5 von 6 | 9 | 1 von 6 | 0x00 |
| M2 | 5 von 6 | 10 | 1 von 6 | 0x00 |
| M3 | 5 von 6 | 9 | 1 von 6 | 0x00 |

Die Unit-Tests weisen für M1, M2 und M3 jeweils den neuronalen Abschlussweg
nach, einschließlich einer gültigen Nullbyte-Ausgabe und der bei M3 geforderten
Mehrfachaktivität. In der Forschungsstichprobe verbesserten die Schaltungen die
Abschlussrate jedoch nicht. M0 traf den Abschlusskontakt durch allgemeine oder
explorative Aktivität häufiger. Dieser Befund warnt ausdrücklich davor, eine
hohe Abschlussrate ohne Betrachtung des Aktivitätsverlaufs als Erfolg zu
interpretieren.

### Phase IV – Eigen- und Umweltrückmeldung

| Variante | vollständige Antworten | zurückgeführte Sensorereignisse | Gesamtereignisse | Gesamtfeuerungen |
|---|---:|---:|---:|---:|
| R0 | 1 von 6 | 0 | 1.350 | 239 |
| R1 | 1 von 6 | 1 | 1.355 | 240 |
| R2 | 1 von 6 | 10 | 1.400 | 249 |
| R3 | 1 von 6 | 5 | 1.375 | 244 |

R1 gab für das abgeschlossene Nullbyte genau den separaten Marker zurück. R2
spiegelte alle zehn tatsächlich beobachteten Datenmotorereignisse. R3 codierte
für den einen abgeschlossenen Lauf vier gesetzte Bits des neuen
Umweltzustands plus Marker. Die zusätzlichen Sensorereignisse führten jeweils
zu zusätzlicher regulärer neuronaler Aktivität. Keine Variante brach ab.

Damit ist die technische Eigenrückmeldung nachgewiesen. Sie ist noch kein
Nachweis dafür, dass EVE diese Erfahrung erkennt, erwartet oder zum Lernen
nutzt.

## D. Integrations- und Kontrollversuche

| Konfiguration | vollständige Antworten | Läufe mit Datenmotoraktivität | Rückmeldeereignisse |
|---|---:|---:|---:|
| Referenz A | 0 von 6 | 0 von 6 | 0 |
| Stufe I | 0 von 6 | 5 von 6 | 0 |
| Stufe II | 3 von 6 | 4 von 6 | 0 |
| Stufe III | 1 von 6 | 5 von 6 | 0 |
| Stufe IV | 1 von 6 | 5 von 6 | 1 |
| ohne Grundverschaltung | 4 von 6 | 2 von 6 | 6 |
| ohne Exploration | 0 von 6 | 5 von 6 | 0 |
| ohne Motorschaltung | 3 von 6 | 4 von 6 | 6 |
| ohne Rückmeldung | 1 von 6 | 5 von 6 | 0 |

Die Stufen bestätigen die erwartete technische Kette, aber keine monotone
Verbesserung der Abschlussrate. Insbesondere die Kontrollen ohne
Grundverschaltung und ohne Motorschaltung zeigen zufällige Abschlüsse. Das
liegt nicht an einem technischen BIOS-Abschluss: In jedem Fall feuerte der
zugeordnete Abschlusskontakt. Es zeigt vielmehr, dass die aktuelle Exploration
und allgemeine Aktivität den Abschlusskontakt ungezielt erreichen können.

Die vollständige Kette aus gezielter lokaler Motoraktivierung, M1-Abschluss und
R1-Rückmeldung ist zusätzlich als deterministischer Integrationstest abgedeckt.
Ein Persistenztest speichert die angeborenen und motorischen Synapsen, lädt sie
wieder und vergleicht anschließend Motorantwort, Rückmeldesequenz und
Zustandsfingerabdruck.

## E. Reproduzierbarkeit, Stabilität und Ressourcen

Alle 62 Tests bestehen. Ein zweiter vollständiger Lauf erzeugte identische
Parameter- und Pfaddateien sowie identische Konfigurationen, Antworten,
Ereigniszähler und Zustandsfingerabdrücke. Nur Laufzeit- und daraus abgeleitete
Durchsatzfelder sind maschinenabhängig.

Über alle 168 Läufe wurden 25.460 Ereignisse und 20.913 synaptische Impulse
verarbeitet. Es gab keinen technischen Antwortabbruch, keinen
Rückkopplungsabbruch und keinen Fehler. Die geschätzte Belegung lag bei 35.792
Byte für V0 und typischerweise etwa 60,4 kB nach Ergänzung der angeborenen
Pfade. Der Sprung enthält die zusätzliche Vektorkapazität des bestehenden
kompakten Synapsenspeichers; es wurde keine parallele Graphstruktur eingeführt.

Die gemessenen Laufzeiten liegen pro Fall im Mikrosekundenbereich und sind in
der geteilten Umgebung kein belastbarer Benchmark. Die CSV enthält deshalb
sowohl Ereignisse als auch synaptische Ereignisse pro Sekunde, die nur für
Messungen auf derselben Maschine verglichen werden sollen. CMake war in der
Messumgebung nicht installiert; Build und Validierung erfolgten mit dem
vorhandenen Makefile. Die CMake-Zieldefinition wurde dennoch ergänzt.

## F. Grenzen der aktuellen Rückkopplung

Rückmeldeereignisse werden vollständig neuronal verarbeitet und nicht
unterdrückt. Sie laufen jedoch nach dem Ende der abgegrenzten Antwortphase.
Feuerungen von Motorkontakten während dieser Verarbeitung werden daher nicht
automatisch als neue Antwortphase beobachtet. Dies verhindert keine
neuronalen Ereignisse, bildet aber noch keinen dauerhaft autonomen rekursiven
Motorzyklus. Ein solcher Zyklus benötigt später eine explizite, budgetierte
Phasensteuerung und ist nicht Teil dieses Auftrags.

Die R3-Umwelt ist absichtlich minimal und deterministisch. Sie trennt
Motorbefehl und beobachteten Zustand technisch, stellt aber keine
Problemlösungsaufgabe dar. Der Umweltzustand wird pro Versuchskonfiguration
übergeben und ist noch kein eigenständig persistiertes Weltmodell.

## G. Schlussfolgerung und Empfehlung

Die Abschlusskriterien sind wie folgt zu beantworten:

1. Angeborene positive Pfade verbessern die funktionale Motorerreichbarkeit;
   rein strukturelle Pfade und zu kleine Gewichte nicht.
2. Begrenzte Exploration erzeugt zusätzliche und variablere Motoraktivität,
   aber noch keine zuverlässige Handlungsauswahl.
3. Motorische Grundschaltungen können technisch neuronal abschließen, erhöhen
   in der untersuchten Matrix aber nicht die Abschlusszuverlässigkeit.
4. Tatsächliche Motorereignisse werden reproduzierbar als getrennte sensorische
   Erfahrung zurückgeführt und erzeugen zusätzliche Netzaktivität.
5. Das Gesamtsystem bleibt unter den untersuchten Budgets stabil und
   reproduzierbar.

Für die nächste Entscheidung sollten V2 mit Gewicht 3, E4 und die drei
Motorschaltungen als Forschungsvarianten erhalten bleiben. Keine davon sollte
bereits als endgültige Architektur festgeschrieben werden. Vor einer
rückmeldungsabhängigen Lernregel sollte zuerst untersucht werden, wie ein
Abschluss an einen vorausgegangenen motorischen Verlauf gebunden und gegenüber
zufälliger Abschlussaktivität abgegrenzt werden kann. A.1 selbst führt keine
Bewertung, Belohnung, Ereignisspur oder neue Lernregel ein.

## Nachweise

- [Entwicklungsauftrag A.1](005-entwicklungsauftrag-forschungsstufe-a1.md)
- [Entscheidungsvorlage nach A.1](../entscheidungen/0005-vorlage-stammhirnentwicklung-nach-a1.md)
- `research/results/research-stage-a1-parameters.csv`
- `research/results/research-stage-a1-paths.csv`
- `research/results/research-stage-a1-results.csv`
