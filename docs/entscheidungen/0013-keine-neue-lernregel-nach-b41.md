# Entscheidung 0013: Keine neue Lernregel nach B.4.1

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidungen 0011 und 0012

## Kontext

B.4.1 reproduzierte B.4 bytegenau und bestätigte dessen methodische
Einschränkungen. Der korrigierte Versuch trennt Zufallsströme, Motor- und
Modulationskontakte, schreitet explizit in simulierter Zeit fort und hält
sensorische Eingaben in Explorationskontrollen aufrecht.

Eine kontrolliert verstärkte Nullgewicht-Verbindung änderte in 64 von 64
Kausalproben die spätere neuronale Handlung; die diagnostische Ablation
entfernte den Effekt. Im freien Training waren E1, E2 und E3 dennoch deutlich
schlechter als K0. Nach 512 und 2.048 Erfahrungen erreichten alle drei
Lernvarianten eine mittlere Auswertungsbelohnung von −1.

## Entscheidung

Die Drei-Faktor-Regel wird weder freigegeben noch vorschnell ersetzt. Eine neue
Lernregel ist nicht gerechtfertigt, solange die nachgewiesene Begrenzung durch
Exploration und negative Modulation nicht isoliert untersucht wurde.

Der nächste Lernversuch muss als eigene Hypothese prüfen, wie alternative
motorische Wege nach Abschwächung erreichbar bleiben. Er darf weder eine
richtige Handlung technisch auswählen noch die Gewichtsänderung aus einer
globalen Aktionskennung ableiten.

Modulationskontakte bleiben nicht-erregende neuronale Rezeptorkontakte. Sie
dürfen Membran, G3 und Motorentscheidung nicht selbst verändern.

## Folgen

- Entscheidungen 0011 und 0012 bleiben gültig.
- B.4.1 gilt als methodisch abgeschlossen, nicht als Lernerfolg.
- Ein künftiger Auftrag muss Explorationserhalt, Lernspurbreite und negative
  Modulation getrennt variieren und vorab definierte Kontrollen verwenden.
