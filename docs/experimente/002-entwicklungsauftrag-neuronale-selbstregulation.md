# EVE – Experiment 002

## Neuronale Selbstregulation und synaptische Stabilität

**Auftrag an:** Nova-EVE

**Ausgangsbasis:** `codex/research-002-performance-memory`

**Status:** Entwicklungs- und Forschungsauftrag

## Ausgangslage und Forschungsfrage

Experiment 001 und Forschungsauftrag 002 bestätigten einen funktionsfähigen,
ereignisgesteuerten neuronalen Kern mit Lernen und Wachstum. Wiederholtes Lernen
führt jedoch auch ohne Synapsenwachstum zu unkontrollierter Aktivität; beim Seed
`20261117` wurde im zwölften `AB`-Durchgang das Limit von einer Million
Ereignissen erreicht.

Experiment 002 untersucht deshalb:

> Welche einfachen neuronalen Regulationsmechanismen ermöglichen es EVE,
> Erfahrungen dauerhaft zu speichern, ohne dass ihre Aktivität außer Kontrolle
> gerät und ohne die Unterscheidung neuer Erfahrungen zu verlieren?

## Verbindliche Varianten

Die bestehende Implementierung bleibt die deaktivierte Referenz. Die Mechanismen
werden zunächst ausschließlich einzeln untersucht.

### A – Maximales Synapsengewicht

Positive Einzelgewichte werden konfigurierbar begrenzt. Der Lernzähler arbeitet
auch bei Sättigung unverändert weiter.

| Variante | Maximalgewicht |
|---|---:|
| A0 | unbegrenzt |
| A1 | 1 |
| A2 | 2 |
| A3 | 4 |
| A4 | 8 |
| A5 | 16 |

Zu messen sind insbesondere Kaskaden, Sättigung, Synapsen am Grenzwert,
Gewichtsänderungen sowie Unterschiede trainierter und untrainierter Netze.

### B – Gesamtgewichtsbudget je Neuron

Die Summe positiver Eingangsgewichte wird begrenzt. Bei freiem Budget wird normal
verstärkt. Bei ausgeschöpftem Budget wird reproduzierbar zufällig eine andere
positive Eingangssynapse gewählt, um eins abgeschwächt und die lernende Synapse
um eins verstärkt. Die lernende Synapse selbst ist ausgeschlossen. Gewicht null
bleibt als Verbindung bestehen; diese Versuchsregel erzeugt keine negativen
Gewichte und keinen zusätzlichen Synapsenschwund. Die Auswahlregel muss
austauschbar gekapselt sein.

| Variante | Eingangsbudget |
|---|---:|
| B0 | unbegrenzt |
| B1 | 4 |
| B2 | 8 |
| B3 | 16 |
| B4 | 32 |
| B5 | 64 |

Eine Abhängigkeit des Budgets von der Zahl ausgehender Synapsen ist nur
theoretisch zu bewerten und nicht zu implementieren.

### C – Ereignisbasierte Refraktärzeit

Nach dem Feuern darf ein Neuron für die nächsten konfigurierten eingehenden
Ereignisse nicht erneut feuern. Eingänge, Aktivitätsphasen, Synapsenzähler und
Lernen laufen weiter. Die Behandlung des Membranpotenzials ist eindeutig zu
dokumentieren. Die ereignisbasierte Sperre ist nicht mit biologischer Zeit
gleichzusetzen.

| Variante | gesperrte Eingangsereignisse |
|---|---:|
| C0 | 0 |
| C1 | 1 |
| C2 | 2 |
| C3 | 4 |
| C4 | 8 |
| C5 | 16 |

## Unveränderte Grundregeln

- C++17, BIOS-, Stammhirn- und Netzwerkarchitektur bleiben erhalten.
- Keine semantischen Neuronen, Wörter, Sequenzrepräsentationen, LLMs oder
  neuronalen Frameworks.
- UTF-8-Bitcodierung, Wachstumswahrscheinlichkeit, Lernzähler und
  Aktivitätsphasen bleiben unverändert, soweit die Variante nichts anderes
  verlangt.
- Jeder Mechanismus ist einzeln aktivierbar; deaktiviert muss das bisherige
  Verhalten reproduzierbar bleiben.
- Vorhandene lokale Änderungen dürfen nicht überschrieben werden.

## Versuchsaufbau

- 256 Neuronen, 1.024 initiale Synapsen, unveränderte 3D-Faltung.
- Mindestens die Seeds `20261117`, `7`, `42` und `99`, ergänzt um weitere Seeds.
- Training mit `AB`, `BA`, `AA` und `BB` bis zu 512 Wiederholungen.
- Kontrollierter Abbruch am Sicherheitslimit; unvollständige Läufe sind nicht
  stabil.
- Stabilität wird von Inaktivität und Instabilität getrennt.

Zu erfassen sind Ereignisse, Feuerereignisse, Wachstum, Verstärkungen,
Abschwächungen, Gewichtsverteilung, aktive Neuronen, maximale Warteschlange,
Kaskaden und abgeschlossene Wiederholungen.

## Lern-, Wiedererkennungs- und Interferenzprüfung

Identische Netze werden als `AB`-Training, untrainierte Kontrolle und Kontrolle
mit gleichem Eingabevolumen in anderer Reihenfolge geführt. Vor jedem Test wird
derselbe gespeicherte Zustand wiederhergestellt. Testfolgen sind `AB`, `BA`,
`AC`, `AA` und `BB`. Zu vergleichen sind Gesamtaktivität, beteiligte Neuronen,
Aktivitätsreihenfolge, Gewichte und Struktur. Unterschiedliche Fingerabdrücke
allein sind kein Wiedererkennungsnachweis.

Für hinreichend stabile Varianten folgt auf `AB`-Training die Erfahrung mit
`CD`, `EF` und `GH`; anschließend ist die `AB`-Reaktion erneut zu prüfen.

## Performance, Tests und Ergebnisse

Statische Verarbeitung und aktives Lernen werden getrennt vermessen. Berichtet
werden CPU-Zeit, Ereignisse pro Sekunde, Speicher je Neuron und Synapse,
Verwaltungsdaten und Konkurrenzaufwand.

Automatisierte Tests decken Grenzgewicht, Budgetinvariante, reproduzierbare
Umverteilung, Ausschluss der lernenden Synapse, Refraktärsperre bei weiterlaufendem
Lernen, Persistenz und deaktivierte Referenz ab.

Erwartet werden Forschungsbericht, reproduzierbarer Runner, maschinenlesbare
Rohdaten, Parameterdatei, Vergleichszusammenfassung und Tests. Beobachtungen und
Interpretationen sind zu trennen. Keine Versuchsvariante wird ohne gemeinsame
Auswertung zur endgültigen EVE-Architektur erklärt.

## Abgrenzung

Nicht Bestandteil sind Sprachverständnis, vorgefertigte Worterkennung,
Community-Lernen, neue Semantik, STDP, dynamische neuronale Rollen, neue Sensorik,
produktive Chatto-Kommunikation und dynamische Budgets nach ausgehenden Synapsen.
