# Forschungsbericht – Forschungsstufe B.4.5

## A. Matrix und Reproduktion

B.4.5 führt 143 Varianten mit je 64 gepaarten Seeds aus. K0, K1, N0, alle
Ambiguitätsbehandlungen und die zentralen K3-/K4-Ausprägungen durchlaufen die
vorgegebenen Einzel-, Sequenz-, Alternierungs- und Mischaufgaben. Der
K3-Faktorialversuch kombiniert Abstände 4/16/64, Eligibility-Dauern 1/4/16/64
und Feedbackverzögerungen 0/1/4. K4 prüft Kandidatenkapazitäten 1/2/4/8.

Die B.4.4-Referenzen werden exakt reproduziert: G3 gemischt 39,063 %, K1 X→Y
92,554 %, K1 Y→X 91,405 % und K1 gemischt 35,848 %. Wachstum bleibt in allen
Varianten aus. 393/393 Tests bestehen.

## B. Phase A – Entscheidungsverläufe

`fire-events.csv`, `decision-candidates.csv`, `modulation-events.csv` und
`weight-changes.csv` verbinden jede diagnostische Episode mit Feuerreihenfolge,
lokalen Kandidaten, Modulation und Gewichtsfolgen. `confusion-matrices.csv`
trennt beide Reize und alle vier Ausgabetypen.

K1 erzeugt im gemischten Training durchschnittlich 70,22 Doppelhandlungen,
die von 29,02 in der ersten auf 41,20 in der zweiten Hälfte steigen. Bei einer
Doppelhandlung hat der korrekte Pfad im Mittel Gewicht 2,94, der konkurrierende
Pfad 1,75. Die Blockade entsteht daher nach tatsächlicher Verstärkung beider
Wege, nicht durch fehlende Motorerreichbarkeit.

## C. H1 – zeitliche Interferenz

Keine von 8.192 K1-Trainingsmodulationen erreicht eine Eligibility-Spur aus
einer vorherigen Episode. K3 stellt bei keinem Abstand, keiner Dauer und keinem
Feedbackzeitpunkt gemischtes Lernen wieder her; der beste Wert bleibt 35,848 %
und entspricht K1. H1 ist in diesem Aufbau nicht der dominante Engpass.

## D. H2 – motorische Konkurrenz

| Ambiguitätsbehandlung | X | Y | ausgeglichen | robuste Seeds | Doppelhandlungen |
|---|---:|---:|---:|---:|---:|
| neutral / keine Modulation | 39,39 % | 32,31 % | 35,85 % | 0/64 | 70,22 |
| negativ 25 % | 53,55 % | 53,01 % | 53,28 % | 2/64 | 45,88 |
| negativ 50 % | 62,49 % | 68,62 % | 65,55 % | 5/64 | 35,98 |
| negativ 75 % | 73,50 % | 72,14 % | 72,82 % | 8/64 | 30,23 |
| negativ 100 % | 74,31 % | 79,33 % | 76,82 % | 9/64 | 25,48 |

100 % negative Ambiguitätsrückmeldung verbessert K1 gepaart um 40,97 Punkte,
Intervall [36,46; 45,46], und reduziert Doppelhandlungen bei 64/64 Seeds. Das
ist ein kausaler Nachweis, dass ausbleibende Korrektur konkurrierender
Motoraktivität wesentlich zum Fehler beiträgt.

Die unabhängigen Seeds bestätigen die Wirkung, aber nicht das Ziel: 81,44 %
für X, 73,88 % für Y, ausgeglichen 77,66 %, nur 14/64 robuste Seeds. Die
75-%-Variante erreicht 76,22/72,41 % und 10/64. Der Eingriff bleibt
asymmetrisch und schwächt auch richtig erworbene Pfade.

## E. H3 – lokale Eingangsbindung

K4 erreicht mit Kapazität 1 den No-Learning-Wert 30,78 %, mit Kapazität 2
34,63 % und mit 4/8 jeweils 35,85 %. `active_phase` reduziert den
Fremdreizanteil auf 6,34 %, aber die Motorwege konkurrieren weiterhin. Eine
kleinere Kandidatenmenge ist weder notwendig noch hinreichend für gemischtes
Lernen. H3 bleibt als allgemeine Frage offen, ist hier aber nicht durch eine
fehlende vorhandene Kapazität erklärt.

## F. C1/C2 und Erfolgskriterien

C1 wird mangels H1-/K4-Nachweis nicht eingeführt. H2 begründet eine künftige
C2-Untersuchung. B.4.5 implementiert keine technische Winner-take-all-Auswahl,
weil eine nach Queue-Reihenfolge erzwungene Ausgabe keine selbst erworbene
Fähigkeit wäre. Zuerst braucht es eine allgemeine lokale inhibitorische
Ereignissemantik mit Persistenz, Ablation und Biaskontrollen.

Keine Variante erfüllt alle Kriterien. K2 negativ übertrifft N0 um 46,04
Punkte mit positivem Intervall, verfehlt aber X im Entdeckungssatz, Y im
Bestätigungssatz und das Robustheitsziel 48/64 deutlich. Gemischtes
Mehrfachlernen ist nicht nachgewiesen.

## G. Stabilität und Ressourcen

Alle Gewichte bleiben in [0,3], alle Netzwerke sind gültig und keine Variante
erzeugt Synapsen. 64/64 Zustände werden exakt geladen und deterministisch
fortgesetzt. Die Matrix verarbeitet 34.816.955 Ereignisse; maschinenabhängige
Zeit, RSS, variantenspezifischer Speicher und Ereignisaufwand stehen in
`performance.csv` und `resources.csv`.

## H. Abschluss

1. Die bisherige zeitliche Eligibility-Zuordnung ist im geprüften Ablauf nicht
   ursächlich dominant.
2. Konkurrierende Motoraktivität verhindert wirksame Korrektur; H2 ist kausal
   gestützt.
3. Vorhandene rein lokale Informationen grenzen aktive Eingänge ab, reichen
   aber noch nicht zur Auflösung konkurrierender Motorentscheidungen.
4. Abstimmung vorhandener Dauer-/Kapazitätsparameter genügt nicht. Eine
   allgemeine lokale inhibitorische Konkurrenz ist ein begründeter Kandidat,
   aber noch nicht freigegeben.
5. Beide gemischten Erfahrungen werden weiterhin nicht robust unabhängig
   gelernt und erhalten.

B.4.5 ist als abgeschlossene Ursachenstudie negativ hinsichtlich des
Hauptziels, aber positiv hinsichtlich der Lokalisierung von H2.
