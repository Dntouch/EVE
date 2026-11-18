# Technische Analyse – frühe Erfahrungsbindung in B.4.8

## Methodische Basis

B.4.8 verwendet unverändert das 32-Neuronen-Netz von B.4.7 mit Schwelle 2,
Gewichtsgrenze 3, G3, `active_phase`-Eligibility, ternärer Modulation,
geschützter Exploration, lokaler inhibitorischer Konkurrenz und atomarer
Integration gleichzeitiger Eingänge. Wachstum und globale Handlungswahl sind
deaktiviert.

128/128 gepaarte Läufe reproduzieren die entkoppelten normalen und inversen
B.4.7-Referenzen exakt in E1, E2 und allen sechs Lerngewichten. Damit ändert
B.4.8 den Ausgangsmechanismus nicht verdeckt.

## Unabhängige Zufallsströme

Drei vorab festgelegte Stromfamilien verwenden getrennte RNG-Instanzen und
nichtlinear getrennte Schlüssel für Reiz, Exploration, Zusatzexploration und
Runtime-Initialisierung. Die Unabhängigkeit wird nicht nur aus Seeds
abgeleitet, sondern an 192 Seed-Familien-Paaren unmittelbar geprüft.

| Gegenprobe | Fenster | Übereinstimmung | Korrelation |
|---|---:|---:|---:|
| gleicher Reiz, neue Exploration | 24 | 50,02 % | 0,0006 |
| gleicher Reiz, neue Exploration | 128 | 49,74 % | -0,0054 |
| gleiche Exploration, neuer Reiz | 24 | 49,37 % | -0,0160 |
| gleiche Exploration, neuer Reiz | 128 | 50,21 % | 0,0036 |
| komplementierte Exploration | 24/128 | 0 % | -1,0000 |

Alle 192 komplementierten Folgen sind bitweise exakt gespiegelt. Frühe
Abschnitte zeigen keine systematische Kopplung.

## Zustandskopien und Kausalität

Kopien werden nach 0, 4, 8, 16, 32, 64, 96 und 128 Episoden direkt aus der
vollständigen Runtime erzeugt. Sie enthalten Netz, Membranen, G3, Eligibility,
ausstehende Ereignisse, simulierte Zeit, Metriken und Runtime-RNG. 384/384
persistierte Kontrollzustände werden exakt restauriert und deterministisch
fortgesetzt.

Von den Kopien bei 8, 32 und 64 Erfahrungen laufen zwei neue, voneinander
unabhängige zukünftige Erfahrungsfamilien weiter. Diagnostische Eingriffe bei
16 und 64 Erfahrungen werden sowohl sofort ohne Plastizität als auch nach
identischer weiterer Erfahrung ausgewertet:

- `rollback_cues` setzt ausschließlich die vier erlernten Reizgewichte zurück,
- `disable_inhibition` setzt ausschließlich die beiden Hemmgewichte auf null,
- `freeze_learned` schützt ausschließlich die vier aktuellen Reizgewichte,
- `control` bleibt eine unveränderte Kopie desselben Zustands.

Die Wiederherstellung alter Eligibility-Einträge wird nicht als isolierte
Intervention behauptet: Die öffentliche Runtime bietet dafür absichtlich
keinen technischen Schreibzugriff. Eine komplette ältere Kopie würde weitere
Zustände mitverändern und wäre daher keine gültige Eligibility-Ablation.

## Adaptive Forschungsvariante

Die einzige neue Variante besitzt genau ein flüchtiges Bereitschaftsbit.
Allgemeine negative Rückmeldung setzt es; positive Rückmeldung löscht es. Ist
es gesetzt, darf beim nächsten bereits autonomen Entscheid zusätzlich einer
der beiden geschützten Explorationswege feuern. Die A/B-Auswahl stammt
ausschließlich aus dem unabhängigen Explorationsstrom. Das Bit kennt weder
Reiz, Ziel, Motorbedeutung noch korrekte Handlung und verändert keine
Gewichte direkt.

Kontrollen sind unveränderte geschützte Exploration, plastizitätsfreie
Ausführung, rückmeldungsunabhängige Zufallsexploration und eine gepaarte
Variante mit nahezu gleicher Zusatzanzahl, aber gleichmäßig umverteilten
Zeitpunkten. In der Evaluation ist die adaptive Bereitschaft deaktiviert;
E1 misst ausschließlich den erworbenen neuronalen Zustand.

## Ressourcen und Regression

Alle Netze bleiben gültig, Gewichte bleiben in `[0,3]`, die maximale Queue
bleibt begrenzt und ein Zustand benötigt rund 24,5 KiB. Es entstehen keine
Synapsen. Die bisherigen 439 Tests und 20 neue B.4.8-Prüfungen bestehen:
459/459.
