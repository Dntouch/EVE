# 0028 – B.5.6 lokalisiert Restfehler auf sensorische Gewichtsverluste

## Status

Angenommen.

## Entscheidung

B.5.6 wird zum Merge nach `main` empfohlen. Es wird keine neue neuronale
Produktionsregel freigegeben. S4 bleibt experimentell und standardmäßig
deaktiviert.

B.5.7 soll die lokalen Ursachen negativer sensorischer Gewichtsänderungen nach
bereits erfolgreichem Doppelerwerb untersuchen.

## Begründung

Alle 148 frischen nichtrobusten Netze hatten beide Beziehungen erworben. In
57/72 detaillierten Fällen stellt die Rücknahme sensorischer Gewichte die
gemeinsame Fähigkeit wieder her; inhibitorische und transiente Rücknahmen tun
dies isoliert nie. Zugleich ist S4-d16 rasterabhängig und daher keine allgemein
gültige Lösung.

## Folgen

- Kein automatischer Gewichtsschutz oder Rollback.
- Keine neue Hemmungs- oder Gedächtnisregel.
- Zeitpläne müssen in künftigen Bestätigungen variieren.
- E1 und momentane Verfügbarkeit bleiben getrennte Messgrößen.
