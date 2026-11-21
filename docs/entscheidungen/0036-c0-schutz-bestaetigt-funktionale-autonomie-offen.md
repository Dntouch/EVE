# 0036 – C.0: Schutz bestätigt, funktionale Autonomie offen

**Status:** angenommen  
**Datum:** 21. November 2026

Die methodische Rolle des Schutzvertrags wird durch
[Entscheidung 0037](0037-schutzvertrag-ist-voraussetzung-nicht-lernziel.md)
verbindlich präzisiert: Integrität ist ein unabhängiges Freigabegate und keine
Lernmetrik.

## Entscheidung

Der semantische Growth-Schutzvertrag und die persistierbare,
ereignisgebundene interne Exploration werden als Forschungsinfrastruktur
angenommen. Automatische Exploration und Growth bleiben außerhalb expliziter
Forschungskonfigurationen deaktiviert. Es wird keine neue Lernregel
eingeführt; S4 bleibt standardmäßig deaktiviert.

Autonome interne Exploration und Growth dürfen nur gemeinsam aktiviert sein,
wenn ein vollständiger, sicherer struktureller Schutzvertrag aktiv ist.
Konfiguration, spätere Umschaltung und geladene Zustände erzwingen diese
Invariante. Growth ohne autonome Exploration und Exploration ohne Growth
bleiben für historische Forschungsmodi zulässig.

## Begründung

12.800 zentrale Läufe erzeugten 102.400 autonome Explorationen und 166.261
geschützte Growth-Ereignisse ohne Schutzverletzung. Persistenz,
deterministische Fortsetzung und 832 Regressionstests bestehen.

1.802 natürliche neue Verbindungen erhielten positives Gewicht. 19 erreichten
Gewicht 2 und aktivierten ihr Ziel; W1 entfernte diese Aktivierung in 19/19
Fällen. Acht dieser Fälle liegen in der unabhängigen Bestätigung. Nach der
interventionsfreien W0-Folgeerfahrung waren alle 19 Effekte verloren; W2, W4
und W5 reproduzieren dies, W6 erhält 19/19. Von 48 negativen W0-Schreibungen
sind genau die 19 Abschwächungen der ausgewählten neuen Kante von 2 auf 1 in
isolierter Gegenprobe funktionsmindernd. Die übrigen 29 Schreibungen zeigen
isoliert keinen Funktionseffekt und rechtfertigen keinen pauschalen
Gewichtsschutz. Im untersuchten einfachen 128-Neuronen-Vergleich verbessert
K3 gegenüber K0 weder Erfahrungen noch Repertoire. Erfolgskriterien A und B
bestehen, C für diesen Vergleich nicht.

## Folgen

- Rollen statt Neuron-IDs definieren strukturelle Sicherheit.
- Neue Struktur bleibt diagnostisch S2, bis Funktion und Kausalität belegt
  sind.
- C.1 darf die Stabilisierung S4→S5 isolieren.
- Kein automatischer Rollback und kein pauschaler Gewichtsschutz werden
  eingeführt.
- Schutzinfrastruktur ist mergefähig; Produktionsautonomie ist es nicht.
