# 0030 – B.5.8: Fähigkeitserhalt ist nicht Dauerplastizität

**Status:** angenommen
**Datum:** 20. November 2026

## Entscheidung

B.5.8 wird ohne neue Produktions-Lernregel integriert. Die experimentelle
Sperre synaptischer Gewichtsschreiboperationen bleibt standardmäßig
deaktivierte Forschungsdiagnostik. Sie darf nicht als globale Erkennung eines
gelernten Zustands verwendet werden.

## Begründung

Vorhandene Beziehungen blieben in 96 gepaarten Netzen über 512 Erfahrungen
ohne weitere Gewichtsschreibungen stabiler als unter aktiver Plastizität.
Nach Umweltumkehr lernten weiterhin gesperrte Netze jedoch nicht; nach
Reaktivierung erreichten sie wieder die Leistung durchgehend plastischer Netze.

Damit sind Fähigkeitserhalt und Lernfähigkeit getrennte Eigenschaften.
Pauschale Stabilisierung würde notwendiges Umlernen verhindern. Zugleich sind
positive und negative Änderungen lokal nicht eindeutig als nützlich oder
schädlich klassifizierbar. 20/29 erfasste lokale Signaturen besitzen gemischte
globale Folgen.

## Folgen

- Persistenzformat 15 speichert die diagnostische Schreibsperre und ihren
  Zähler.
- S4 bleibt standardmäßig deaktiviert.
- K18/K19 werden nicht implementiert.
- Negative und positive Plastizität werden nicht pauschal unterdrückt.
- Die zwölf historischen offenen Fälle bleiben offen.
- B.5.9 darf zusätzliche lokale Konkurrenzinformation untersuchen, aber keine
  globalen Erfolgslabels oder Gewichtsschutz einführen.
