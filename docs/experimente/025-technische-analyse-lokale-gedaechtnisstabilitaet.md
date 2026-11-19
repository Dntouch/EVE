# Technische Analyse – lokale Gedächtnisstabilität in B.5.2

## Methodik und Reproduktion

B.5.2 bettet den unveränderten B.5.1-Runner ein und schreibt dessen Ergebnisse
nach `research/results/b52/b51-reproduction`. Die Primärmatrix umfasst 768
Netze, 512 Episoden je Netz, drei Stromfamilien, normale und inverse Zuordnung
sowie getrennte Entdeckungs- und Bestätigungs-Seeds. Die 393.216 Episoden
verwenden weiterhin 32 Neuronen, Feuergrenze 2, Gewichtsgrenze 3, G3,
`active_phase`, ternäre Modulation, geschützte Exploration und lernbare
Hemmung.

Der Runner protokolliert alle 66.159 Verlustereignisse. Für sämtliche 12.452
Verluste ohne Gewichtsänderung sowie den ersten und zweiten Verlust jedes
Pfades werden vollständige Kausalzustände erzeugt. Das ergibt 15.194
detaillierte Verlustkopien und 121.552 faktorielle Auswertungen.

## Speicherung gegen Abruf

Die S×I×T-Matrix liefert:

| Zustand | korrekter Abruf | Anteil |
|---|---:|---:|
| V0: S alt, I alt, T alt | 15.194/15.194 | 100,00 % |
| V1: S neu, I alt, T alt | 15.006/15.194 | 98,76 % |
| V2: S alt, I neu, T alt | 14.919/15.194 | 98,19 % |
| V3: S alt, I alt, T neu | 558/15.194 | 3,67 % |
| V4: S neu, I neu, T alt | 14.731/15.194 | 96,95 % |
| V5: S neu, I alt, T neu | 531/15.194 | 3,49 % |
| V6: S alt, I neu, T neu | 116/15.194 | 0,76 % |
| V7: S neu, I neu, T neu | 0/15.194 | 0,00 % |

14.731/15.194 ausgewählte Verlustantworten werden allein durch den früheren
vollständigen transienten Zustand bei den neuen Gewichten wiederhergestellt.
Nur 463 Fälle bleiben unter altem T durch synaptische Änderungen verloren.
296 Verluste entstehen als echte Faktorinteraktion: Kein einzelner neuer
Faktor genügt, eine Kombination aber schon.

Alle 12.452 Verluste ohne Gewichtsänderung besitzen identische Gewichte und
Struktur. Über alle Detailfälle unterscheiden sich Eingangs- und lokale
Ereignisspeicherzustände in 15.194, Membranzustände in 15.079 und
Synapsenaktivitätszustände in 15.186 Fällen. Refraktär-, Recovery-, Context-
und Queue-Differenzen sind in dieser Architektur null. Einzelne private
Queueelemente können über die öffentliche API nicht sicher transplantiert
werden; die vollständige alte beziehungsweise neue Runtime bildet deshalb
den gültigen T-Faktor.

## Probewirkung und Zeit

Wiederholte Prüfung aus fingerprint-identischen Kopien liefert in
15.194/15.194 Fällen identische Antworten und Folgezustände. Zwei Proben auf
derselben fortgesetzten Runtime stimmen nur in 267/15.194 Fällen überein. Die
Probe selbst verändert somit den transienten Zustand wesentlich.

Ein ereignisfreier Abstand von 0, 1, 4, 8, 16, 32 oder 64 simulierten
Zeiteinheiten stellt keinen der 15.194 post-loss-Zustände automatisch wieder
her. Der beobachtete Abruf ist zustands- und eingangsabhängig, nicht bloß eine
Funktion verstrichener Zeit.

## Inhibitorische Eligibility

149.301 inhibitorische Gewichtsänderungen besitzen zeitlich gültige
Eligibility. 100.434 dazugehörige Synapsen waren in der bewerteten Episode
funktional aktiv; 48.867 waren zwar zeitlich berechtigt, aber weder aktiv noch
für die autonome Ausgabe durch Einzelablation nachweisbar. Damit ist H2 als
häufiger Kandidatenfehler belegt, jedoch nicht als alleinige Verlustursache.

Eine Ablation verändert die autonome Ausgabe in 96.121 Fällen, die isolierte
Übernahme einer Änderung in 10.593. Positive und negative Schritte sind mit
76.436 beziehungsweise 72.865 fast ausgeglichen. Bei der ersten Änderung je
Netz ergeben 16 natürliche Folgeerfahrungen 157/768 gleichzeitig korrekte
Netze, Rollback 159/768 und isolierte Übernahme 178/768. Die Änderung kann
später wirken, doch eine pauschale Rücknahme ist keine robuste Lösung.

## Sensorische Überlappung und gemeinsame Stabilität

In 80.588/393.216 Episoden existieren Kandidaten des anderen Reizes, und in
allen diesen Episoden teilen aktueller und anderer Reiz mindestens einen
Empfänger. H3 ist damit strukturell und ereignisbezogen bestätigt.

Alle 768 Netze erreichen mindestens einmal beide richtigen Beziehungen. Vom
ersten gemeinsamen Erfolg bleiben nach 1, 4, 8, 16, 32 und 64 weiteren
Erfahrungen 531, 322, 203, 147, 147 und 141 Netze gleichzeitig korrekt. Von
578 Zuständen mit mindestens acht stabilen Episoden sind nach 64 weiteren
Episoden noch 91 von 545 nicht zensierten Kopien gleichzeitig korrekt.

## Kontrollen, Aufgaben, Statistik und Persistenz

K0 reproduziert die B.5.1-Referenz. No-Learning erreicht E1 0, feste Hemmung
bleibt unter der Referenz, und Stoppen der Plastizität nach Episode 128 löst
die Konkurrenz nicht. Die vollständige Aufgabenmatrix zeigt korrekte
Einzelreizleistung, aber schwache alternierende, gemischte, inverse,
Umkehr- und Retentionsleistung. Keine der zwölf zentralen Kombinationen aus
Phase, Familie und Zuordnung erfüllt die allgemeinen Erfolgskriterien;
höchstens 8/64 Entdeckungs- und 3/64 Bestätigungs-Seeds sind robust.

64/64 gespeicherte Zustände werden mit identischem Fingerprint, vollständigem
transientem Zustand und identischer autonomer Antwort wiederhergestellt.
Historische Ordner bleiben unverändert. Die komprimierten Rohdaten enthalten
Verlustereignisse, Faktorablationen, Zustandsdifferenzen, Probeversuche,
Eligibility, sensorische Überlappung und inhibitorische Fortsetzungen.

K9 und K10 werden nicht implementiert. B.5.2 isoliert einen dominanten
transienten Abrufanteil, aber noch keine einzelne lokale Produktionsregel,
die gleichzeitig Stabilität und spätere Veränderbarkeit gewährleistet.
