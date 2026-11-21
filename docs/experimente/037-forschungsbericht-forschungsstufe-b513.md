# Forschungsbericht – Forschungsstufe B.5.13

## Ergebnis

B.5.13 weist die vollständige technische Rekrutierbarkeit aller nicht
geschützten lernfähigen Neuronen nach. Unverbundene Neuronen sind direkt intern
explorierbar, ihr erstes Feuer kann Growth auslösen, und die entstehende
Nullgewicht-Synapse kann über die vorhandenen lokalen G3-, Eligibility- und
Drei-Faktor-Mechanismen Gewicht erhalten und kausal wiederverwendet werden.
Eine neue Lernregel war dafür nicht erforderlich.

Der Nachweis ist jedoch eine Architekturgrundlage, noch keine autonome
Produktionsstrategie. Growth berücksichtigt geschützte Schnittstellen nicht.
Größenabhängige Effekte in kleinen historischen Netzen treten ausschließlich in
Läufen mit Growth-Zielen innerhalb der geschützten Aufgabenschaltung auf und
ist daher kein sauberer Kapazitätsnachweis. Automatisches Growth und interne
Exploration bleiben standardmäßig deaktiviert beziehungsweise
forschungsgebunden.

## Versuchsplan

Discovery verwendet Seeds 110001–110064, die unabhängige Bestätigung
120001–120064. Jede Phase umfasst drei Stromfamilien, normale und inverse
Zuordnung sowie 32, 128, 256, 512 und 2.048 Neuronen. Das ergibt 3.840
Rekrutierungsnetze, 768 kontrollierte Funktionsnachweise und 7.680 Netze im
gepaarten Vergleich der wiederverwendeten B.5.11-/B.5.12-Aufgaben.

W0 ist der vollständige lokale Weg. W1 ablatiert nur die neue Verbindung, W2
setzt eine identische Kopie fort, W3 deaktiviert Growth, W4 interne Exploration,
W5 Gewichtsschreibungen. W6 stellt eine statische Nullgewicht-Verbindung unter
gleicher Erfahrung bereit; W7 erfasst Schutzverletzungen und die strukturelle
Basisrate. Alle Intervalle sind 95-%-Bootstrap-Intervalle.

## Rekrutierung und funktionale Entwicklung

| Nachweis | Discovery | Bestätigung |
|---|---:|---:|
| Exploration erzeugt erste Growth-Verbindung | 1.920/1.920 | 1.920/1.920 |
| neue Verbindung erreicht Eligibility und 0→1 | 384/384 | 384/384 |
| Gewicht 2 feuert Ziel kausal | 384/384 | 384/384 |
| Einzelablation entfernt Zielaktivität | 384/384 | 384/384 |
| identische W2-Kopie reproduziert | 384/384 | 384/384 |

Die Bootstrap-Raten und der gepaarte W0−W1-Ablationseffekt betragen jeweils
1,000 [1,000; 1,000]. Diese Werte gelten für den vorab definierten
Architekturbeweis mit lokaler Koinzidenz, nicht für spontanes Aufgabenlernen.

## Netzgrößen und natürliche Aufgaben

Die gepaarte Änderung des finalen A/B-Repertoires durch begrenzte interne
Exploration plus Growth gegenüber fester Topologie lautet:

| Neuronen | Discovery | Bestätigung |
|---:|---:|---:|
| 32 | −0,1328 [−0,2396; −0,0182] | −0,1146 [−0,2240; −0,0078] |
| 128 | +0,0078 [−0,0104; +0,0286] | +0,0130 [+0,0026; +0,0286] |
| 256 | −0,0104 [−0,0286; +0,0026] | 0,0000 [0,0000; 0,0000] |
| 512 | 0,0000 [0,0000; 0,0000] | 0,0000 [0,0000; 0,0000] |
| 2.048 | 0,0000 [0,0000; 0,0000] | 0,0000 [0,0000; 0,0000] |

Die ungefilterte Matrix zeigt bei 32 Neuronen einen bestätigten Schaden und bei
128 nur in der Bestätigung einen kleinen Vorteil. Diese Netze enthalten jedoch fast immer Growth-Ziele in der
geschützten Aufgabenschaltung: 384/384 und 372/384 Discovery-Netze sowie
384/384 und 374/384 Bestätigungsnetze. In allen schutzbereinigten Teilmengen
der Größen 128 bis 2.048 beträgt der gepaarte Effekt exakt 0,000; für Größe 32
existiert kein einziger sauberer Lauf. Die beobachteten Effekte belegen daher
eine Wirkung struktureller Eingriffe, aber keinen sicheren Nutzen zusätzlicher freier
Kapazität. Die natürliche Auswahl nützlicher neuer Koinzidenzen und der
fehlende Schutzfilter sind die aktuellen Grenzen.

## Schutz- und Growth-Grenze

In 221/3.840 Rekrutierungsversuchen wählte Growth ein geschütztes
Schnittstellenneuron als Ziel. Interne Exploration selbst respektiert dieselben
Reservierungen vollständig. Diese Asymmetrie verhindert die Freigabe eines
dauerhaft aktiven Produktions-Growth. Growth bildet außerdem nur erregende
Verbindungen; hemmende Integration bleibt über bestehende explizite Synapsen
möglich, wird aber nicht strukturell exploriert.

## Historische Reproduktion und Sicherheit

Der Runner reproduziert alle 15 deterministischen fachlichen B.5.12-Dateien
bytegenau; maschinenabhängige Ressourcenwerte und das verschachtelte
B.5.11-Provenienzprotokoll werden getrennt behandelt. 807/807 Regressionstests
bestehen, davon 21 neue B.5.13-Tests. Neu entstandene Synapsen, Membranstatus,
ausstehende Ereignisse und der Growth-Zufallszustand werden identisch geladen
und fortgesetzt. Es gibt keine neuen persistenten Bytes je Neuron oder Synapse,
keine neue Produktionsregel und S4 bleibt standardmäßig deaktiviert.

## Antworten A–O

**A.** Ja. In früheren festen Versuchen waren viele vorhandene Neuronen zwar
gespeichert, aber weder verbunden noch intern aktiviert. **B.** Alle nicht
geschützten Neuronen sind explorativ erreichbar. Die acht Schnittstellen sind
absichtlich reserviert; Growth selbst besitzt diese Trennung noch nicht.
**C.** Ja, technisch und exhaustiv für alle fünf Größen. **D.** Ja, als Sender
und Ziel neuer gerichteter Verbindungen.

**E.** B.4.4 fehlte die Kombination aus Zielneuronfeuer, Eligibility der neuen
Nullgewicht-Synapse und positiver Modulation. **F.** Der Engpass ist heute
kontrolliert überwindbar, tritt ohne geeignete natürliche Koinzidenz aber
weiter auf. **G.** Ja, in 768/768 kausalen Architekturbeweisen ohne Zielwissen.
**H.** Die technische Rekrutierung ist größenunabhängig; ein bestätigter
Aufgabenvorteil entsteht bei keiner Größe.

**I.** Im kontrollierten Weg werden Ressourcen funktional verwendet, in den
natürlichen Aufgaben bleiben viele neue Synapsen nur strukturell. **J.** Der
ungefilterten kleinen Effekte bei 32/128 sind vollständig mit geschützten
Growth-Zielen konfundiert; ein sicherer allgemeiner Nutzen für Teillernen,
Verfestigung oder Mehrfachlernen ist nicht belegt. **K.** Ja, neue Kanten nutzen vorhandene lokale Aktivität, G3,
Eligibility und Modulation unverändert. **L.** Die Grenze liegt in der lokalen
Auswahl nützlicher Koinzidenzen und im fehlenden Schutzfilter, nicht in der
reinen Neuronenzahl.

**M.** Exploration, Growth, Nullgewicht-Verarbeitung, Persistenz und
Drei-Faktor-Gewichtung genügen als technische Bausteine. Growth-Schutz,
hemmendes strukturelles Wachstum und eine autonome lokale Nutzenauswahl fehlen.
**N.** Benötigt werden ein lokaler Schutzfilter und ein zielwissensfreier,
ereignisgebundener Mechanismus, der interne Aktivierung mit später natürlicher
Konsequenz verbindet. **O.** Ja für eine abgegrenzte Forschungsphase C; nein
für eine Produktionsaktivierung der automatischen Mechanismen.

## Merge-Empfehlung

**B.5.13 nach `main` mergen: Ja.**

Die Empfehlung umfasst Forschungsrunner, Rohdaten, Architekturdiagnose,
Kontrollen, Tests und Dokumentation. Sie gibt weder dauerhaftes Growth noch
automatische interne Exploration, eine neue Lernregel oder S4 als
Produktionsstandard frei. Die Schutzlücke und der fehlende bestätigte
Aufgabenvorteil sind ausdrücklich Teil des Ergebnisses.
