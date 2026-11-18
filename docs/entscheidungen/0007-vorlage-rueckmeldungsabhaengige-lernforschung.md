# 0007 – Übergang zu rückmeldungsabhängiger Lernforschung

**Status:** Entwurf – gemeinsame Entscheidung ausstehend

**Datum:** 2026-11-18

**Verantwortung:** offen

## Kontext

Forschungsstufe B hat gezeigt, dass ein lokaler Ringspeicher relevante
Eingangssynapsen einer M3-Handlung festhalten kann. G2 bis G4 rekonstruierten
alle 14 technisch nachgewiesenen Handlungswege der 64-Seed-Stichprobe. G3
behielt nach zusätzlichen Eingängen und Eigenrückmeldung in allen 14 Fällen
mindestens eine relevante Akkumulator-Synapsen-ID.

Der Befund ist noch keine Grundlage für unmittelbare Gewichtsänderung:

- Der Ring enthält auch zeitnahe, aber nicht beitragende Nullgewicht-Eingänge.
- Eine Synapsen-ID unterscheidet alte und neue Ereignisinstanzen nicht.
- Einzelunterbrechungen stoppten 10 von 14, aber nicht alle Handlungen.
- Bei fortgeführtem Zustand bleiben wiederholte Handlungen stark instabil.
- Eine Rückmeldung besitzt noch keinen neuronalen Weg zu genau den beteiligten
  lokalen Zuständen.

## Entscheidungskriterien

- Lernen darf nur auf realen neuronalen Zuständen beruhen, nicht auf der
  technischen Kausalitätsdiagnose.
- Erfolg und Misserfolg müssen mit identischen Ausgangszuständen kontrolliert
  werden.
- Nicht beitragende und mehrdeutige Eingänge dürfen nicht stillschweigend als
  kausal behandelt werden.
- Refraktärregel, bestehende Lernregeln und M3-Topologie bleiben getrennte
  Versuchsvariablen.
- Jede automatische Gewichtsänderung benötigt Abschaltkontrollen und harte
  Gewichtsgrenzen.

## Optionen

### Option A – Direkte Rückmeldung auf alle Ringeinträge

Nach einer Rückmeldung werden alle noch sichtbaren Synapsen des lokalen Rings
verstärkt oder abgeschwächt.

Diese Lösung ist einfach, vermischt aber beteiligte, inaktive und erst später
eingetroffene Ereignisse. Die B-Daten sprechen gegen diese Option.

### Option B – Begrenzte lokale Eligibility-Markierung am Feuerentscheid

Ein Neuron erzeugt beim eigenen Feuern aus seinem Ring eine kurzlebige lokale
Kandidatenmarkierung. Rückmeldung darf ausschließlich diese neuronale
Markierung adressieren. Kapazität, Lebensdauer, Beitragsregel und
Gewichtsänderung werden zunächst getrennt untersucht.

Diese Option nutzt das positive G3-Ergebnis, benötigt aber einen neuen
Mechanismus und eine klare neuronale Rückmeldungsroute.

### Option C – Zuerst Wiederholungsstabilität untersuchen

Vor jeder Gewichtsänderung wird geklärt, weshalb Datenaktivität im
fortgeführten Zustand nach der ersten Antwort weitgehend ausbleibt.

Das reduziert eine starke Störvariable, verschiebt aber die eigentliche
Rückmeldungsforschung.

### Option D – B ohne Lernmechanismus erweitern

Weitere Puffergrößen, Zeitfenster oder globale Diagnosemerkmale werden
untersucht, ohne neuronale Zustände zu verändern.

G4 zeigte gegenüber G3 keinen Zusatznutzen. Eine reine Ausweitung des Rings
verspricht daher wenig Erkenntnisgewinn.

## Vorgeschlagene Entscheidung

Option C wird als nächster eigenständiger Schritt empfohlen. Parallel kann
Option B als präziser Folgeauftrag spezifiziert werden, darf aber erst nach
einer gemeinsamen Architekturentscheidung Gewichte verändern.

Für Option B wird G3 mit acht Einträgen als Ausgangsvariante empfohlen. G2
bleibt Minimal- und G0 Kontrollvariante; G4 dient nur als Speichervergleich.
Die technische Diagnose bleibt ausschließlich externe Vergleichsgrundlage.

## Verbindliche Kontrollen einer späteren Lernstufe

- G0 ohne lokalen Speicher und ohne neue Gewichtsänderung.
- G3 mit Eligibility-Zustand, aber deaktivierter Rückmeldungswirkung.
- Identische erfolgreiche und fehlgeschlagene Rückmeldungen.
- Gezielte Nullgewicht- und K4-Unterbrechungen.
- Wiederhergestellte und fortgeführte Zustände getrennt.
- Protokollierung jeder geänderten Synapse mit neuronaler, nicht technischer
  Auswahlbegründung.
- Prüfung auf globale Verstärkung, Sättigung und unbeabsichtigte
  Selbstaktivierung.

## Konsequenzen

### Positiv

- Die nächste Lernforschung baut auf empirisch ausreichender lokaler
  Information auf.
- Mehrdeutigkeit und verzögerter Informationsverlust werden ausdrücklich
  kontrolliert.
- Wiederholungsinstabilität wird nicht durch Lernen verdeckt.

### Negativ oder begrenzend

- Eine automatische rückmeldungsabhängige Anpassung beginnt noch nicht.
- Eligibility-Markierungen erhöhen Zustand und Persistenzbedarf.
- G3 garantiert keine eindeutige Ursachenzuordnung.

## Nachweise

- [Entwicklungsauftrag B](../experimente/007-entwicklungsauftrag-forschungsstufe-b.md)
- [Forschungsbericht B](../experimente/007-forschungsbericht-forschungsstufe-b.md)
- [Lokales Ereignisgedächtnis](../konzept/EVE-Lokales-neuronales-Ereignisgedaechtnis.md)
- `research/results/research-stage-b-primary.csv`
- `research/results/research-stage-b-retention.csv`
- `research/results/research-stage-b-interruptions.csv`
- `research/results/research-stage-b-repetitions.csv`

## Überprüfung

Die Vorlage ist vor Einführung einer Eligibility-Markierung, eines neuen
Rückmeldungspfads oder irgendeiner rückmeldungsabhängigen Gewichtsänderung
gemeinsam zu entscheiden.
