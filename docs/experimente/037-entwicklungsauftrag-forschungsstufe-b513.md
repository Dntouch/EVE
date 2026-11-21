# Entwicklungsauftrag 037 – Forschungsstufe B.5.13

## Vollständige neuronale Rekrutierbarkeit, Growth und interne Exploration

**Branch:** `codex/research-stage-b513-neural-recruitment-growth-exploration`  
**Ausgangsbasis:** B.5.12  
**Forschungstyp:** Architekturvalidierung und experimenteller Übergang zur
Selbstorganisation  
**Neue Produktions-Lernregel:** nicht vorab freigegeben

## Verbindlicher Grundsatz

Jedes lernfähige Neuron muss grundsätzlich erreichbar, explorativ aktivierbar
und in einen funktionalen neuronalen Weg integrierbar sein. Daraus folgt weder
eine Vollverdrahtung noch die Pflicht, jede Ressource tatsächlich zu nutzen.

## Prüfprogramm

1. Vorhandene Growth-, Explorations-, G3-, Eligibility- und
   Drei-Faktor-Mechanismen technisch inventarisieren.
2. Alle Neuronen nach Rolle, Schutz, Ein-/Ausgängen und Rekrutierbarkeit
   erfassen; unbegründete Ausschlüsse nach ID, Position oder Nichtbenutzung
   ausschließen.
3. Das Henne-Ei-Problem unverbundener Neuronen durch direkte interne
   Exploration sowie ein- und ausgehendes Growth prüfen.
4. Neue Nullgewicht-Synapsen bis zu Eligibility, positiver Gewichtung,
   kausaler Nutzung und weiterem Lernen verfolgen.
5. Interne neuronale Exploration strikt von geschützter Motor-Exploration
   trennen und ohne externes Zielwissen durchführen.
6. Netze mit 32, 128, 256, 512 und 2.048 Neuronen sowie die bisherigen
   B.5.11-/B.5.12-Aufgaben untersuchen.
7. W0–W7-Kontrollen, gezielte Ablationen, Discovery mit mindestens 64 Seeds
   und eine unabhängige Bestätigung mit 64 frischen Seeds ausführen.
8. Persistenz, deterministische Fortsetzung, historische Referenzen,
   Ereignis- und Speichergrenzen prüfen.

## Mindestnachweis

- Jedes nicht geschützte lernfähige Neuron ist explorativ auswählbar.
- Unverbundene Neuronen können feuern und Growth auslösen.
- Neue Verbindungen beginnen bei Gewicht null.
- Eine neue Verbindung wird durch vorhandene lokale Mechanismen funktional
  oder ein verbleibender Engpass wird kausal lokalisiert.
- Alle bisherigen Tests bleiben erhalten; mindestens 21 neue Tests decken
  Rekrutierung, Growth, Lernen, Schutz, Persistenz und Grenzen ab.

## Abschluss

Der Bericht beantwortet die Fragen A–O des Auftrags und trennt ausdrücklich
die Merge-Freigabe der Forschungsgrundlagen von einer Produktionsaktivierung
automatischen Wachstums oder interner Exploration. S4 bleibt standardmäßig
deaktiviert; eine neue neuronale Produktionsregel ist nicht freigegeben.
