# Forschungsbericht – Forschungsstufe B.5.1

## A. Ergebnis in einem Satz

Natürliche erfolgreiche Wiederholung ist ausreichend häufig und erzeugt in
allen untersuchten Pfaden autonomen Erwerb; die Fähigkeit wird jedoch durch
fortlaufende sensorische, inhibitorische und transiente Konkurrenz ebenso
regelmäßig wieder verloren, sodass kein stabiles Mehrfachlernen entsteht.

## B. Referenz und methodische Korrektur

Die B.5.0-Ergebnisse werden getrennt und bytegenau reproduziert. Die
ausdrückliche Prüfung der vollständigen Handlung vor jeder Modulation ändert
die historischen Wiederholungswerte nicht:

| real validierte positive Erfahrungen | autonom korrekt |
|---:|---:|
| 1 | 0/384 |
| 2 | 364/384 |
| 3 | 384/384 |
| 4 | 384/384 |

Falsche, fehlende und doppelte Ausgaben erhalten keine positive Bewertung.
E1 und E2 beginnen an jedem Checkpoint aus fingerprint-identischen Kopien.

## C. Natürlicher Erwerb

Die 768 Netze liefern 1.536 Sensorpfade und 393.216 natürliche Episoden.
Alle Pfade erleben mindestens zwei positive Zieländerungen und werden
mindestens einmal autonom korrekt. Der erste Erwerb liegt im Mittel bei
Episode 5,86 in der Entdeckung und 5,89 in der unabhängigen Bestätigung.

Die zweite positive Änderung folgt der ersten im Mittel nach rund 3,3
Episoden. Fehlende natürliche Wiederholung ist daher nicht der Engpass. Der
Schritt des Zielpfads von 1 auf 2 ist bei allen 2.828 dazu passenden
Erwerbsereignissen notwendig und hinreichend. Eine Absenkung eines bereits
wirksamen Konkurrenzpfads von 2 auf 1 kann den eindeutigen Erwerb ebenfalls
kausal herstellen. Erwerb hängt folglich vom Verhältnis beider Pfade und der
Hemmung ab, nicht nur vom absoluten Zielgewicht.

## D. Verlust und Wiedererwerb

Alle 1.536 erworbenen Pfade gehen später mindestens einmal verloren und
werden mindestens einmal wiedererworben. Über 512 Episoden entstehen 66.159
Verluste und 65.284 Wiedererwerbe. Bereits eine Episode nach dem ersten
Erwerb bleiben nur 70,18 % der Entdeckungs- und 74,35 % der
Bestätigungspfade korrekt; nach 64 Episoden sind es 43,88 % und 45,44 %.

Der erste Verlust folgt durchschnittlich nach fünf bis sechs Episoden. Frühe
Verluste sind überwiegend mit inhibitorischen Änderungen verbunden. Ihre
isolierte Rücknahme rettet jedoch nur 17,19 % beziehungsweise 22,53 % der
ersten Verlustzustände. Neutrale Modulation oder deaktivierte Plastizität
rettet 18,49 % beziehungsweise 24,48 %. Die Ursache ist damit häufig
kombiniert und nicht auf einen einzelnen Gewichtstyp beschränkt.

12.452 Verluste treten ohne jede Gewichtsänderung in der betreffenden Episode
auf. Fortlaufende Membran-, Erholungs- und Konkurrenzzustände verändern also
die momentane Abrufbarkeit zusätzlich zu synaptischer Plastizität.

## E. Zwei Beziehungen

Alle 768 Netze erreichen mindestens einmal gleichzeitig richtige autonome
Antworten für X und Y. Der Zustand entsteht im Mittel um Episode 15, bleibt
aber im Mittel nur etwa neun Episoden ununterbrochen bestehen und ist über den
Gesamtlauf nur in rund 18 % der Episoden vorhanden. Am Ende sind 140/768
Netze gleichzeitig korrekt.

Eine ausschließlich auf die gerade erworbene Beziehung konzentrierte
Fortsetzung hält sie in 100 % der Kopien verfügbar, die andere Beziehung aber
nur in 38,93 %. Eine Wiederholung mindestens alle zwei Episoden steigert die
Retention von 43,88 % auf 53,78 %, senkt jedoch den Erhalt der anderen
Beziehung von 44,40 % auf 30,34 %. Bei acht Episoden Abstand bleibt der neue
Pfad nur in 39,71 % korrekt. Wiederholung kann eine einzelne Fähigkeit
stabilisieren, verschiebt unter gemischten Aufgaben aber die Konkurrenz,
statt robustes Mehrfachlernen zu erzeugen.

## F. Hypothesen

- **H1 – fehlende natürliche Wiederholung:** widerlegt. Wiederholung tritt in
  allen Pfaden häufig auf.
- **H2 – unterbrochener Schwellenübergang:** für einzelne Episoden bestätigt,
  aber nicht als dauerhafter Erwerbsengpass. 1→2 wird häufig erreicht und
  später ebenso häufig wieder unterschritten.
- **H3 – konkurrierende positive Erfahrung:** teilweise bestätigt. Direkte
  positive Konkurrenz erklärt nur einen kleinen Anteil einzelner Verluste,
  verschiebt aber in kontrollierten Fortsetzungen die Stabilität zwischen
  beiden Beziehungen.
- **H4 – negative Erfahrung:** bestätigt. Negative Zielabschwächungen und
  weitere lokal berechtigte Sensoränderungen zerstören vorhandene Abrufe;
  sie sind jedoch nicht die einzige Ursache.
- **H5 – instabile inhibitorische Konkurrenz:** deutlich bestätigt.
  Inhibitorische Änderung ist die häufigste Klasse und dominiert den ersten
  Verlust, wirkt aber mit anderen Zuständen zusammen.
- **H6 – unzureichende Stabilisierung:** bestätigt. Erstmaliger Erwerb sagt
  wenig über Retention nach 16 bis 64 Erfahrungen aus.

## G. Antworten A bis J

1. **A:** 1.536/1.536 Pfade erreichen in 512 Episoden mindestens einmal
   autonome Wirksamkeit.
2. **B:** Zwei positive Zieländerungen treten überall auf; zwischen den ersten
   beiden liegen im Mittel 3,3 Episoden sowie negative und konkurrierende
   Erfahrungen.
3. **C:** Ziel 1→2 ist in 2.828 passenden Erwerbskopien notwendig und
   hinreichend. Konkurrenz 2→1 kann ebenfalls den eindeutigen Abruf erzeugen.
4. **D:** Ziel-, Konkurrenz- und Hemmgewichte oszillieren weiter; zusätzlich
   verändert der transiente Runtime-Zustand die Abrufbarkeit.
5. **E:** Inhibitorische Änderungen sind die größte Verlustklasse. Negative
   Sensoränderungen und Verluste ohne Gewichtsänderung sind ebenfalls
   wesentlich; konkurrierende positive Episoden wirken häufiger mittelbar.
6. **F:** Die zweite Beziehung beeinflusst Eligibility, Sensor- und
   Hemmzustände. Häufigere Wiederholung einer Beziehung verschlechtert die
   andere.
7. **G:** Ja. Alle verlorenen Pfade werden natürlich wiedererworben, jedoch
   erneut instabil.
8. **H:** Ja für momentanen Erwerb und Wiedererwerb, nein für zuverlässig
   verfügbares Mehrfachlernen.
9. **I:** Noch ist kein einzelner zusätzlicher Mechanismus isoliert. K9/K10
   bleiben deaktiviert.
10. **J:** B.5.2 muss Stabilität als lokale Konkurrenz- und Zustandsfrage
    untersuchen, nicht als bloßen Mangel an positiver Wiederholung.

## H. Abschlussentscheidung

Keiner der 36 zentralen Checkpointfälle erfüllt die allgemeinen
Erfolgskriterien. Eine neue Lernfunktion wird nicht freigegeben. Die
vorhandene Plastizität kann Fähigkeiten erzeugen, aber nicht zuverlässig
stabilisieren. Der nächste Auftrag soll zuerst kausal trennen, welcher Anteil
der instabilen Hemmung aus nichtbeitragenden Eligibility-Kandidaten stammt
und welcher Anteil durch notwendige lokale Konkurrenz beziehungsweise
transiente neuronale Zustände entsteht.

