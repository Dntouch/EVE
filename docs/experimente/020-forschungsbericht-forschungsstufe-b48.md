# Forschungsbericht – Forschungsstufe B.4.8

## A. Aufbau

B.4.8 führt 2.304 zentrale Entdeckungsläufe, 1.152 unabhängige
Bestätigungsläufe, 2.048 Aufgabenläufe und zusätzliche Checkpoint-,
Fortsetzungs-, Eingriffs- und Persistenzversuche aus. Alle zentralen Vergleiche
verwenden 64 gepaarte Seeds. Drei vorab festgelegte Zufallsstromfamilien sowie
normale und inverse Zielzuordnungen werden getrennt ausgewiesen.

Die primäre Metrik E1 zählt nur eine richtige eindeutige autonome Antwort.
E2 darf nach ausbleibender Antwort die geschützte Exploration verwenden. Eine
E2-Verbesserung wird nicht als autonomes Lernen interpretiert.

## B. Zeitpunkt der Erfahrungsbindung

| Erfahrungen | E1 normal | E1 invers | E2 normal | E2 invers |
|---:|---:|---:|---:|---:|
| 0 | 0,00 % | 0,00 % | 49,61 % | 50,39 % |
| 4 | 44,33 % | 35,61 % | 70,51 % | 67,59 % |
| 8 | 56,67 % | 55,00 % | 76,66 % | 76,22 % |
| 16 | 48,16 % | 52,03 % | 72,41 % | 75,19 % |
| 32 | 47,65 % | 45,14 % | 72,38 % | 71,59 % |
| 64 | 49,33 % | 36,19 % | 73,56 % | 65,96 % |
| 96 | 47,44 % | 47,42 % | 72,84 % | 71,83 % |
| 128 | 43,82 % | 50,14 % | 70,07 % | 73,78 % |

Eine autonome Bindung entsteht bereits nach vier Erfahrungen und erreicht im
Mittel nach acht Erfahrungen ihr Maximum. Sie stabilisiert sich aber nicht:
weitere gemischte Erfahrungen können dieselbe Fähigkeit wieder schwächen.
Frühe Bindung ist daher kein einzelner irreversibler Zeitpunkt, sondern eine
ab 4–8 Erfahrungen wirksame, fortlaufend überschreibbare Pfadpräferenz.

Alternative zukünftige Stromfamilien verändern aus identischen Kopien die
spätere E1-Leistung deutlich. Der Endzustand ist also auch nach 64 Erfahrungen
nicht vollständig festgelegt. Gleichzeitig erzeugen dieselben zukünftigen
Erfahrungen aus unterschiedlich weit trainierten Zuständen andere Ergebnisse;
vorhandene Gewichte begrenzen die weitere Entwicklung kausal.

## C. Kausale Eingriffe

Das unmittelbare Rücksetzen der vier Reizgewichte senkt E1 bei 16 und 64
Erfahrungen für beide Zuordnungen exakt auf 0 %. E2 fällt auf das jeweilige
Zufallsniveau von rund 50 %. Gelernte sensorische Verbindungen tragen damit
kausal die autonome Antwort.

Das Abschalten der Hemmung senkt unmittelbar vor allem E2: nach 64 Erfahrungen
von 73,56 auf 50,10 % normal und von 65,96 auf 37,47 % invers. Die Hemmung ist
funktional, erzeugt aber nicht selbst die Reizbindung.

Eingefrorene frühe Gewichte verschlechtern nach identischer Fortsetzung E2
gegenüber der Kontrolle deutlich. Nach dem 64er-Checkpoint erreicht die
Kontrolle 70,66 % normal und 70,79 % invers, der eingefrorene Zustand nur
65,83 % beziehungsweise 54,76 %. Weitere lokale Korrektur vorhandener Pfade
ist somit notwendig; frühe Verfestigung kann spätere Anpassung behindern.

## D. Alternativerreichbarkeit und negative Rückmeldung

Die Referenz exploriert im Mittel etwa 69–73 der 128 Trainingsfolgen, fast
ausschließlich wenn keine autonome Antwort vorliegt. Nach einer falschen
eindeutigen Antwort wird dieselbe falsche Handlung beim nächsten gleichen
Reiz noch etwa 10–12-mal pro Seed wiederholt. Eine alternative Exploration
tritt in der Referenz nur etwa 10–12-mal auf. Strukturelle Erreichbarkeit
genügt daher nicht: Eine autonome, aber falsche Ausgabe unterdrückt den
geschützten Explorationsweg funktional.

Negative Modulation verändert aktive Gewichte, garantiert aber weder eine
alternative Aktivität noch den Abruf des konkurrierenden Pfads. Das erklärt,
warum wiederholte Fehlentscheidungen und ausbleibende Antworten nebeneinander
auftreten. Das Rücksetzen der Reizpfade beseitigt E1 vollständig; spätere
Erfahrung kann sie teilweise neu aufbauen, jedoch nicht robust.

## E. Adaptive Exploration

| Familie | Zuordnung | Referenz E1 | Adaptiv E1 | Timingkontrolle E1 | robuste adaptive Seeds |
|---:|---|---:|---:|---:|---:|
| 0 | normal | 43,82 % | 51,70 % | 48,84 % | 13/64 |
| 0 | invers | 50,14 % | 48,43 % | 51,86 % | 3/64 |
| 1 | normal | 45,54 % | 43,87 % | 52,12 % | 4/64 |
| 1 | invers | 45,19 % | 47,96 % | 50,66 % | 4/64 |
| 2 | normal | 41,13 % | 45,30 % | 50,70 % | 6/64 |
| 2 | invers | 37,06 % | 37,46 % | 51,28 % | 4/64 |

Nur Familie 0 normal zeigt gegenüber der Referenz einen kleinen positiven
E1-Effekt von 7,88 Punkten mit knapp positivem Intervall. Der Vergleich gegen
rückmeldungsunabhängige Zufallsexploration schließt null ein. Die zeitlich
umverteilte Kontrolle ist in mehreren Familien gleich gut oder besser. Damit
ist kein eigenständiger kausaler Vorteil der Rückmeldungsabhängigkeit belegt.

Die Bestätigung fällt in fünf von sechs Familie-Zuordnungs-Paaren gegenüber
der jeweiligen Referenz zurück. Adaptive E1 liegt dort zwischen 37,95 % und
45,43 %. Keine Gruppe erreicht 75 % für beide Reize oder 48/64 robuste Seeds.
Alle sechs vorab definierten Erfolgskriterien bleiben negativ.

Die Variante erzeugt im Mittel rund 20–21 zusätzliche Explorationen. Sie
erhöht Alternativversuche, aber zusätzliche Aktivität allein stabilisiert die
sensorischen Pfade nicht. Eine allgemeine angeborene adaptive
Explorationsfunktion wird deshalb nicht freigegeben.

## F. Aufgaben, Retention und Umlernen

Blocksequenzen bleiben stark: Die adaptive Variante erreicht E1 93,74–94,42 %
und 42–45 robuste Seeds, verfehlt aber weiterhin 48/64. Strenge Alternierung
bleibt bei 39,76–41,34 % E1. Die Einzelreizaufgaben erreichen für den jeweils
trainierten Reiz 100 %; die ausgewiesene ausgeglichene Zweireizmetrik beträgt
wegen des absichtlich ungelernten zweiten Reizes 50 %.

Retention ohne weitere Plastizität entspricht dem gespeicherten Endzustand.
Alle 384 Persistenzproben sind exakt. Umkehrlernen erreicht adaptiv nur
48,41 % E1 normal und 46,56 % invers, mit 4/64 beziehungsweise 8/64 robusten
Seeds. Ein technischer Reset findet nicht statt. Zuverlässiges Umlernen ist
nicht nachgewiesen.

## G. Antworten A bis H

1. **A – Zeitpunkt:** Wirksame Pfadpräferenzen entstehen nach 4–8 Erfahrungen,
   bleiben aber überschreibbar und werden nicht monoton stabiler.
2. **B – Alternativen:** Beide Wege bleiben strukturell vorhanden, werden nach
   einer eindeutigen autonomen Antwort jedoch funktional zu selten exploriert.
3. **C – negative Rückmeldung:** Sie schwächt aktive Pfade, reicht aber nicht,
   um zuverlässig eine alternative Entscheidung hervorzurufen.
4. **D – autonome Aktivierung:** Sensorische Gewichte tragen E1 kausal, bleiben
   aber zwischen konkurrierenden Erfahrungen zu schwach oder einseitig.
5. **E – adaptive Exploration:** Die minimale Variante erhöht Versuche, zeigt
   aber keinen stromübergreifenden eigenständigen Lernvorteil.
6. **F – technische Unabhängigkeit:** Ja. Drei direkt geprüfte Stromfamilien,
   Komplementkontrollen und normale/inverse Aufgaben verhindern eine günstige
   Einzelkopplung als Hauptaussage.
7. **G – Erhalt:** Gespeicherte Fähigkeiten bleiben ohne Plastizität exakt
   erhalten; der erworbene Zustand selbst ist jedoch unzureichend robust.
8. **H – Umlernen:** Nein. Bewertungsumkehr bleibt deutlich unter den
   autonomen Kriterien.

## H. Abschluss

B.4.8 bestätigt H1 bis H4 teilweise und gemeinsam: Frühe Erfahrungen binden
schnell, autonome Fehlantworten unterdrücken Alternativerprobung, negative
Modulation erzeugt keine Alternative und sensorische Pfade werden nicht robust
genug autonom aktiviert. Die untersuchte adaptive Exploration löst diese
Kombination nicht.

Das Ergebnis ist negativ, aber kausal eingegrenzt. Für B.4.9 wird kein breiter
neuer Mechanismus freigegeben. Eine Folgestufe sollte zuerst die lokale
Konkurrenz zwischen bereits aktiven sensorischen Pfaden und der Auslösung
einer alternativen Erfahrung trennen, ohne Explorationsmenge als Lernersatz zu
verwenden.
