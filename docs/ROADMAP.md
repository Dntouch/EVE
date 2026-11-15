# Roadmap zum ersten funktionalen Prototyp

**Status:** Entwurf  
**Horizont:** zwölf Wochen ab freigegebenem Projektstart  
**Quelle:** EVE-Konzept vom 15. November 2026

Die Roadmap beschreibt Zielzustände und Nachweise. Sie legt noch keine
Programmiersprache, Bibliothek oder konkrete Implementierungsarchitektur fest.

## Vor dem Start: Gate 0

Die zwölf Wochen beginnen erst, wenn folgende Voraussetzungen geklärt sind:

- Projektumfang und Erfolgskriterien sind angenommen.
- Rollen, Verantwortlichkeiten und Entscheidungsweg sind benannt.
- Lizenz und Umgang mit Forschungsdaten sind entschieden.
- erste Lernaufgabe und Kontrollbedingungen sind beschrieben.
- Messgrößen für Lernen, Reproduzierbarkeit und Ressourcenverbrauch sind
  definiert.
- Sicherheits- und Handlungsgrenzen der Versuche sind freigegeben.

## Phase 1 – Wochen 1 bis 3: BIOS und neuronale Engine

### Ziele

- ereignisgesteuerte Verarbeitung
- Neuronen, Synapsen und diskrete Impulse
- zeitliche Integration, Reizschwellen und Erholungszeiten
- erste angeborene Regulationsfunktionen
- Energie- und Ressourcenmessung
- Diagnostik und reproduzierbare Wiederholung

### Erforderliche Nachweise

- gleiche Ausgangsbedingungen erzeugen den gleichen beobachtbaren Verlauf,
- Impulse und Zustandsänderungen sind zeitlich nachvollziehbar,
- BIOS und neuronaler Kern sind in Verantwortung und Schnittstellen getrennt,
- interne Aktivitätskosten und reale Hardwaremessung werden nicht vermischt.

## Phase 2 – Wochen 4 bis 6: Lernen und Erinnerung

### Ziele

- synaptische Plastizität und Aktivitätsregulation
- persistenter neuronaler Zustand
- einfache Entscheidungsaufgaben
- Lernen aus Konsequenzen
- erste messbare Lernkurven

### Erforderliche Nachweise

- Verhalten verbessert sich durch Erfahrungen statt durch versteckte Regeln,
- die erworbene Veränderung bleibt nach einem Neustart erhalten,
- eine Kontrollgruppe ohne Lernmechanismus zeigt den Effekt nicht,
- Fehler und unerwartete Konsequenzen bleiben analysierbar.

## Phase 3 – Wochen 7 bis 9: Selbstorganisation

### Ziele

- Veränderung neuronaler Eigenschaften
- Bildung und Entfernung von Verbindungen
- strukturelle Anpassung
- Stabilisierung erworbener Fähigkeiten
- Untersuchung des Energieaufwands

### Erforderliche Nachweise

- strukturelle Änderungen sind Ursache und Wirkung zuordenbar,
- erlernte Fähigkeiten werden nicht unkontrolliert überschrieben,
- Instabilität wird erkannt und reproduzierbar untersucht,
- Nutzen, Zuverlässigkeit und Ressourcenaufwand werden gemeinsam bewertet.

## Phase 4 – Wochen 10 bis 12: Praktische Integration

### Ziele

- ausgewählte, begrenzte EVE-Schnittstellen
- Interaktion mit Menschen
- erste Lernaufgaben für Zeichen und einfache Mathematik
- Vergleich mit dem bestehenden LLM als Lehrer oder Referenz
- Messung von Zuverlässigkeit und Energieverbrauch
- systematische Analyse von Fehlschlägen

### Erforderliche Nachweise

- die ausgewählten Aufgaben funktionieren ohne LLM-Entscheidungen,
- Lehrer-, Mess- und Entscheidungskanal sind eindeutig getrennt,
- erworbene Zusammenhänge werden auf eine veränderte Aufgabe übertragen,
- nach veränderten Umweltbedingungen ist erneute Anpassung messbar.

## Definition eines erfolgreichen ersten Prototyps

Der Prototyp gilt nur dann als erfolgreich, wenn kontrollierte Versuche zeigen:

1. Eine zunächst unbekannte Aufgabe wird nicht bereits durch eine fest
   programmierte Lösung beherrscht.
2. EVE trifft Entscheidungen und erfährt deren Konsequenzen.
3. Der neuronale Zustand verändert sich überprüfbar.
4. Die Leistung verbessert sich über mehrere Erfahrungen hinweg.
5. Die Veränderung bleibt nach Wiederherstellung des Zustands wirksam.
6. Gelerntes unterstützt eine verwandte, aber unbekannte Aufgabe.
7. Das System kann sich nach einer Umweltänderung erneut anpassen.
8. Zufallstreffer und verdeckte LLM-Entscheidungen sind durch Kontrollen
   ausgeschlossen.

## Nicht Bestandteil dieser Roadmap

- Zusage menschenähnlicher oder allgemeiner Intelligenz
- anatomisch vollständige Simulation eines Gehirns
- unbeschränkter Zugriff auf externe Systeme
- Festlegung auf bestimmte Hardware
- Garantie des langfristigen 20-Watt-Orientierungsziels
