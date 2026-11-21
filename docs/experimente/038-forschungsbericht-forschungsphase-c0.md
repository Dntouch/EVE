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
liegen in der unabhängigen Bestätigung. Eine unveränderte W0-Fortsetzung mit
einer weiteren natürlichen Erfahrung entfernt jedoch alle 19 Effekte. W2,
W4 und W5 reproduzieren diesen Verlust; nur W6 mit unterdrückten
Gewichtsschreibungen erhält 19/19. Isolierte Gegenproben zeigen: In jedem Fall
ist die Abschwächung der neuen Verbindung von 2 auf 1 funktionsmindernd; 29
weitere tatsächlich ausgeführte Schreibungen verändern die spätere Funktion
isoliert nicht. Im untersuchten einfachen K3/K0-Vergleich entsteht kein
Lernvorteil.

## Umfang und Ergebnis

| Größe | Wert |
|---|---:|
| zentrale Discovery-/Bestätigungsläufe | 12.800 |
| aktuelle Kontrollläufe K0–K7 | 2.048 |
| K8 | historische B.5.13-Referenz, kein aktueller Messwert |
| Seeds je Phase und Familie | 64 |
| Netzgrößen | 32, 128, 256, 512, 2.048 |
| autonome interne Aktivierungen | 102.400 |
| geschützte Growth-Ereignisse | 166.261 |
| Schutzverletzungen | 0 |
| positive natürliche Growth-Pfade | 1.802 |
| funktionale und W1-kausale S3-Pfade | 19/19 |
| bestätigte S3-Pfade | 8 |
| nach unveränderter W0-Folgeerfahrung erhalten | 0/19 |
| konkrete W0-Gewichtsschreibungen | 48 |
| davon isoliert funktionsmindernd | 19 |
| bestandene Regressionstests | 832/832 |

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

Bestätigt, aber selten und nur unmittelbar nachgewiesen. 19 natürliche Pfade
aktivieren ihr Ziel; acht replizieren in der Bestätigung. K8 ist ausschließlich
als historische B.5.13-Referenz ausgewiesen und kein aktueller C.0-Messwert.

### G – Lernkurven

Die historischen Aufgabenfamilien zeigen Strukturzuwachs, 1.802 positive
Growth-Gewichte und 19 S3-Pfade. In der untersuchten einfachen
128-Neuronen-Kontrollfamilie der Bestätigung erzielen K3 und K0 in beiden
Zuordnungen jeweils exakt 544 korrekte Erfahrungen und Repertoiresumme 64 je
64 Seeds. Nur für diesen Vergleich ist kein Lernvorteil nachgewiesen; eine
allgemeine Aussage über andere Aufgaben oder Trainingsumfänge folgt daraus
nicht.

### H – Kausale Interventionen

W0–W6 wurden in 133 echten Kopien ausgeführt. Vor der Fortsetzung aktivieren
die 19 ausgewählten Verbindungen ihr Ziel; W1 senkt dies durch Einzelablation
auf 0/19. Die echte, interventionsfreie W0-Folgeerfahrung sowie W2, W4 und W5
enden bei 0/19, W6 bei 19/19. W0 enthält 48 konkrete negative
Gewichtsschreibungen: Die 19 Abschwächungen der ausgewählten neuen Verbindung
von 2 auf 1 sind in isolierter Schreibunterdrückung jeweils kausal für den
Funktionsverlust; 19 Schreibungen an dasselbe Ziel und zehn an andere Kanten
zeigen isoliert keinen Funktionseffekt. Gewichtsänderung wird damit nicht
pauschal als schädlich klassifiziert.

### I – Größen

Alle Größen sind sicher ausführbar. 32 Neuronen besitzen nur zwölf freie
Knoten und gelten nicht als kapazitiv äquivalent. Größere Netze schaffen mehr
freie Struktur, aber keinen Funktionsvorteil.

### J – Ressourcen

Exploration ist budgetiert, Growth lokal probabilistisch, Events bleiben
gebunden. Der gemessene Lauf benötigte rund 9,9 MiB Peak-RSS; Laufzeitwerte
sind maschinenabhängig.

### K – Persistenz

Rollenvertrag, erlaubte Richtungen, Explorationskonfiguration, Zufallszustand,
Restbudget, Aktivierungszähler, pending Events, neue Synapsen und sämtliche
älteren neuronalen Zustände werden deterministisch fortgesetzt.

### L – Historische Regression

B.5.11, B.5.12 und B.5.13 wurden mit deaktivierten C.0-Mechanismen tatsächlich
neu ausgeführt. 21, 15 und 13 deterministische Ergebnisdateien stimmen
inhaltlich exakt mit den historischen Referenzen überein. Alle 832 Tests
bestehen.

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
- **C – Lernvorteil im vorab untersuchten K3/K0-Vergleich:** nicht bestanden.

Das negative Ergebnis begrenzt die Aussage von Meilenstein 002 korrekt: EVE
kann freie Neuronen technisch und nun auch strukturell sicher rekrutieren und
selten natürlich funktional nutzen. Stabilität und messbarer Lernnutzen sind
noch nicht erreicht.
