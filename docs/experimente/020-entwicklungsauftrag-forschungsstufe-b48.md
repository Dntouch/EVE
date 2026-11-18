# Entwicklungsauftrag 020 – Forschungsstufe B.4.8

## Ziel

B.4.8 untersucht, wann zufällige frühe Exploration dauerhafte neuronale
Präferenzen erzeugt, warum negative Rückmeldung konkurrierende Handlungen
nicht zuverlässig erschließt und weshalb gelernte Beziehungen ohne weitere
Exploration nur unzureichend autonom abrufbar sind.

## Verbindliche Untersuchungen

- exakte Reproduktion der entkoppelten B.4.7-Referenzen,
- direkte Prüfung unabhängig erzeugter Reiz-, Explorations- und Runtime-Ströme,
- mindestens drei vorab festgelegte Stromfamilien,
- vollständige Zustandskopien nach 0, 4, 8, 16, 32, 64, 96 und 128 Erfahrungen,
- E1/E2-Lernkurven und kontrollierte alternative Fortsetzungen,
- Rücknahme früher Reizgewichte, Hemmungsablation und Einfrieren gelernter Pfade,
- ereignisbezogene Analyse negativer Rückmeldung und wiederholter Fehlhandlung,
- normale, inverse, sequenzielle, alternierende, gemischte und umgekehrte Aufgaben,
- 64 Entdeckungs- und 64 unabhängige Bestätigungs-Seeds,
- erst nach der Ursachenanalyse höchstens eine minimale lokale adaptive
  Explorationsvariante mit Zufalls-, No-Learning- und Timingkontrollen.

## Grenzen

Die adaptive Variante darf weder Reizsemantik noch die richtige Handlung
kennen. Sie darf keine Gewichte zurücksetzen, keinen Zielmotor auswählen und
keine globale neuronale Aktualisierung einführen. Wachstum bleibt deaktiviert.
E1 ist die primäre Messgröße; eine reine E2-Steigerung ist kein Nachweis
autonomer Entscheidungsfähigkeit. Historische Forschungsdaten bleiben
unverändert.

## Erfolg

Normale und inverse Aufgaben müssen je mindestens 75 % E1 für X und Y sowie
48/64 robuste Seeds erreichen. Zusätzlich erforderlich sind mindestens 15
Prozentpunkte gepaarter Vorteil gegenüber No-Learning, ein vollständig
positives 95-%-Bootstrapintervall, unabhängige Bestätigung, Retention und
Reproduktion über mehrere Erfahrungsstromfamilien.
