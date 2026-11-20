# Forschungsbericht – Forschungsstufe B.5.4

## A. Ergebnis in einem Satz

Die vorhandene Recovery verbessert mit aktivem Refraktärzustand Lernen,
gemeinsamen Abruf und Umweltumkehr deutlich; die zusätzliche 50-%-Relaxation
stabilisiert besonders die gemeinsame Verfügbarkeit, erreicht aber das
48/64-Robustheitskriterium nicht und kann relevante nicht feuernde
Subthreshold-Restpotenziale grundsätzlich nicht erreichen.

## B. Versuchsumfang

B.5.4 reproduziert B.5.3 und untersucht anschließend 6.912 Netze mit insgesamt
3.538.944 natürlichen Episoden. Entdeckung und Bestätigung verwenden je 64
unabhängige Seeds, drei Stromfamilien sowie normale und inverse Zuordnungen.
Zusätzlich laufen die vollständige technische Parametermatrix, 48 zeitliche
Integrationsfolgen je Variante, kausale Verlustkopien, Umweltumkehr,
Retention, Ressourcen- und Persistenzprüfungen.

Die zentrale Bestätigung vergleicht R0, No-Learning, R1, R2-0 und R2-50. Die
Parameter wurden vor dem Lauf festgelegt; die Bestätigungsseeds 10001–10064
waren von der Entdeckung getrennt.

## C. Referenz, Refraktärwirkung und Recovery

R0 erreicht in der Bestätigung mittlere E1-Werte von 43,09 % für X und
44,11 % für Y. R1 mit Refraktärwert 1 und Delay 16 erreicht 73,96 % und
74,60 %. Der gepaarte E1-Vorteil gegenüber R0 beträgt 30,68 Prozentpunkte
(95-%-Cluster-Bootstrap-KI 27,25 bis 34,22).

Dieser Unterschied ist nicht die Wirkung eines bloß länger aktiven
Refraktärzustands: R0 mit Refraktärwert 1, aber ohne unabhängige zeitgesteuerte
Recovery, fällt in der Entdeckung auf 30,39 % und 30,97 %. R1 liegt gepaart
42,78 Prozentpunkte darüber. Die zeitige lokale Beendigung der Feuersperre ist
daher ein eigenständiger wirksamer Faktor.

R2 mit 0 % Relaxation ist in allen 384 Entdeckungs- und allen 384
Bestätigungspaaren für E1, gemeinsame Episoden und Verluste exakt gleich R1.
Damit ist die Kontrollsemantik bestätigt.

## D. Isolierter Relaxationsbeitrag

R2-50 erreicht in der unabhängigen Bestätigung 76,42 % E1 für X und 73,95 %
für Y. Gegenüber R1 beträgt die mittlere gepaarte E1-Differenz nur
+0,91 Prozentpunkte; ihr 95-%-KI von -1,31 bis +3,29 schließt null ein.

Die gemeinsame Verfügbarkeit verbessert sich dagegen klar: Der Anteil der
Episoden, in denen beide Beziehungen gleichzeitig korrekt sind, steigt von
35,81 % in R1 auf 53,93 % in R2-50. Die gepaarte Differenz beträgt
+18,12 Prozentpunkte (95-%-KI 17,79 bis 18,46). Gleichzeitig sinkt die mittlere
Zahl registrierter Verluste um 26,52 (KI -27,40 bis -25,62).

In der Entdeckung zeigen 25 % und 50 % identische Verläufe, weil die ganzzahlige
Membranrelaxation in den dort erreichten Zuständen auf dieselben Werte rundet.
100 % erhöht die gemeinsame Episodenquote weiter, zeigt aber keinen eindeutig
positiven E1-Vorteil gegenüber R1. B.5.4 leitet daraus keine nachträgliche
Bestwertfreigabe ab.

## E. Strenge Erfolgskriterien

Keine zentrale Variante erreicht die vorgegebenen 48/64 robusten Seeds in
jeder Stromfamilie und Zuordnung. R2-50 erzielt in der Bestätigung je Zelle nur
7 bis 16 robuste Seeds; R1 7 bis 14. Mehrere Zellmittel unterschreiten außerdem
für X oder Y die 75-%-Schwelle.

R2-50 ist somit ein reproduzierbarer mechanistischer Teilerfolg, aber keine
allgemein erfolgreiche Recovery-Konfiguration. Es erfolgt keine Freigabe als
Produktionsstandard und keine Implementierung von K12/K13.

## F. Lernen, Eligibility, Umweltumkehr und Retention

R1/R2 verändern über Feuerhäufigkeit und Ereignisreihenfolge auch die Zahl
lokaler Eligibility-Entscheidungen. In der Bestätigung verarbeitet R1 393.734,
R2-50 393.418 Eligibility-Entscheidungen; die Verbesserung ist daher nicht als
reine abrufseitige Membrankorrektur zu interpretieren. Recovery beeinflusst
den späteren Lernverlauf indirekt.

Geschützte Exploration bleibt in allen 6.912 Netzen erhalten. Nach
Umweltumkehr erreicht R2-50 77,33 %/72,87 % E1 gegenüber 75,22 %/69,35 % für
R1 und 44,66 %/44,36 % für R0. Gegenüber R1 beträgt der gepaarte E1-Vorteil
+2,82 Punkte (95-%-KI 0,46 bis 5,18), die gemeinsame Episodenquote steigt um
17,17 Punkte. Umlernen und anschließende Retention bleiben damit möglich.

Die Retentionswerte ohne weitere Plastizität entsprechen den E1-Werten der
jeweiligen Endzustände. Keine Variante gewinnt ihre Leistung durch eine
technische Wiederherstellung früherer Zustände oder die Kenntnis richtiger
Handlungen.

## G. Zeitliche Integration und Architekturgrenze

Alle geprüften schwachen AA-, BB-, AB-, BA-, ABA- und BAB-Folgen integrieren
über Abstände bis 64. Die vorhandene Recovery greift erst nach einem Feuer und
kann diese Prä-Feuer-Integration deshalb nicht zerstören.

Dieselbe Eigenschaft ist aber der zentrale Mangel: Ein unterschwelliges
Restpotenzial erzeugt kein Recoveryereignis und bleibt von R1/R2 unberührt.
Die vorhandene Recovery kann daher nicht alle in B.5.3 lokalisierten
Entscheidungsmembranen regulieren. Ihre positiven Naturergebnisse beweisen
eine nützliche feuergebundene Dynamik, aber keine vollständige Lösung für
transiente Abrufstörungen.

## H. Antworten auf die Abschlussfragen

1. **A:** Die historische Forschung verwendet `ResetMode::to_zero`.
2. **B:** R1/R2 werden nur nach einem Feuer, bei passender Generation und
   aktivem Refraktärzustand wirksam; Refraktärwert 0 macht das Ereignis stale.
3. **C:** Ein nicht zeitig beendeter Refraktärzustand verschlechtert die
   Leistung stark; unabhängige Recovery hebt diese Feuersperre lokal auf.
4. **D:** Ja, R2-50 reduziert Verluste und erhöht gemeinsame Abrufbarkeit;
   der bestätigte mittlere E1-Zusatznutzen gegenüber R1 ist jedoch unsicher.
5. **E:** Alle geprüften Stärken und Abstände erhalten die kontrollierte
   Prä-Feuer-Integration, weil vorher noch keine Recovery geplant ist.
6. **F:** R2-50 verbessert die gemeinsame Episodenquote reproduzierbar um
   rund 18 Punkte gegenüber R1, verfehlt aber die robuste Seed-Schwelle.
7. **G:** Der R1-Vorteil beruht nicht auf einer allgemeinen Feuersperre;
   R2-50 besitzt zusätzlich einen isolierten Relaxationsbeitrag zur
   gemeinsamen Stabilität.
8. **H:** Ja. Recovery verändert Feuer- und Eligibility-Ereignisse und damit
   indirekt auch synaptisches Lernen.
9. **I:** Geschützte Exploration bleibt in sämtlichen Netzen intakt.
10. **J:** Umlernen nach Umweltumkehr bleibt möglich und verbessert sich.
11. **K:** Nicht feuernde Subthreshold-Membranen werden grundsätzlich nicht
    erreicht.
12. **L:** Die vorhandene Recovery ist nützlich, aber wegen dieser Reichweite
    und der verfehlten Robustheit nicht ausreichend.
13. **M:** B.5.5 soll untersuchen, ob eine rein lokale, ereignisgesteuerte und
    sparsame Relaxation auch subthreshold Membranzustände erreichen kann,
    ohne einen globalen Tick oder den Verlust nützlicher Integration.

## I. Abschlussentscheidung

B.5.4 ist als kontrollierte Untersuchung abgeschlossen. R1/R2 bleiben
experimentelle Konfigurationen. Keine neue allgemeine Recoveryfunktion wird
eingeführt. B.5.5 darf den kausal eingegrenzten Reichweitenmangel untersuchen;
eine Freigabe erfordert weiterhin gepaarte Kausalität, zwei Beziehungen,
Retention, Umkehrlernen, Ressourcenbegrenzung und unabhängige Bestätigung.

