# Entscheidung 0027 – lokale Subthreshold-Relaxation bleibt experimentell

## Status

Bestätigt am 20. November 2026.

## Entscheidung

Die lokale, ereignisgesteuerte Subthreshold-Relaxation wird als abschaltbare
Forschungsfunktion in die Architektur aufgenommen, bleibt aber standardmäßig
deaktiviert und wird nicht als Produktionsstandard freigegeben.

B.5.5 wird zum Merge nach `main` empfohlen. Diese Merge-Empfehlung gilt für
den reproduzierbaren Forschungsstand, die Persistenzerweiterung, Tests und
Dokumentation; sie ist ausdrücklich keine allgemeine Freigabe von S4-d16.

## Begründung

Natürliche Zustandskopien weisen einen starken kausalen Bedarf nach: 62.172
unterschwellige Entscheidungspotenziale verursachen eine später falsche
Antwort, während 7.417 für eine korrekte Antwort nützlich sind. Eine pauschale
sofortige Löschung wäre deshalb falsch.

Die feste lokale Lebensdauer von 16 Zeiteinheiten verbessert in unabhängiger
Bestätigung den mittleren E1-Wert gegenüber R2-50 um 6,42 Prozentpunkte
(95-%-KI 3,31 bis 9,39) und reduziert Verluste deutlich. Retention,
Umweltumkehr, geschützte Exploration, Determinismus und Persistenz bleiben
erhalten. Die Abschaltkontrolle ist exakt gleich R2-50.

Trotzdem erreicht keine der sechs Kombinationen aus Stromfamilie und
Zuordnung die erforderlichen 48/64 robusten Seeds. Der Mechanismus ist damit
kausal und technisch tragfähig, aber noch nicht allgemein robust genug.

## Folgen

- Der Standard bleibt `subthreshold_relaxation_enabled = false` und Delay 0.
- S4-d16 darf nur explizit in Forschungsvarianten aktiviert werden.
- Pro Neuron ist höchstens ein lokales Ereignis gleichzeitig gültig.
- Ein globaler Tick, ein globaler Reset und Zielwissen bleiben ausgeschlossen.
- B.5.6 untersucht zuerst die verbleibenden nichtrobusten Seeds kausal; es
  führt keine nachträgliche Bestwertsuche über Zeitparameter durch.

