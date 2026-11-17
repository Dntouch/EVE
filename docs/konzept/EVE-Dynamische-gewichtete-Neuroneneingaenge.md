# EVE – Dynamische gewichtete Neuroneneingänge

## Grundprinzip

Jedes Neuron besitzt einen einzigen logischen Ausgang und eine dynamische Anzahl gewichteter Eingänge.

Feuert ein Neuron, wird das gleiche Signal über sämtliche ausgehenden Synapsen übertragen. Der Sender nimmt keine Gewichtung oder Auswahl vor.

Das empfangende Neuron bestimmt anhand der Gewichtung seiner jeweiligen Eingangsverbindung, wie stark das Signal sein Membranpotenzial beeinflusst.

## 1. Neuronenausgang

- Jedes Neuron besitzt genau einen logischen Signalausgang.
- An diesen können beliebig viele ausgehende Synapsen angeschlossen sein, begrenzt durch die verfügbaren Ressourcen.
- Feuert das Neuron, werden sämtliche angeschlossenen Synapsen aktiviert.
- Der Ausgang besitzt keine eigene Gewichtung.
- Der Sender kennt die Gewichtungen der Empfänger nicht.

## 2. Dynamische gewichtete Eingänge

- Ein Neuron kann beliebig viele eingehende Synapsen besitzen.
- Jede eingehende Synapse ist einer Gewichtungsgruppe zugeordnet.
- Mehrere Synapsen mit identischer Gewichtung teilen sich dieselbe Gruppe.
- Gewichtungsgruppen werden nur angelegt, wenn sie benötigt werden.
- Nicht mehr verwendete Gewichtungsgruppen werden entfernt.
- Die Anzahl der Gewichtungsgruppen verändert sich dynamisch.

## 3. Signalverarbeitung

Empfängt ein Neuron ein Signal:

1. Die zugehörige Eingangsverbindung wird identifiziert.
2. Die Gewichtung der Verbindung wird ermittelt.
3. Das Membranpotenzial wird entsprechend verändert.
4. Das Neuron prüft, ob seine Reizschwelle erreicht wurde.
5. Bei Erreichen der Reizschwelle feuert das Neuron über seinen einzigen Ausgang.

Die Reizschwelle und die Gewichtungen sind voneinander unabhängige Eigenschaften.

## 4. Lernen durch Umordnung

Eine Synapse verändert ihre effektive Gewichtung, indem sie einer anderen Eingangsgewichtungsgruppe zugeordnet wird.

**Beispiel:**

Neuron B besitzt 1.000 eingehende Synapsen mit den Gewichtungen 1 bis 1.000.

Eine Synapse soll von Gewicht 999 auf 998 wechseln.

1. Die Synapse wird aus Gewichtungsgruppe 999 entfernt.
2. Sie wird der bereits vorhandenen Gewichtungsgruppe 998 zugeordnet.
3. Die nicht mehr verwendete Gewichtungsgruppe 999 wird entfernt.

Existiert die gewünschte Gewichtung noch nicht, wird eine neue Gewichtungsgruppe angelegt.

## 5. Veränderungsmechanismen

**Individuelles Lernen**

Eine einzelne eingehende Synapse wird einer anderen Gewichtungsgruppe zugeordnet. Andere Verbindungen bleiben unverändert.

**Gemeinsame Anpassung**

Die Gewichtung einer bestehenden Gruppe wird verändert. Dadurch verändert sich die Wirkung sämtlicher dieser Gruppe zugeordneten Synapsen.

**Strukturelles Lernen**

Synapsen können neu entstehen, auf andere Neuronen umgeleitet oder entfernt werden. Die genauen Lernregeln werden gesondert definiert.

## 6. Speicherarchitektur

Die Umsetzung erfolgt in C/C++.

- Neuronen werden in zusammenhängenden Speicherbereichen verwaltet.
- Synapsen werden durch kompakte Indizes statt durch eigenständige Objekte repräsentiert.
- Gewichtungen werden beim empfangenden Neuron verwaltet.
- Identische Gewichtungen innerhalb eines Neurons werden gemeinsam gespeichert.
- Jede Synapse referenziert die passende Eingangsgewichtungsgruppe.
- Nicht mehr verwendete Gruppen werden freigegeben oder ihre Speicherplätze wiederverwendet.
- Die ausgehenden Verbindungen müssen effizient auffindbar sein, damit ein feuerndes Neuron seine Empfänger benachrichtigen kann.

Die logische Zuordnung der Gewichtungen zum Empfänger ist festgelegt. Die konkrete physische Speicherorganisation bleibt offen.

## 7. Vorteile und Grenzen

**Vorteile**

- Klare Trennung zwischen Signalübertragung und Signalbewertung.
- Der Sender benötigt keine Kenntnis über die Gewichtungen.
- Jedes Neuron verwaltet seine eigene Reaktion auf eingehende Signale.
- Identische Eingangsgewichtungen können gemeinsam gespeichert werden.
- Dynamische Anpassung und Umordnung von Verbindungen.
- Ereignisgesteuerte Verarbeitung ohne permanente Berechnung sämtlicher Neuronen.

**Grenzen**

- Jede Verbindung muss weiterhin eindeutig identifizierbar sein.
- Die Zuordnung zu Gewichtungsgruppen benötigt Speicher.
- Bei ausschließlich unterschiedlichen Gewichtungen entsteht kein Kompressionsvorteil.
- Die Verwaltung dynamischer Gruppen verursacht zusätzlichen Aufwand.
- Der tatsächliche Speicher- und Energiegewinn muss experimentell überprüft werden.

## 8. Bedeutung für EVE

Das neuronale Modell besteht aus drei funktionalen Elementen:

1. **Neuron:** Verwaltet Membranpotenzial, Reizschwelle und eigene Eingangsgewichtungen.
2. **Eingangsgewichtungsgruppe:** Bestimmt die Wirkung aller ihr zugeordneten eingehenden Synapsen.
3. **Synapse:** Stellt eine gerichtete Verbindung zwischen zwei Neuronen her.

Das Lernen erfolgt durch Veränderungen der Eingangsgewichtungen, durch Umordnung der Synapsen zwischen Gewichtungsgruppen und gegebenenfalls durch strukturelle Veränderungen des Netzwerks.

Das Modell ist biologisch inspiriert, stellt aber eine bewusste technische Vereinfachung dar.

## Leitgedanke

**Der Sender entscheidet, ob er feuert. Der Empfänger entscheidet, wie stark das empfangene Signal auf ihn wirkt.**

Identische Gewichtungen werden gemeinsam verwaltet, ohne die individuelle Veränderbarkeit einzelner Verbindungen aufzugeben.
