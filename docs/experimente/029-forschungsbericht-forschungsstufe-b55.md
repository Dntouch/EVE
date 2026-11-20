# Forschungsbericht – Forschungsstufe B.5.5

## A. Ergebnis in einem Satz

Persistente unterschwellige Entscheidungsmembranen erklären bestätigt rund
73,6 % der untersuchten natürlichen Abruffehler; eine lokale feste Lebensdauer
von 16 Zeiteinheiten verbessert R2-50 reproduzierbar, halbiert Verluste und
erhält Umlernen, verfehlt aber weiterhin das 48/64-Robustheitsziel und bleibt
daher eine standardmäßig deaktivierte Forschungsfunktion.

## B. Versuchsumfang und Reproduktion

B.5.5 reproduziert B.5.3 und B.5.4 vollständig. Die neue Naturmatrix umfasst
13.440 Netze und 6.881.280 Episoden. Entdeckung und unabhängige Bestätigung
verwenden je 64 Seeds, drei unabhängige Stromfamilien und normale sowie inverse
Zuordnungen. Acht Aufgaben decken Einzelreize, beide Reihenfolgen,
alternierende und gemischte Reize, Umweltumkehr und Retention ab.

Die B.5.4-Reproduktion stimmt in allen Verhaltens-, Ereignis- und
Statistikwerten. Der geschätzte statische Speicherwert steigt durch die zwei
neu vorhandenen Neuronenfelder, obwohl die Funktion deaktiviert ist. S5 und
S2 sind im neuen Runner in allen gepaarten Ergebnissen exakt gleich.

Alle 588 Kerntests bestehen. 64/64 neue Persistenzläufe stellen ein
ausstehendes Relaxationsereignis, seine Fälligkeit und die weitere Fortsetzung
fingerabdruckgleich wieder her.

## C. Natürliche Restpotenziale und Kausalität

In 393.216 Episoden der kausalen Vorphase entstehen 251.391 natürliche
Subthreshold-Zustände, also 0,639 je Episode. 99,58 % liegen auf
Entscheidungsneuronen. Wegen der diskreten Architektur besitzen alle den Wert
1; mehrere prozentuale Relaxationsstärken wären deshalb keine verschiedenen
lokalen Wirkungen.

Von 250.339 untersuchten Entscheidungspotenzialen sind 62.172 störend, 7.417
nützlich, 169 antwortverändernd ohne Korrektur und 180.581 neutral. Unter
84.679 natürlichen Fehlern werden 73,42 % durch die isolierte Entfernung
korrigiert. Die Bestätigung liegt bei 73,57 % je Cluster
(95-%-Bootstrap-KI 73,18 bis 73,94 %). Zugleich verhindert Entfernung in
2,96 % der Entscheidungszustände eine korrekte Antwort. H1 und H2 werden damit
beide bestätigt: persistente Aktivität ist häufig schädlich, aber nicht
grundsätzlich falsch.

## D. Bedarfsentscheidung und Varianten

Erst nach diesem Kausalnachweis wurde S4 implementiert. Die Regel gibt jedem
neu entstandenen unterschwelligen lokalen Zustand eine feste Lebensdauer.
Weitere unterschwellige Eingänge erneuern sie nicht; Feuer, Hemmung auf null
und gültige bestehende Recovery machen das Ereignis ungültig. Die Regel kennt
keine richtige Handlung.

Delay 16 schützt Integration innerhalb einer Episode und begrenzt ältere
Aktivität vor der nächsten Episode. Die Kontrollen Delay 32 und 64 verbessern
S2 nicht eindeutig. Eine zusätzliche prozentuale Matrix war wegen des einzigen
möglichen Werts 1 nicht gerechtfertigt. Ein Lazy-Ansatz wurde nicht zusätzlich
implementiert: Die gewünschte Wirkung umfasst eine Relaxation auch ohne
späteren Eingang; reine Berechnung beim nächsten Eingang wäre daher nicht
semantisch gleichwertig.

## E. Natürliche Lern- und Abrufwirkung

In der unabhängigen gemischten Bestätigung erreicht S2 E1-Werte von 76,42 %
für X und 73,95 % für Y. S4-d16 erreicht 80,43 % und 82,77 %. Der gepaarte
mittlere Vorteil beträgt 6,42 Prozentpunkte mit 95-%-KI 3,31 bis 9,39 und ist
damit ein eigener positiver Beitrag gegenüber der unveränderten B.5.4-Referenz.

Die gemeinsame korrekte Episodenquote steigt von 53,93 % auf 55,92 %
(gepaart +1,99 Punkte; KI 1,54 bis 2,43). Die mittlere Verlustzahl sinkt von
142,20 auf 70,13; die gepaarte Differenz beträgt -72,07
(KI -72,84 bis -71,29). Die Verbesserung beruht daher vor allem auf deutlich
weniger Wechseln zwischen verfügbar und nicht verfügbar, nicht auf einer
technischen Sicherung früherer Gesamtzustände.

Nach 64 erfahrungsfreien Zeitabständen bleiben 80,47 % für X und 82,81 % für
Y erhalten. Geschützte Exploration bleibt in allen Netzen intakt. Feuer- und
Eligibility-Zahlen ändern sich geringfügig; S4 beeinflusst neben dem
unmittelbaren Abruf folglich auch spätere Plastizität indirekt.

## F. Umweltumkehr und beide Beziehungen

Nach Umweltumkehr erreicht S4-d16 in der Bestätigung 83,59 % für X und
82,78 % für Y; S2 erreicht 77,33 % und 72,87 %. Der gepaarte mittlere Vorteil
beträgt 8,08 Punkte (95-%-KI 5,06 bis 11,00). Exploration, Eligibility,
Gewichtsänderung und anschließende Retention bleiben wirksam. Die neue lokale
Dynamik friert bestehende Beziehungen nicht ein.

S4 verbessert in gemischten Läufen beide Beziehungen im Aggregat und erhöht
die Zahl robuster Netze von 66/384 auf 243/384. Der Befund ist jedoch nicht in
jeder vorgegebenen Zelle stark genug: Je Kombination aus Stromfamilie und
Zuordnung erreichen nur 36 bis 45 von 64 Seeds beide 75-%-Schwellen. Keine
Zelle erreicht die verlangten 48/64.

## G. Integration und Ressourcen

Die technische Matrix zeigt die unvermeidliche Gegenwirkung klar. Alle 156
Folgen, deren relevante Eingänge vor Fälligkeit liegen, behalten ihre
Integration. In den 132 Folgen mit fälliger Relaxation vor dem nächsten
Eingang wird sie unterbrochen. Delay 16 vermittelt deshalb zeitlich, erkennt
aber nicht inhaltlich, ob ein Zustand nützlich ist.

S4-d16 plant in der gemischten Bestätigung 0,671 Relaxationen je Episode,
führt 0,523 aus und verwirft 0,147 deterministisch. Die Gesamtlast steigt um
0,424 verarbeitete Ereignisse je Episode. Die maximale Queue bleibt bei 28,
der dynamisch geschätzte Speicher steigt um 40 Byte; strukturell kommen zwei
64-Bit-Felder je Neuron hinzu. Der finale Gesamtlauf benötigte 234,1 Sekunden
Wandzeit, 233,1 CPU-Sekunden und maximal 16.064 KiB RSS.

## H. Erfolgskriterien und Freigabe

S4-d16 erfüllt den gepaarten Vorteil gegenüber No-Learning, positive
Konfidenzintervalle, unabhängige Bestätigung, beide Zuordnungen, drei
Stromfamilien, Retention, Umweltumkehr, Kausalbeitrag gegenüber S2,
Determinismus und Ressourcenbegrenzung. Es verfehlt jedoch das zwingende
48/64-Kriterium in allen sechs zentralen Zellen; einzelne Zellmittel liegen
außerdem knapp unter 75 %.

Die Funktion wird deshalb nicht zum Produktionsstandard. Sie bleibt
standardmäßig deaktiviert und darf nur als explizite Forschungsvariante
verwendet werden.

## I. Antworten auf die Abschlussfragen

1. **A:** 251.391 Zustände in 393.216 Episoden, entsprechend 0,639 je Episode.
2. **B:** 62.172 waren störend, 7.417 nützlich, 169 nur verändernd und 180.581
   ohne beobachtbaren Effekt.
3. **C:** 73,42 % der natürlichen Fehler waren durch isolierte Entfernung
   korrigierbar; bestätigt 73,57 % je Cluster.
4. **D:** Integration bleibt bis unmittelbar vor der gewählten lokalen
   Lebensdauer erhalten; ab Fälligkeit wird sie deterministisch getrennt.
5. **E:** Ja, Delay 16 reduziert störende Aktivität deutlich und schützt den
   Intraepisodenhorizont, zerstört aber bewusst ältere Integration.
6. **F:** Das geplante Ereignis ist erforderlich, wenn Relaxation auch ohne
   nächsten Eingang stattfinden soll; ein reiner Lazy-Ansatz wäre nicht
   gleichwertig.
7. **G:** Hemmung kann ein ausstehendes Ereignis durch lokale Rückkehr auf
   null invalidieren und bleibt Teil der beobachteten Konkurrenz.
8. **H:** Gültige R2-Recovery und S4 besitzen getrennte Generationen; R2 kann
   S4 lokal invalidieren, ohne doppelte Relaxation.
9. **I:** Ja. Geänderte Feuerfolgen verändern Eligibility und spätere
   Gewichtsverläufe indirekt.
10. **J:** Aggregiert ja, aber noch nicht mit 48/64 Robustheit je Zelle.
11. **K:** Ja. Umlernen verbessert sich in der unabhängigen Bestätigung.
12. **L:** Der Forschungsmechanismus ist kausal gerechtfertigt, aber noch
    nicht allgemein freigabefähig.
13. **M:** Ja. Ein gültiges Ereignis je Neuron, unveränderte maximale Queue
    und 16 lokale Zustandsbytes je Neuron begrenzen den Aufwand.
14. **N:** B.5.6 soll die verbleibenden nichtrobusten S4-d16-Seeds kausal in
    synaptische Speicherung, inhibitorische Konkurrenz und transiente
    Zeitgrenzen zerlegen, bevor eine weitere Regel variiert wird.

## J. Merge-Empfehlung

**Empfehlung zum Merge von B.5.5 nach `main`: Ja.**

Die Forschung ist reproduzierbar abgeschlossen, Diagnose und Laufzeitfunktion
sind getrennt, die Abschaltkontrolle ist exakt, Persistenz und Determinismus
sind erhalten und sämtliche Tests bestehen. Die Merge-Empfehlung ist keine
Produktionsfreigabe: `subthreshold_relaxation_enabled` bleibt standardmäßig
`false`, und S4-d16 bleibt wegen der verfehlten Robustheit experimentell.

