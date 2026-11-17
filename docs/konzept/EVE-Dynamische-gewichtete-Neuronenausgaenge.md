# EVE – Dynamische gewichtete Neuronenausgänge

## Grundprinzip

Ein Neuron besitzt einen Signalausgang, der sich auf mehrere **gewichtete Ausgänge** verteilt. An jeden dieser Ausgänge können beliebig viele gerichtete Synapsen angeschlossen werden.

Synapsen besitzen keine eigene Gewichtung. Stattdessen übernehmen sie die Gewichtung des Ausgangs, dem sie zugeordnet sind.

## 1. Dynamische Ausgänge

- Ein Neuron kann beliebig viele ausgehende Synapsen besitzen, begrenzt durch die verfügbaren Ressourcen.
- Gewichtete Ausgänge werden nur angelegt, wenn sie benötigt werden.
- Mehrere Synapsen mit identischer Gewichtung teilen sich denselben Ausgang.
- Nicht mehr verwendete Ausgänge werden entfernt.
- Die Anzahl der Ausgänge verändert sich dynamisch.

## 2. Lernen durch Umordnung

Die Gewichtung einer einzelnen Synapse wird verändert, indem ihre Zuordnung angepasst wird.

**Beispiel:**

Ein Neuron besitzt 1.000 Synapsen mit den Gewichtungen 1 bis 1.000.

Eine Synapse soll von Gewicht 999 auf 998 wechseln.

1. Die Synapse wird vom Ausgang 999 getrennt.
2. Sie wird dem bereits vorhandenen Ausgang 998 zugeordnet.
3. Ausgang 999 besitzt keine Synapsen mehr und wird entfernt.

Es entsteht kein zusätzlicher Gewichtungswert.

Existiert die gewünschte Gewichtung noch nicht, wird ein entsprechender Ausgang angelegt.

## 3. Zwei Veränderungsmechanismen

**Individuelles Lernen**

Eine einzelne Synapse wird einem anderen gewichteten Ausgang zugeordnet. Andere Synapsen bleiben unverändert.

**Gemeinsame Anpassung**

Die Gewichtung eines bestehenden Ausgangs wird verändert. Dadurch ändert sich gleichzeitig die Wirkung sämtlicher angeschlossener Synapsen.

## 4. Speicherarchitektur

Für die Umsetzung in C/C++:

- Neuronen werden in zusammenhängenden Speicherbereichen verwaltet.
- Synapsen werden als gerichtete Verbindungen über kompakte Indizes repräsentiert.
- Gewichtungen werden pro Neuron nur einmal je tatsächlich verwendetem Wert gespeichert.
- Synapsen referenzieren den zugehörigen gewichteten Ausgang.
- Freigewordene Ausgangseinträge können wiederverwendet werden.
- Es werden keine eigenständigen Synapsenobjekte benötigt.

## 5. Vorteile und Grenzen

**Vorteile**

- Keine redundante Speicherung identischer Gewichtungen innerhalb eines Neurons.
- Dynamische Anpassung der Netzwerkstruktur.
- Individuelles und gemeinsames Lernen möglich.
- Keine feste Anzahl gewichteter Ausgänge erforderlich.
- Kompakte, zusammenhängende Speicherstrukturen möglich.

**Grenzen**

- Jede Synapse benötigt weiterhin eine Zielzuordnung und eine Ausgangsreferenz.
- Bei ausschließlich unterschiedlichen Gewichtungen entsteht kein Kompressionsvorteil.
- Die Verwaltung dynamischer Ausgänge verursacht zusätzlichen Aufwand.
- Der tatsächliche Speicher- und Energiegewinn muss experimentell gemessen werden.

## 6. Bedeutung für EVE

Dieses Modell trennt drei Eigenschaften:

1. **Neuron:** Verarbeitet Eingangssignale und entscheidet, ob es feuert.
2. **Gewichteter Ausgang:** Bestimmt die Signalwirkung für eine Gruppe von Synapsen.
3. **Synapse:** Stellt die gerichtete Verbindung zu einem anderen Neuron her.

Lernen erfolgt durch Veränderungen der Gewichtungen, durch Umordnung von Synapsen und gegebenenfalls durch Bildung oder Abbau von Verbindungen.

Das Modell ist biologisch inspiriert, aber keine exakte Nachbildung biologischer Synapsen.
