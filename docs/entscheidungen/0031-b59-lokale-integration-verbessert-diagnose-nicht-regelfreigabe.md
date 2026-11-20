# 0031 – B.5.9: Lokale Integration verbessert Diagnose, nicht Regelfreigabe

**Status:** angenommen  
**Datum:** 20. November 2026

## Entscheidung

Die ereignisnahe Erfassung von Entscheidungsmarge sowie erregenden und
hemmenden Beiträgen wird als verhaltensneutrale Diagnostik integriert. Eine
neue Produktions-Lernregel, ein automatisches Stabilitäts-Gate und Rollbacks
früherer Gewichte werden nicht eingeführt.

## Begründung

L0 bis L4 verbessern die unabhängige Vorhersage von 49,49 % auf 53,51 %;
L4-L0 ist seed-geclustert positiv. Trotzdem bleiben auf L4 90,50 % der
Beobachtungen lokal mehrdeutig. Die Entscheidungsmarge erklärt die Feuerung,
nicht die globale Nützlichkeit einer Gewichtsänderung.

Positive Konkurrenz ist kausal real: Zehn einzelne Verstärkungen stellen bei
diagnostischer Rücknahme eine zuvor verlorene Doppelbeziehung wieder her, die
kombinierte Rücknahme wirkt in allen zwölf historischen Fällen. Dasselbe
positive Lernen bleibt jedoch für Erwerb und Umweltumkehr erforderlich.

## Folgen

- Die neuen Beobachterwerte werden nicht persistiert; Formatversion 15 bleibt.
- K21/K22 werden nicht implementiert.
- Die diagnostische Schreibsperre bleibt standardmäßig aus.
- S4 bleibt standardmäßig deaktiviert und experimentell.
- B.5.10 untersucht die lokale Vorgeschichte konkreter positiver
  Verluständerungen prospektiv am Schreibzeitpunkt.
