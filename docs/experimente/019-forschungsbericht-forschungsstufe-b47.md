# Forschungsbericht – Forschungsstufe B.4.7

## A. Aufbau und Reproduktion

B.4.7 untersucht 49 vorab definierte Varianten mit 64 gepaarten
Entdeckungs-Seeds. Sechs zentrale B.4.6-Referenzen werden mit Abweichungen
unter `10^-6` reproduziert. Zentrale Gruppen werden zusätzlich mit den
unbenutzten Seeds 1001–1064 bestätigt. 439/439 Kerntests bestehen.

Die Versuche trennen:

- reine technische Rollenabbildungen,
- unveränderte und handlungskomplementierte Erfahrungsströme,
- autonome neuronale Antwort E1 und explorationsgestützte Antwort E2,
- keine, feste, lernbare und nachträglich deaktivierte Hemmung,
- normale, inverse und nach Bewertungsumkehr zu lernende Beziehungen.
- die unveränderte B.4.6-Zufallsstromkopplung und eine einzelne isolierte
  Seedentkopplung ohne Änderung des Lernmechanismus.

## B. Technische und strukturelle Symmetrie

Alle 17 reinen Abbildungsvarianten sind nach logischer Rückübersetzung in
64/64 Seeds exakt äquivalent. Insgesamt stimmen 1.088/1.088 Seed-Paare in
Ausgaben und logischen Gewichten überein. Sensor-, Motor-, Explorations- und
Modulationsnummern, Neuronen-IDs, Synapsen-IDs, Konstruktions- und
Queue-Reihenfolge erklären den B.4.6-Bias daher nicht. Die technische
Zufallsstromerzeugung ist dagegen asymmetrisch: Jeder historische Seed enthält
dieselbe 67:61-Kopplung zugunsten der normalen Zielrolle, im frühen Drittel
sogar 24:18.

Vor dem Lernen erzeugt I6 keine autonome Motorantwort. Mit geschützter
Exploration sind A und B über alle Seeds nahezu ausgeglichen erreichbar
(31,53 beziehungsweise 32,47 von 64 Evaluationen). Es gibt keine doppelten
oder fehlenden explorationsgestützten Ausgaben. Die anfänglichen Reizgewichte
sind alle 0, die beiden festen Hemmgewichte beide 1.

## C. Historische B.4.6-Reproduktion

| Messung | X | Y | ausgeglichen | robuste Seeds |
|---|---:|---:|---:|---:|
| E1 autonom | 68,76 % | 68,06 % | 68,41 % | 9/64 |
| E2 mit Exploration | 84,23 % | 83,46 % | 83,85 % | 35/64 |
| E2 Bestätigung | 81,36 % | 81,92 % | 81,64 % | 30/64 |

Exploration trägt beim Hauptkandidaten 15,44 Prozentpunkte zur gemessenen
Mischleistung bei. Die Retention ohne weitere Plastizität ist exakt gleich
der E1/E2-Auswertung des trainierten Zustands. Das Hauptkriterium von 48/64
robusten Seeds wird in Entdeckung und Bestätigung verfehlt.

Einzelreize erreichen E2 jeweils 100 %. Die beiden Blocksequenzen erreichen
95,73 % und 94,26 %. Strenge Alternierung bleibt mit 66,25 % unzureichend.

## D. Entstehung und Korrektur der Zuordnungsabhängigkeit

Die inverse Aufgabe mit unverändertem Explorationsstrom erreicht nur 59,00 %
E2 und 1/64 robuste Seeds. Wird bei identischer inverser Zielbeziehung
ausschließlich die A/B-Rolle desselben Explorationsstroms komplementiert,
entstehen exakt die normalen Ergebnisse: 83,85 % und 35/64 robuste Seeds.
Auch in der Bestätigung stimmen 81,64 % und 30/64 überein.

Damit liegt keine Nummern- oder Verschaltungsasymmetrie vor. Das System ist
jedoch stark von der konkreten Zuordnung früher explorativer Erfahrungen zu
den Handlungsrollen abhängig. Kleine stochastische Anfangsunterschiede werden
durch lokale Plastizität stabilisiert. Die finale normale Reiztrennung beträgt
im Mittel 1,86 beziehungsweise 1,78 Gewichtseinheiten; bei unveränderter
inverser Aufgabe nur 0,53 beziehungsweise 0,39.

Die komplementierte Gegenprobe ist kausal: Sie verändert weder Architektur
noch Lernregel, sondern nur, welche allgemeine Explorationshandlung in der
gespiegelten Erfahrung auftritt. Sie weist keine neue Fähigkeit nach, sondern
lokalisiert die fehlende Robustheit in der Erfahrungsbindung.

Die einzelne zulässige Phase-G-Korrektur entkoppelt ausschließlich die
RNG-Seeds. Die Kopplung variiert danach tatsächlich zwischen Seeds. Unter
dieser korrigierten Versuchsbasis ergeben sich:

| Zielzuordnung | Entdeckung E2 | robuste Seeds | Bestätigung E2 | robuste Seeds |
|---|---:|---:|---:|---:|
| normal | 70,07 % | 12/64 | 72,33 % | 16/64 |
| invers | 73,78 % | 16/64 | 68,77 % | 10/64 |

Der Entdeckungsunterschied normal minus invers beträgt -3,72 Punkte mit
Intervall `[-9,24; 1,64]`. Damit ist weder ein signifikanter Rollenunterschied
noch die vorab verlangte statistische Äquivalenz innerhalb ±5 Punkten belegt.
Vor allem zeigt die Korrektur, dass die historische 83,85-%-Leistung von einer
für die normale Zuordnung günstigen Stromkopplung profitierte.

Gegen die jeweils gepaarte korrigierte No-Learning-Gruppe bleiben Vorteile von
20,46 beziehungsweise 23,39 Punkten mit positiven Intervallen bestehen. Die
vorhandene Plastizität lernt also weiterhin, erreicht aber weder die
Einzelschwellen noch robuste Seedleistung.

## E. Eigenständiger Beitrag inhibitorischer Plastizität

| Gruppe | E1 | E2 | robuste E2-Seeds |
|---|---:|---:|---:|
| I0 keine Hemmwirkung | 79,49 % | 79,49 % | 14/64 |
| I1 feste Hemmung | 63,53 % | 81,56 % | 31/64 |
| I2 lernbare Hemmung, bestehende Regel | 68,41 % | 83,85 % | 35/64 |
| I3 beitragsbewusste Regel | 67,96 % | 79,26 % | 21/64 |
| I6 keine Plastizität | 0 % | 49,83 % | 0/64 |
| I5 trainiert, Hemmwirkung deaktiviert | 67,23 % | 70,49 % | 6/64 |

I2 übertrifft I1 um 2,29 Prozentpunkte; das 95-%-Intervall
`[-0,97; 5,28]` schließt null ein. Ein eigenständiger Vorteil lernbarer
Hemmung gegenüber fester Hemmung ist daher weiterhin nicht nachgewiesen.

Innerhalb identischer trainierter Zustände kostet die Deaktivierung der
Hemmwirkung dagegen 13,36 Punkte, Intervall `[10,42; 16,58]`. Gelernte
inhibitorische Stärke trägt funktional zum erreichten Zustand bei, ohne dass
damit ihre Überlegenheit gegenüber einer geeigneten festen Hemmung belegt ist.

Unter entkoppelten Strömen beträgt der Vorteil lernbarer gegenüber fester
Hemmung nur 1,35 Punkte normal und 1,31 Punkte invers; beide Intervalle
schließen null ein. Der negative Schluss bleibt damit auch nach Korrektur
bestehen.

Das Rücksetzen gelernter Reizpfade kostet 34,01 Punkte; ihre Vertauschung
kostet 65,68 Punkte. Die Verhaltensleistung wird somit kausal von den lokal
erworbenen Reiz-Handlungs-Pfaden getragen.

## F. Umlernen

Die normale Bewertungsumkehr erreicht E2 58,67 % und 2/64 robuste Seeds.
Doppelte Trainingsdauer verbessert auf 69,39 % und 11/64; vierfache Dauer
fällt wieder auf 66,77 % und 10/64. Die inverse Umkehr erreicht zunächst
82,06 %, bleibt aber mit 30/64 ebenfalls unter dem Robustheitsziel; längeres
Training verbessert die Robustheit nicht monoton.

Alte Reizgewichte bleiben im kontinuierlichen Zustand wirksam, während
negative Rückmeldung aktive Pfade abschwächt und Exploration konkurrierende
Pfade nur stochastisch wieder aufbaut. Mehr Episoden lösen diese
Pfadabhängigkeit nicht zuverlässig. Es findet kein technischer Reset statt.

## G. Antworten A bis H

1. **A – technische Asymmetrie:** Keine Asymmetrie durch IDs, Reihenfolge oder
   konsistente Rollenabbildung. Nachgewiesen ist eine technische binäre
   RNG-Kopplung des historischen Versuchsplans.
2. **B – Unterschiede vor dem Lernen:** Beide neuronalen Wege sind
   strukturell gleich; der historische Erfahrungsplan bevorzugt jedoch in
   jedem Seed dieselbe normale Reiz-/Explorationskopplung.
3. **C – lernbedingte Asymmetrie:** Ja. Lokale Plastizität stabilisiert frühe
   handlungsbezogene Erfahrungsunterschiede in deutlich getrennten oder kaum
   getrennten Reizgewichten.
4. **D – lernbare Hemmung:** Funktional kausal im trainierten Zustand, aber
   nicht statistisch besser als feste symmetrische Hemmung.
5. **E – vertauschte Beziehungen:** Nur bei konsistent gespiegelter
   Exploration exakt gleich. Nach RNG-Korrektur sind beide Zuordnungen ähnlich
   schwach und nicht robust.
6. **F – Exploration:** 15,44 Punkte der normalen Mischleistung; E1 allein
   erreicht nur 68,41 % und 9/64 robuste Seeds.
7. **G – Umlernen:** Alte Pfade und stochastische Alternativerreichbarkeit
   verhindern stabile Neubindung; längeres Training genügt nicht.
8. **H – bestehende Fähigkeiten:** Sie reichen für historische günstige
   Ströme, Einzelreize und Sequenzen, nicht für robuste stromunabhängige
   Mehrfach- und Umkehrentscheidung. B.4.7 führt deshalb keinen neuen
   Mechanismus frei.

## H. Abschluss

B.4.7 widerlegt eine technische Nummernpräferenz, entdeckt aber eine
technische RNG-Kopplung und bestätigt nach deren isolierter Korrektur
unzureichende adaptive Konkurrenz. Das Hauptziel bleibt mit höchstens 16/64
robusten korrigierten Seeds in Entdeckung und Bestätigung deutlich verfehlt.
Konkurrenzlernen und inhibitorische Plastizität bleiben experimentell. B.4.8
soll die lokale Stabilisierung zufälliger früher Explorationsrollen isolieren,
ohne eine richtige Handlung technisch vorzugeben.
