# Forschungsbericht – Forschungsstufe B.4.9

## A. Aufbau

B.4.9 führt 2.688 Entdeckungsläufe für K0–K6, 1.152 unabhängige
Bestätigungsläufe für K0, K1 und K5 sowie 2.048 Aufgabenläufe aus. Alle
zentralen Gruppen verwenden 64 gepaarte Seeds, drei vorab festgelegte
Zufallsstromfamilien und normale wie inverse Zielzuordnungen. Hinzu kommen 122
kopierte Einzelfehlerdiagnosen, Kausalablationen, Zeitkontrollen,
Alternativbindungen und 64 Persistenzproben.

E1 und E2 werden erstmals aus getrennten, vorab fingerprint-identischen
Kopien desselben vollständig trainierten Zustands ermittelt. E1 bleibt die
primäre autonome Metrik. E2 enthält ausschließlich die bekannte geschützte
Exploration nach fehlender Antwort und keine Plastizität.

## B. Referenz und korrigierte Evaluation

Die gespeicherte B.4.8-E2-Auswertung und alle Lerngewichte werden in 384/384
K0-Läufen exakt reproduziert. Die unabhängige E1-Kopie beseitigt dagegen eine
kleine temporale Beeinflussung durch vorausgegangene E2-Exploration.

| Familie | Zuordnung | K0 E1 | K0 E2 | robuste E1-Seeds |
|---:|---|---:|---:|---:|
| 0 | normal | 43,68 % | 70,07 % | 5/64 |
| 0 | invers | 49,61 % | 73,78 % | 5/64 |
| 1 | normal | 46,84 % | 71,75 % | 1/64 |
| 1 | invers | 47,59 % | 71,05 % | 5/64 |
| 2 | normal | 41,98 % | 69,92 % | 1/64 |
| 2 | invers | 39,65 % | 66,98 % | 1/64 |

Die niedrigen E1-Werte sind kein Kopierfehler: Alle 2.688 Entdeckungszeilen
beginnen mit identischen E1-/E2-Fingerabdrücken, alle Netze bleiben gültig.

## C. Kontrollgruppen K0–K6

| Familie | Zuordnung | K0 | K4 zufällig | K5 adaptiv | K6 umverteilt |
|---:|---|---:|---:|---:|---:|
| 0 | normal | 43,68 % | 52,42 % | 53,03 % | 51,08 % |
| 0 | invers | 49,61 % | 54,42 % | 50,61 % | 52,06 % |
| 1 | normal | 46,84 % | 47,96 % | 46,86 % | 53,28 % |
| 1 | invers | 47,59 % | 51,29 % | 49,81 % | 51,82 % |
| 2 | normal | 41,98 % | 50,08 % | 47,85 % | 52,95 % |
| 2 | invers | 39,65 % | 51,03 % | 39,16 % | 52,48 % |

K5 verbessert nur Familie 0 normal gegenüber K0 signifikant um 9,34 Punkte
(95-%-Bootstrapintervall 2,18 bis 17,26). Dort verbessert aber auch die
rückmeldungsunabhängige K4-Exploration um 8,74 Punkte (1,09 bis 16,28). In
Familie 2 invers ist K6 um 13,32 Punkte besser als K5 (4,47 bis 22,23).
Damit bleibt kein eigenständiger, stromübergreifender Vorteil der adaptiven
Rückmeldungsbindung.

Die Bestätigung erreicht für K5 nur 40,56–49,21 % E1 und höchstens 3/64
robuste Seeds je Familie und Zuordnung. Alle zwölf vorab definierten
Entdeckungs- und Bestätigungskriterien bleiben negativ. K7 wird nach der
Kausaldiagnose nicht begründet und K8 entfällt.

## D. Einzelne Fehlentscheidung

122 reale eindeutige Fehlantworten wurden aus Zustandskopien unmittelbar vor
der jeweiligen Trainingsentscheidung rekonstruiert. Negative Modulation
ändert im Mittel 1,754 Reizsynapsen und 0,230 Hemmsynapsen. Das Gewicht des
gewählten Pfads fällt im Mittel von 1,484 auf 0,484; auch der konkurrierende
Pfad fällt von 1,836 auf 1,230.

Beim nächsten identischen Reiz wiederholt kein Fall dieselbe eindeutige
Fehlantwort. 74/122 wechseln zur richtigen Antwort, 45/122 bleiben ohne
Ausgabe und 3/122 aktivieren beide Wege. Die bestehende Plastizität korrigiert
also die Fehlentscheidung häufig, kann aber fast 40 % der Fälle nicht in eine
eindeutige Alternative überführen.

Wird die Modulation erst nach Ablauf der Eligibility bei Zeit 16 oder 32
gegeben, bleiben 33 Fehlantworten bestehen. Das belegt die lokale
Eligibility-Wirkung. Der Abstand des erneuten Reizes von 0 bis 32 verändert
die rechtzeitige Korrektur dagegen überhaupt nicht.

## E. Konkurrenz und alternative Aktivierung

Deaktivierte Hemmung und zurückgesetzte Hemmgewichte ergeben exakt dieselben
Antworten wie die Kontrolle. Eine isolierte Reduktion der dominanten
Reizsynapse entfernt nur die drei Doppelantworten, aber nicht die 45
Ausfälle. Der unterlegene Pfad ist in 122/122 Strukturproben erreichbar.
H2 – fortdauernde inhibitorische Unterdrückung – erklärt den Hauptfehler
damit nicht.

Nach erneuter Reizaktivierung entstehen 67 positiv bewertete allgemeine
Alternativerprobungen. Sie besitzen aktive Eligibility und ändern lokale
Reiz- und Hemmgewichte. Beim nächsten gleichen Reiz sind dennoch nur 35/67
Antworten korrekt, 20/67 falsch und 12/67 ausbleibend. Nur 21/67 Zustände
erhalten zugleich auch die zweite Reizbeziehung. H4, die unzureichende
Stabilisierung erfolgreicher Alternativen, ist damit der größere verbleibende
Engpass.

Wiederholte negative Bewertung löst ihn nicht: Nach vier Wiederholungen sind
81/122 Zustände still. Die Aktivität wird erodiert, statt ein stabiler
Ersatzpfad aufgebaut zu werden.

## F. Aufgaben, Retention und Umlernen

K0-Retention entspricht für beide Zuordnungen exakt dem gemischten Endzustand.
Dasselbe gilt für K5. Alle 64 gespeicherten Kontrollzustände werden exakt
wiederhergestellt und deterministisch fortgesetzt.

Umkehrlernen bleibt unzureichend. K0 erreicht 43,89 % E1 normal und 46,43 %
invers, K5 49,97 % normal und 48,59 % invers. K5 erreicht nur 4/64
beziehungsweise 8/64 robuste Seeds. Ein technischer Reset findet nicht statt;
eine zuverlässige eigenständige Korrektur geänderter Umweltbewertung ist
nicht nachgewiesen.

## G. Antworten A bis H

1. **A – negative Rückmeldung:** Sie schwächt aktuelle lokale Pfade wirksam,
   erzeugt aber in 48/122 Fällen keine eindeutige nächste Alternative und
   schwächt zugleich konkurrierende aktive Kandidaten.
2. **B – Konkurrenz:** Erregende Gewichte tragen die Antwort. Persistente
   inhibitorische Konkurrenz und bloße Dominanz des alten Pfads sind jedoch
   nicht der Hauptgrund des verbleibenden Ausfalls.
3. **C – größter Engpass:** Die selektive Speicherung und Stabilisierung einer
   positiv bewerteten Alternative ist größer als ihre strukturelle
   Erreichbarkeit.
4. **D – bestehende Fähigkeiten:** Sie korrigieren einzelne Fehler häufig,
   aber nicht robust und nicht unter Erhalt beider Beziehungen.
5. **E – neue Fähigkeit:** Ein konkreter einzelner lokaler Zustandsengpass ist
   nicht belegt. Deshalb wird keine neue Funktion eingeführt.
6. **F – Funktionsvorteil:** Entfällt für K7. K5 zeigt weiterhin keinen
   unabhängigen stromübergreifenden Vorteil.
7. **G – Erhalt:** Persistenz und plastizitätsfreie Retention sind exakt; die
   erlernte Fähigkeit selbst bleibt unzureichend.
8. **H – Umlernen:** Nein. Normale und inverse Umkehr bleiben weit unter den
   vorab definierten E1- und Robustheitskriterien.

## H. Abschluss

B.4.9 verschiebt die Diagnose: Negative Erfahrung ist nicht wirkungslos und
die konkurrierende Handlung ist nicht verborgen. Die bestehende lokale
Plastizität beseitigt die konkrete Fehlantwort häufig, verteilt Bewertung aber
nicht selektiv genug, um eine positiv erprobte Alternative dauerhaft mit dem
Reiz zu verbinden und gleichzeitig die zweite Beziehung zu erhalten.

Ein neues lokales Aktivierungsbit wäre deshalb nicht kausal begründet. Die
allgemeine Lernfreigabe bleibt gesperrt. B.5.0 soll die lokale zeitliche
Zuordnung positiver Alternativerfahrung und den Erhalt konkurrierender
Reizbindungen untersuchen, bevor eine neue Lern- oder Zustandsfunktion
implementiert wird.

