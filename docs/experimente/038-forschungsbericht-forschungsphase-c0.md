# Forschungsbericht – Forschungsphase C.0

## Geschützte autonome neuronale Selbstorganisation

**Datum:** 21. November 2026  
**Status:** abgeschlossen; Sicherheit und seltene natürliche Funktion
bestätigt, Stabilität und Lernvorteil nicht bestätigt

## Kurzbefund

C.0 schließt die Schutzlücke aus B.5.13. Ein rollenbasierter, persistenter
Vertrag verhindert Growth in angeborene Schnittstellen und erhält freie
Rekrutierbarkeit. Autonome interne Exploration wird nun durch reale lokale
Aktivität ausgelöst und ist weder ein Runner-Befehl noch ein periodischer
globaler Scan.

Die natürliche Funktionsbildung gelingt selten: Von 166.261 neuen
Verbindungen erhielten 1.802 positives Gewicht und 19 erreichten Gewicht 2.
Alle 19 aktivierten ihr Ziel und verloren diese Wirkung unter W1; acht Fälle
liegen in der unabhängigen Bestätigung. Schon eine weitere natürliche
Erfahrung entfernt jedoch alle 19 Effekte, sofern W6 spätere Schreibungen
nicht unterdrückt. Ein Lernvorteil entsteht nicht.

## Umfang und Ergebnis

| Größe | Wert |
|---|---:|
| zentrale Discovery-/Bestätigungsläufe | 12.800 |
| Kontrollläufe K0–K8 | 2.304 |
| Seeds je Phase und Familie | 64 |
| Netzgrößen | 32, 128, 256, 512, 2.048 |
| autonome interne Aktivierungen | 102.400 |
| geschützte Growth-Ereignisse | 166.261 |
| Schutzverletzungen | 0 |
| positive natürliche Growth-Pfade | 1.802 |
| funktionale und W1-kausale S3-Pfade | 19/19 |
| bestätigte S3-Pfade | 8 |
| nach einer weiteren Erfahrung erhalten | 0/19 |
| bestandene Regressionstests | 827/827 |

## Antworten A–N

### A – Schutzarchitektur

Sechs semantische Rollen und eine explizite Richtungsmatrix ersetzen
ID-basierte Ausschlüsse. Der Vertrag klassifiziert jedes Neuron und wird bei
jeder Growth-Auswahl geprüft.

### B – Autonome Exploration

Ein lokales Feuerereignis triggert probabilistisch genau eine räumlich
begrenzte freie Aktivierung. Budget und Zufallszustand sind Teil des
persistenten Runtime-Zustands.

### C – Schnittstellenschutz

Sensorik, Motorik, Bewertung/Feedback und Stammhirn dürfen weder senden noch
empfangen. Nur freie Knoten und ausdrücklich markierte Übergangskontakte dürfen
Growth bilden. Beobachtete Verletzungen: 0.

### D – Growth

Autonom explorierte freie Neuronen erzeugen natürliche gerichtete Synapsen mit
Anfangsgewicht null. Sender, Ausgangsgrad, Empfänger, Rollen, Zustand,
Zeitpunkt und spätere Aktivität/Gewicht sind in den Rohdaten protokolliert.

### E – Rekrutierungsstufen

S0, S1 und S2 werden breit erreicht. 19 Pfade erreichen durch natürliche
Erfahrung S3 und S4 und bestehen die Einzelkausalität. S5 bleibt unerreicht,
weil kein Pfad die nächste natürliche Erfahrung übersteht oder einen
Kontrollvorteil erzeugt.

### F – Natürliche Funktionsbildung

Bestätigt, aber selten und instabil. 19 natürliche Pfade aktivieren ihr Ziel;
acht replizieren in der Bestätigung. K8 bleibt davon getrennte technische
Positivkontrolle.

### G – Lernkurven

Die historischen Aufgabenfamilien zeigen Strukturzuwachs, 1.802 positive
Growth-Gewichte und 19 S3-Pfade. K3 und K0 erzielen in beiden Zuordnungen
jedoch exakt 544 korrekte Erfahrungen und Repertoire 64 je 64 Seeds. Ein
B.5.12-Lernstufenvorteil ist nicht bestätigt.

### H – Kausale Interventionen

W0–W6 wurden in 133 echten Kopien ausgeführt. W0 aktiviert 19/19 Ziele, W1
0/19. W4 und W5 verlieren nach einer weiteren Erfahrung alle Effekte; W6
erhält 19/19. Damit sind unmittelbare Funktion und spätere Instabilität
getrennt kausal lokalisiert.

### I – Größen

Alle Größen sind sicher ausführbar. 32 Neuronen besitzen nur zwölf freie
Knoten und gelten nicht als kapazitiv äquivalent. Größere Netze schaffen mehr
freie Struktur, aber keinen Funktionsvorteil.

### J – Ressourcen

Exploration ist budgetiert, Growth lokal probabilistisch, Events bleiben
gebunden. Der gemessene Lauf benötigte rund 8,7 MiB Peak-RSS; Laufzeitwerte
sind maschinenabhängig.

### K – Persistenz

Rollenvertrag, erlaubte Richtungen, Explorationskonfiguration, Zufallszustand,
Restbudget, Aktivierungszähler, pending Events, neue Synapsen und sämtliche
älteren neuronalen Zustände werden deterministisch fortgesetzt.

### L – Historische Regression

B.5.11, B.5.12 und B.5.13 bleiben bei deaktivierten C.0-Mechanismen exakt.
Alle 827 Tests bestehen.

### M – Neue Regel

Keine. Der beobachtete Engpass rechtfertigt noch keine neue neuronale
Produktionsregel. S4 bleibt standardmäßig deaktiviert.

### N – Empfehlung

Schutzvertrag und persistierbare, ereignisgebundene Autonomie können als
Forschungsinfrastruktur gemergt werden. Die seltene natürliche S3-Bildung
rechtfertigt weitere Forschung, aber keine Produktionsaktivierung.
Automatische Exploration und Growth bleiben standardmäßig aus. Eine neue
Lernregel oder pauschaler Gewichtsschutz wird ausdrücklich nicht empfohlen.

## Erfolgskriterien

- **A – Schutz und freie Rekrutierbarkeit:** bestanden.
- **B – natürlicher funktionaler Pfad:** bestanden.
- **C – unabhängig bestätigter Lernvorteil:** nicht bestanden.

Das negative Ergebnis begrenzt die Aussage von Meilenstein 002 korrekt: EVE
kann freie Neuronen technisch und nun auch strukturell sicher rekrutieren und
selten natürlich funktional nutzen. Stabilität und messbarer Lernnutzen sind
noch nicht erreicht.
