# Forschungsbericht – Forschungsstufe B.5.12

## Ergebnis

B.5.12 trennt neuronale Aktivität, neuronale Veränderung, Teillernen,
latentes Lernen, ersten erfolgreichen Abruf, zuverlässige Beherrschung,
Verfestigung, Retention und Wiedererwerb erstmals als eigene Lernstadien.
Die Lernlogik wurde nicht verändert. Bewertet wird ausschließlich tatsächlich
beobachtete Motoraktivität; ein geplanter Explorationsschritt ist kein Erfolg.

Das Ergebnis ist differenziert. In einer einfachen Einzelreiz-Referenz erzeugt
eine aufgabenspezifische Erfahrung vor dem ersten autonomen Erfolg einen
reproduzierbaren funktionalen Vorteil: Der erste spätere Erfolg tritt gegenüber
unerfahrenen Netzen in Discovery 1,089 und in der Bestätigung 0,859 Erfahrungen
früher auf. In 384/768 Netzen wurde die Exploration tatsächlich korrekt
ausgeführt und positiv moduliert; die übrigen erhielten keine positive
Modulation. Damit ist Teillernen vor der ersten erfolgreichen autonomen
Motorantwort für diese begrenzte Aufgabe nachgewiesen.

In der schwierigeren zeitlichen A-Aufgabe bestätigt sich dieser Vorteil nicht.
Die erfolglos vortrainierten Netze benötigen im Mittel sogar 0,205 beziehungsweise
0,184 zusätzliche Erfahrungen mehr als die unerfahrene Kontrolle; nur der
Discovery-Unterschied schließt null aus. Nach 64 gleichen Folgeerfahrungen ist
ihre bedingte A-Abrufrate nicht besser. Latentes Lernen ist daher keine allgemeine
Eigenschaft jeder frühen Gewichtsänderung.

Weitere passende A-Erfahrungen nach dem ersten Erfolg verbessern den späteren
Abruf gegenüber einer Pause bestätigt um 0,330 beziehungsweise 0,347. Bloßer
Zeitablauf verfestigt die Fähigkeit nicht. Trotzdem erreicht unter R0 kein Netz
das vorab festgelegte Kriterium von fünf korrekten Antworten in sechs Kontexten.
Der erste richtige Abruf ist damit klar vom Beherrschen der Aufgabe getrennt.

## Versuchsplan

Discovery verwendet Seeds 90001–90064, Bestätigung 100001–100064, jeweils drei
Stromfamilien und normale/inverse Zuordnung. Das ergibt 384 Primärnetze je Phase
und 768 insgesamt. Lernkurven werden nach 0, 1, 2, 4, 8, 16, 32, 64, 128, 256
und 512 Erfahrungen erfasst. Die Kontrollmatrix enthält unerfahrene, spezifisch
vortrainierte, unabhängig vortrainierte und reine Zeitkontrollen sowie
No-Learning, Frozen-Writes, R0, R2-50 und S4-d16.

Die einfache Referenz prüft, ob das Messverfahren einen realen frühen Vorteil
überhaupt erkennen kann. Die zeitliche A-Aufgabe prüft denselben Anspruch unter
der strengeren B.5.11-Aufgabenstruktur. Auswahl, Folgeerfahrungen und Auswertung
sind für die gepaarten Kontrollen identisch.

## Historische Reproduktion

Der Runner berechnet B.5.11 vollständig neu. Alle 21 deterministischen
B.5.11-Ergebnisdateien stimmen bytegenau mit dem integrierten Referenzstand
überein; die maschinenabhängige Ressourcendatei wird getrennt behandelt. Damit
bleiben insbesondere die korrigierte Explorationswertung sowie 10 produktive,
12 schädliche und 38 unentschiedene Reorganisationsfälle unverändert.

## Vollständige Lernkurve

Die Tabelle zeigt R0-Mittelwerte der bedingten A-Abrufrate. In Klammern steht
der Anteil der Netze, die bis zu diesem Kontrollpunkt mindestens einen
autonomen Erfolg hatten.

| Erfahrungen | Discovery | Bestätigung |
|---:|---:|---:|
| 0 | 0,000 (0,000) | 0,000 (0,000) |
| 1 | 0,333 (0,000) | 0,330 (0,000) |
| 2 | 0,149 (0,500) | 0,142 (0,495) |
| 4 | 0,563 (0,844) | 0,580 (0,870) |
| 8 | 0,616 (0,984) | 0,625 (0,984) |
| 16–512 | 0,618 (1,000) | 0,625 (1,000) |

Alle Netze erzielen spätestens nach 16 Erfahrungen einen ersten Erfolg, aber
kein Netz erreicht das Zuverlässigkeitskriterium. Der nichtmonotone Verlauf
zwischen Erfahrung 1 und 2 und das frühe Plateau zeigen, warum ein einzelner
Endwert oder der erste Treffer den Lernprozess unzureichend beschreibt.
No-Learning und Frozen-Writes bleiben bei 0,000. R2-50 und S4-d16 erreichen in
der Discovery jeweils 0,333 und ebenfalls kein zuverlässiges Netz; sie lösen
diese Erwerbsaufgabe nicht.

## Teillernen und latentes Lernen

| Vergleich A-vortrainiert minus unerfahren | Discovery | Bestätigung |
|---|---:|---:|
| einfache Referenz: Erfahrungen bis Erfolg | −1,089 [−1,260; −0,930] | −0,859 [−0,992; −0,732] |
| zeitliche Aufgabe: Erfahrungen bis Erfolg | +0,205 [+0,034; +0,375] | +0,184 [−0,053; +0,447] |
| zeitliche Aufgabe: finale bedingte A-Rate | −0,106 [−0,174; −0,045] | −0,053 [−0,123; +0,009] |

Die Intervalle sind seed-geclusterte 95-%-Bootstrap-Intervalle. Für die
zeitliche Aufgabe erfüllen 88 Discovery- und 76 Bestätigungsnetze je Gruppe
die vorab festgelegte Auswahl ohne vorherigen erfolgreichen Abruf. Die einfache
Referenz zeigt, dass das Verfahren nach tatsächlich korrekt ausgeführter
Exploration einen spezifischen latenten Vorteil findet, wenn er vorhanden ist.
Bei falscher Exploration entsteht keine positive Modulation. Allgemeine
Aktivität, unabhängige Erfahrung und gleiche Wartezeit erklären den Vorteil
nicht. In der komplexen Aufgabe entsteht dagegen kein solcher Vorteil; frühe
Veränderung allein genügt nicht als Lernnachweis.

## Vom ersten Abruf zur Verfestigung

| Zustand nach erstem A-Erfolg | Discovery finale A-Rate | Bestätigung finale A-Rate | zuverlässige Netze Discovery / Bestätigung |
|---|---:|---:|---:|
| 64 weitere A-Erfahrungen | 0,410 | 0,417 | 0 / 0 |
| Pause | 0,080 | 0,069 | 0 / 0 |
| unabhängige Erfahrungen | 0,080 | 0,069 | 0 / 0 |
| gemischte A/B-Erfahrungen | 0,420 | 0,423 | 112 / 110 |

Der gepaarte Vorteil weiterer A-Erfahrungen gegenüber Pause beträgt 0,330
[0,295; 0,363] in Discovery und 0,347 [0,316; 0,378] in der Bestätigung.
Passende Erfahrung verfestigt den Abruf somit relativ, führt aber nicht
automatisch zur definierten Beherrschung. Gemischte Erfahrung erzeugt bei rund
29 % der Netze zuverlässigen A-Abruf, ist aber keine reine A-Konsolidierung.

## Temporärer Verlust und Wiedererwerb

Nach A→B sind 250/384 Discovery- und 232/384 Bestätigungsnetze im A-Abruf
beeinträchtigt. Alle ausgewählten Netze erwerben A anschließend wieder. Der
Wiedererwerb benötigt jedoch 3,344 beziehungsweise 3,526 weitere Erfahrungen
und ist gegenüber naivem Ersterwerb um 0,344 [0,028; 0,660] beziehungsweise
0,440 [0,116; 0,759] langsamer. Nur 86/250 und 78/232 Einzelfälle sind schneller.
Für diese Auswahl ist daher keine allgemein fortbestehende funktionale
Vorstrukturierung nachgewiesen; Wiedererwerb ist nicht automatisch leichter.

## Mehrfachlernen und Reorganisation

| Stufe | Discovery A / B / Repertoire | Bestätigung A / B / Repertoire |
|---|---:|---:|
| nach A | 0,632 / 0,000 / 0,948 | 0,622 / 0,000 / 0,932 |
| nach A→B | 0,246 / 0,433 / 0,823 | 0,295 / 0,418 / 0,891 |
| nach A→B→A | 0,326 / 0,182 / 0,594 | 0,285 / 0,161 / 0,531 |
| nach A→B→A→gemischt | 0,427 / 0,436 / 0,849 | 0,375 / 0,422 / 0,750 |

Zusätzliche Fähigkeiten reorganisieren den funktionalen Zustand; sie werden
nicht wie unveränderliche Dateien abgelegt. Repertoire und Einzelabruf können
sich dabei gegenläufig verändern.

## Kausale Schreibgegenproben

60 vorab festgelegte Schreibereignisse nach dem ersten A-Erfolg wurden als W0,
W1 und W2 fortgesetzt. 58 erhöhen und zwei senken ein Gewicht. W2 reproduziert
W0 in 60/60 Fällen exakt. Zwei ausgewählte Änderungen sind für den späteren
A-Abruf nützlich, 58 sind in dieser Gegenprobe unentschieden; keine ist hier
einzeln schädlich. Das Unterdrücken sämtlicher weiterer Schreibungen hält den
finalen A-Abruf dagegen in allen 60 Fällen bei null. Einzelne neutrale Schritte
dürfen deshalb nicht mit entbehrlicher Plastizität insgesamt verwechselt werden.

## Antworten A–P

**A.** Ja, in der einfachen Referenz entsteht vor der ersten autonomen Antwort
eine funktional relevante Vorstrukturierung. **B.** Ja, dort lernen vortrainierte
Netze bestätigt schneller; in der zeitlichen Aufgabe nicht. **C.** Der einfache
Vorteil ist aufgabenspezifisch und wird weder durch unabhängige Aktivität noch
Wartezeit erklärt. **D.** Positive lokale Schreibungen sind erforderlich;
ein einzelner universeller kausaler Schritt wurde nicht gefunden.

**E.** Erster Abruf und Beherrschung fallen klar auseinander. **F.** Weitere
passende oder gemischte Erfahrungen erhöhen den späteren Abruf; Pause tut dies
nicht. **G.** Ja, beteiligte Synapsen werden weiter verändert. **H.** Ja,
negative Änderungen treten auf, aber eine allgemeine Zuverlässigkeitssteigerung
durch Abschwächung ist mit zwei Detailfällen nicht belegt.

**I.** Ohne neue Erfahrung ändert sich der Abruf durch vorhandene transiente und
ausstehende Ereignisse; hier liefert die Pause aber keine Verfestigung.
**J.** Die ausgewählte A→B-Verlustpopulation erwirbt A vollständig wieder, zeigt
im Mittel jedoch keinen Geschwindigkeitsvorteil gegenüber naiven Netzen.
**K.** Schnellerer Wiedererwerb tritt nur in einer Teilpopulation auf und ist
kein allgemeines Ergebnis. **L.** Zusätzliche Fähigkeiten reorganisieren A und B
stark; das funktionale Repertoire ist kontext- und reihenfolgeabhängig.

**M.** Endzustand, erster Treffer und unveränderte Gewichte übersehen wesentliche
Teile des Lernverlaufs. **N.** Kontrollpunktkurven, Erfahrungen bis zum ersten
Erfolg, kontextuelle Zuverlässigkeit, Retention, Wiedererwerb, passende Zeit- und
Aktivitätskontrollen sowie isolierte W0/W1/W2-Gegenproben werden dauerhaft
beibehalten. **O.** Die vorhandene Architektur kann frühe aufgabenspezifische
Vorstrukturierung erzeugen, integriert zeitliche und konkurrierende Fähigkeiten
aber nicht zuverlässig. **P.** B.5.13 untersucht wie ursprünglich vorgesehen
die lokale zeitliche Kreditbindung und vorzeitige Ausgabe in geteilten Folgen.

## Validierung, Grenzen und Merge-Empfehlung

786/786 Regressionstests bestehen, davon 25 neue B.5.12-Tests. Der
Persistenztest speichert einen Zustand mit einem tatsächlich ausstehenden
S4-Ereignis; Laden und Fortsetzung sind identisch. Ein unabhängiger B.5.12-Lauf
reproduziert alle fachlichen Ergebnisdateien bytegenau. Es entstehen keine
zusätzlichen persistenten Bytes je Neuron oder Synapse. S4 bleibt standardmäßig
deaktiviert, und es wurde keine neue neuronale Produktionsregel eingeführt.

Die Aufgaben bleiben klein, binär und stark konstruiert. Das Kriterium fünf von
sechs ist eine vorab festgelegte Forschungskennzahl, keine universelle Definition
von Beherrschung. Latente Kandidaten sind selektierte Teilpopulationen; deshalb
werden ausschließlich gepaarte Kontrollen innerhalb derselben Auswahl
interpretiert. Laufzeit und RSS sind maschinenabhängig.

**B.5.12 nach `main` mergen: Ja.**

Die Empfehlung umfasst Forschungsumgebung, Lernstadien, Auswertung, Rohdaten,
Tests und Dokumentation. Sie gibt weder eine neue Lernregel noch S4 als
Produktionsstandard frei. Auch die negativen Ergebnisse der komplexen Aufgabe
und des Wiedererwerbs sind wissenschaftlich relevante, bestätigte Befunde.
