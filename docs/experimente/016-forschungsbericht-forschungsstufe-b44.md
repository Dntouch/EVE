# Forschungsbericht – Forschungsstufe B.4.4

## A. Versuchsaufbau

B.4.4 vergleicht S0 ohne Wachstum, S1 mit dynamischem Wachstum, S2 mit
vorab angelegter statischer Zusatzkapazität, S3 mit der bestehenden
`active_phase`-Auswahl und S4 als Kombination. Jede Gruppe durchlief nur X,
nur Y, X→Y, Y→X, alternierendes und zufällig gemischtes Lernen mit 64
identisch gepaarten Seeds. N0 und N1 kontrollieren dieselben Aufgaben ohne
Plastizität. Seeds 1001–1064 bilden einen vorab festgelegten unabhängigen
Bestätigungssatz für die zentralen Sequenz- und Mischaufgaben.

Wachstum wird nur im Training zugelassen und vor der Auswertung abgeschaltet.
S2 verwendet je Aufgabe ausschließlich die zuvor gemessene mittlere
S1-Strukturzunahme, nicht deren Verhalten. Aufgaben-, Explorations-,
Feedback-, Topologie- und Wachstumszufall sind getrennt. Alle neuronalen
Entscheidungen stammen aus tatsächlichen Feuerereignissen.

## B. Erfolgreiche B.4.3-Seeds

Die erneute S0-Auswertung enthält 7/64 erfolgreiche Seeds nach X→Y und 6/64
nach Y→X. Erfolgreiche X→Y-Seeds hatten im Mittel 6,00 positive Änderungen
gegenüber 8,51, einen Fremdreizanteil von 14,67 % gegenüber 17,10 % und keine
doppelten Ausgaben gegenüber durchschnittlich 26,28. Bei Y→X betrugen die
entsprechenden Werte 6,17 gegenüber 8,43, 14,75 % gegenüber 16,96 % und 1,00
gegenüber 23,74 doppelten Ausgaben.

Die absolute Drift der beiden Gewichte der zuerst trainierten Beziehung vom
Blockwechsel bis zum Trainingsende betrug in beiden erfolgreichen Gruppen im
Mittel 1,00. Bei den erfolglosen Seeds lag sie bei 3,44 für X→Y und 3,36 für
Y→X. Die erste Beziehung blieb in den erfolgreichen Läufen also auch
synaptisch deutlich stabiler.

Die erfolgreichen Endgewichte speichern beide Beziehungen; die übrigen
Netze enden meist mit Dominanz der zuletzt trainierten Beziehung. Niedrigere
Kandidatenvermischung, weniger Änderungen und weniger doppelte Ausgaben sind
damit klare Begleitmerkmale. Die retrospektive Gruppierung beweist jedoch
keine Ursache und wird weder zur Seed-Auswahl noch technisch verwendet.

## C. Strukturvarianten im Entdeckungssatz

| Variante | X→Y X/Y | X→Y ausgeglichen | Y→X X/Y | Y→X ausgeglichen | gemischt |
|---|---:|---:|---:|---:|---:|
| S0, Referenz | 14,07/100,00 % | 57,03 % | 100,00/18,86 % | 59,43 % | 39,06 % |
| S1, Wachstum | 14,07/100,00 % | 57,03 % | 100,00/18,86 % | 59,43 % | 39,06 % |
| S2, statisch | 14,74/100,00 % | 57,37 % | 99,35/21,31 % | 60,33 % | 38,47 % |
| S3, `active_phase` | 85,11/100,00 % | 92,55 % | 100,00/82,81 % | 91,40 % | 35,85 % |
| S4, Kombination | 85,11/100,00 % | 92,55 % | 100,00/82,81 % | 91,40 % | 35,85 % |

S1 ist auf allen zentralen Aufgaben Seed für Seed verhaltensgleich mit S0.
S2 bringt in X→Y +0,34 Punkte mit Intervall [0,00; 1,02], in Y→X +0,90
Punkte [0,00; 2,59] und im gemischten Versuch −0,59 Punkte
[−1,52; 0,00]. Zusätzliche Struktur allein hilft nicht belastbar.

S3 verbessert X→Y gegenüber S0 um 35,52 Punkte mit 95-%-Intervall
[31,77; 39,05] und Y→X um 31,98 Punkte [27,78; 35,90]. Beide mittleren
Beziehungen überschreiten 75 %. 36/64 beziehungsweise 33/64 einzelne Seeds
erreichen beide Schwellen zugleich. Alternierend erreicht S3 nur 32,02 %,
gemischt 35,85 %. Die Verbesserung ist deshalb ein Sequenzbefund, kein
allgemeiner Nachweis robusten Mehrfachlernens.

Gegen die aufgabengleiche No-Learning-Kontrolle N0 beträgt der gepaarte
S3-Vorteil 61,77 Punkte für X→Y, Intervall [59,36; 63,97], und 60,62 Punkte
für Y→X, Intervall [58,39; 62,79]. Das vorab definierte 15-Punkte-Kriterium ist
damit in den Sequenzaufgaben deutlich erfüllt.

## D. Wachstum und funktionaler Beitrag

S1 erzeugt im Mittel 103,70 neue Synapsen in X→Y, 104,72 in Y→X und 111,84
im gemischten Lauf. Diese Verbindungen übertragen tausende protokollierte
Nullimpulse, aber keine erreicht ein Gewicht über 0. S4 zeigt dasselbe Muster.
Die Größe wächst, das Verhalten nicht.

S2 startet in den Sequenzaufgaben mit 127 statt 23 Synapsen und im gemischten
Lauf mit 134. Pro Seed werden im Mittel höchstens Bruchteile dieser
Verbindungen ungleich null. Die kleinen, nicht belastbaren Verhaltensänderungen
zeigen weder einen Kapazitäts- noch einen Entstehungszeitvorteil.

N1 wächst auch ohne Plastizität, bleibt jedoch verhaltensgleich zu N0. Das
trennt bloße Aktivitäts- und Strukturzunahme von Lernen.

## E. Kausale Einordnung

Für alle 256 gepaarten Sequenz-, Alternierungs- und Mischläufe ist S4 exakt
gleich S3, obwohl S4 jeweils viele zusätzliche Synapsen enthält. Sämtliche
neuen S4-Synapsen bleiben bei Gewicht 0. Eine Deaktivierung kann daher weder
einen erregenden Pfad noch das gemessene Verhalten entfernen; die
`causal-diagnosis.csv` klassifiziert alle Fälle als
`growth_functionally_inert`.

Die im Auftrag bedingte Einzelablation erfolgreicher neuer Wege ist nicht
ausgelöst: Keine Wachstumsvariante verbessert S0 beziehungsweise S3 und es
existiert kein neuer gewichteter Weg, der in einer Zustandskopie entfernt
werden könnte. Die S3-Verbesserung ohne jede neue Verbindung weist stattdessen
die lokale Rückmeldungszuordnung als wirksame Ursache aus.

## F. Unabhängige Bestätigung

Mit Seeds 1001–1064 erreicht S3 nach X→Y 81,54 % für X und 100 % für Y,
ausgeglichen 90,77 %; nach Y→X 100 % und 82,25 %, ausgeglichen 91,13 %.
30/64 beziehungsweise 31/64 Seeds überschreiten beide Einzelschwellen.
S4 ist erneut exakt gleich S3, S1/S2 bleiben nahe der Referenz, und die
gemischte S3/S4-Aufgabe erreicht nur 35,54 %. Der positive Sequenzeffekt und
die Wirkungslosigkeit des Wachstums sind damit unabhängig reproduziert.

## G. Stabilität, Persistenz und Ressourcen

Alle Gewichte bleiben innerhalb [0,8], alle Netzwerkvalidierungen bestehen,
und es gibt keine unbeschränkte Ereigniskaskade. 64/64 wachsende Zustände
werden bytegenau äquivalent geladen und nach demselben Folgeereignis
deterministisch fortgesetzt. Neue Synapsen bleiben dabei erhalten.

`resources.csv` weist Synapsenzahl, geschätzten Laufzeitspeicher,
Ereignisanzahl und maximale Queue pro Variante aus. Dynamisches Wachstum
erhöht Speicher und Ereignisverarbeitung ohne Verhaltensgewinn.
`growth-events.csv`, `weight-changes.csv`, `trials.csv` und
`seed-results.csv` enthalten die vollständigen Protokolle. 369/369 Tests
einschließlich aller B.4.2-/B.4.3-Regressionen bestehen.

## H. Abschluss

Der Befund entspricht Hypothese **A**, mit einer wichtigen Einschränkung: Das
23-Synapsen-Netz besitzt für die beiden sequenziell gelernten Beziehungen
grundsätzlich ausreichende strukturelle Kapazität; die lokale Zuordnung der
Rückmeldung ist der dominante Engpass. Das vorhandene Wachstum verbessert
weder die Referenz noch `active_phase` und erzeugt keine funktionalen Wege.

`active_phase` erfüllt die aggregierten Sequenzkriterien und reproduziert sie,
ist aber im alternierenden und gemischten Lernen weiterhin unzureichend und
nur bei etwa der Hälfte der Einzelseeds gleichzeitig robust. Daher wird weder
strukturelles Wachstum noch `active_phase` als allgemeiner Lernmechanismus
freigegeben. Die nächste Stufe soll die verbleibende lokale zeitliche Bindung
bei ineinander verschachtelten Erfahrungen untersuchen, ohne globale Reiz-
oder Handlungskennung.
