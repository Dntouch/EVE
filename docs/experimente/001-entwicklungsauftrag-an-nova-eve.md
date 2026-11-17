# Entwicklungsauftrag an Nova-EVE
## EVE – Neuronaler Kern, Experiment 001

### Ziel

Implementiere das erste ausführbare Grundgerüst für EVEs biologisch inspiriertes neuronales Netzwerk.

Grundlage ist die bereits im Repository `Dntouch/EVE` hinterlegte Spezifikation **„EVE – Experiment 001“**.

Lies vor der Implementierung die vorhandenen Konzeptdokumente, Architekturentscheidungen und die Roadmap.

Das Ziel ist ein reproduzierbares, beobachtbares Experiment, mit dem wir neuronale Aktivität, synaptisches Lernen und später die Verarbeitung zeitlicher Signalmuster untersuchen können.

### 1. Technische Anforderungen

- Implementierung in C oder C++.
- Eigenständiger neuronaler Kern ohne Python-Laufzeit.
- Keine externen KI-Modelle oder neuronalen Frameworks.
- Plattformunabhängige Kernarchitektur.
- Kompakte Speicherstrukturen.
- Neuronen und Synapsen über numerische Indizes referenzieren.
- Reproduzierbare Zufallsinitialisierung.
- Persistenz der vollständigen neuronalen Zustände.
- Ereignisgesteuerte Verarbeitung statt permanenter globaler Neuronendurchläufe.

### 2. Architektur

Implementiere getrennte Komponenten für:

**BIOS**
- Technische Eingabe und Ausgabe.
- Ereignisverwaltung.
- Persistenz.
- Konfiguration und Diagnose.

**Stammhirn**
- Sensorische Reizumwandlung.
- Neuronale Entwicklungsregeln.
- Synaptische Lernmechanismen.
- Wachstumssteuerung.

**Neuronales Netzwerk**
- Neuronen.
- Gerichtete Synapsen.
- Membranpotenziale.
- Eingangsgewichtungen.
- Aktivitätszähler.
- Feuerereignisse.

Die Komponenten müssen klar voneinander getrennt sein.

### 3. Initiales Netzwerk

Initialisiere:

- 256 Neuronen.
- 4 zufällige ausgehende Synapsen je Neuron.
- Insgesamt 1.024 gerichtete Synapsen.
- Keine Selbstverbindungen.
- Keine doppelten gerichteten Verbindungen.

Verwende eine einmalige logische Faltung der Neuronen in einen dreidimensionalen Raum von 8 × 8 × 4.

Die virtuellen Koordinaten werden aus dem Neuronenindex berechnet und nicht individuell gespeichert.

### 4. Neuronenmodell

Jedes Neuron benötigt:

- Membranpotenzial.
- Konfigurierbare Erregungsschwelle.
- Lokalen Eingangsereigniszähler.
- Einen einzigen logischen, ungewichteten Ausgang.

Beim Feuern wird derselbe binäre Impuls an sämtliche ausgehenden Synapsen übertragen.

Die Gewichtung liegt ausschließlich auf der Empfängerseite.

Die konkrete Regel zum Zurücksetzen des Membranpotenzials ist noch nicht endgültig beschlossen. Implementiere sie deshalb austauschbar und dokumentiere die für den ersten Test verwendete Variante.

### 5. Synaptisches Lernen

Neue Synapsen beginnen mit Gewichtung 0.

Jeder eingehende Impuls erhöht:

- Den lokalen Eingangszähler des Neurons.
- Den Aktivitätszähler der betreffenden Synapse.

Experimentelle Startwerte:

- 3 Impulse über dieselbe Synapse: Gewichtung +1.
- 8 Eingangsimpulse insgesamt: Ende der neuronalen Aktivitätsphase.

Beim Phasenende werden der Neuronenzähler und sämtliche zugehörigen Synapsenzähler zurückgesetzt.

Wird mit einem Impuls gleichzeitig die synaptische Lernschwelle und das Phasenende erreicht, erfolgt zuerst die synaptische Verstärkung.

Die Lernregel funktioniert unabhängig davon, ob das empfangende Neuron selbst feuert.

### 6. Synapsenwachstum

Ein Neuron kann bei einem eigenen Feuerereignis einen Wachstumsversuch auslösen.

Als experimentelle Wachstumswahrscheinlichkeit gilt:

`P = 1 / (1 + k)`

`k` ist die Anzahl bereits vorhandener ausgehender Synapsen.

Bei erfolgreichem Wachstumsversuch:

- Suche zunächst in der unmittelbaren virtuellen 3D-Nachbarschaft.
- Schließe Selbstverbindungen und vorhandene Verbindungen aus.
- Erweitere bei Bedarf den Suchradius.
- Erzeuge die neue Synapse mit Gewichtung 0.

Es gibt zunächst keine festgelegte maximale Synapsenzahl pro Neuron.

### 7. Sensorische Schnittstelle

Implementiere eine erste technische Reizumwandlung im Stammhirn.

Das BIOS übergibt rohe UTF-8-Bytes.

Das Stammhirn wandelt jedes Byte über acht sensorische Kanäle in binäre Feuerereignisse um.

- Ein gesetztes Bit erzeugt einen Impuls auf dem entsprechenden Kanal.
- Die Bytes werden in ihrer ursprünglichen Reihenfolge verarbeitet.
- Byte-Grenzen und Reihenfolge müssen eindeutig erhalten bleiben.
- Es findet keine Wort-, Zeichen- oder Bedeutungsinterpretation statt.

Die acht Kanäle sind technische Eingangskanäle und keine semantisch vorbelegten Neuronen.

Die konkrete Anbindung der Kanäle an das neuronale Netzwerk muss dokumentiert und für Tests reproduzierbar sein.

### 8. Sequenzexperiment

Bereite einen isolierten Test zur Verarbeitung unterschiedlicher Impulsfolgen vor.

Testfälle:

- `AB`
- `BA`
- Wiederholtes `AB`
- Wiederholtes `BA`

Untersuche, ob das vorhandene Neuronenmodell unterschiedliche Aktivitätsverläufe erzeugt und ob wiederholte Eingaben zu unterscheidbaren, stabilen Veränderungen führen.

Füge dafür zunächst keine zusätzlichen Mechanismen wie STDP, Zeitstempel oder besondere Sequenzneuronen hinzu.

Wenn das bestehende Modell keine Reihenfolge lernen kann, dokumentiere dieses Ergebnis.

### 9. Diagnose und Beobachtung

Stelle mindestens folgende Messwerte bereit:

- Anzahl der Neuronen.
- Anzahl vorhandener Synapsen.
- Anzahl Feuerereignisse.
- Anzahl abgeschlossener Aktivitätsphasen.
- Anzahl synaptischer Verstärkungen.
- Anzahl neu entstandener Synapsen.
- Verteilung der Eingangsgewichtungen.
- Speicherverbrauch.
- Verarbeitete Ereignisse.
- Erreichbare Reproduzierbarkeit bei gleichem Zufallsstartwert.

Für einzelne Testläufe muss eine detaillierte Ereignisprotokollierung möglich sein.

### 10. Noch nicht implementieren

Folgende Mechanismen sind noch nicht abschließend spezifiziert und dürfen nicht eigenmächtig als endgültige Lernregeln eingeführt werden:

- Synaptische Abschwächung.
- Synapsenentfernung durch Lernen.
- Synaptische Normalisierung.
- Maximale Gewichtsbudgets.
- Ausgangsabhängige Gewichtsbudgets.
- STDP.
- Vorgegebene Sprach- oder Worterkennung.
- Semantisch festgelegte Neuronen.
- Vorgefertigtes Wissen.
- Community-Lernsystem.
- Chatto-Integration als produktive Kommunikationsschnittstelle.

Bereite die Architektur so vor, dass diese Funktionen später ergänzt werden können.

### 11. Tests

Erstelle automatisierte Tests für:

1. Korrekte Netzwerkinitialisierung.
2. Ausschluss doppelter Synapsen.
3. Korrekte 3D-Indexberechnung.
4. Weiterleitung binärer Feuerereignisse.
5. Gewichtete Eingangsverarbeitung.
6. Synaptische Verstärkung nach drei Impulsen.
7. Rücksetzung der Aktivitätsphase nach acht Impulsen.
8. Korrekte Reihenfolge bei gleichzeitigem Erreichen beider Schwellen.
9. Lernen über Synapsen mit Gewichtung 0.
10. Wachstumswahrscheinlichkeit und Partnerauswahl.
11. Reproduzierbare Ausführung mit identischem Zufallsstartwert.
12. Speichern und Wiederherstellen neuronaler Zustände.
13. Korrekte sensorische Verarbeitung von UTF-8-Bytes.

### 12. Arbeitsweise

Arbeite zunächst das vorhandene Repository durch.

Erstelle anschließend eine kurze technische Umsetzungsplanung.

Identifiziere alle noch fehlenden Entscheidungen, die für einen ausführbaren Prototyp zwingend notwendig sind.

Treffe keine stillschweigenden Architekturentscheidungen, die den bisherigen Konzepten widersprechen.

Implementiere den ersten funktionsfähigen Prototyp mit klar gekennzeichneten, austauschbaren Versuchsparametern.

Dokumentiere sämtliche getroffenen Annahmen.

**Ziel der ersten Version ist nicht Sprachfähigkeit, sondern ein technisch überprüfbares neuronales System, dessen Lern- und Entwicklungsverhalten wir anschließend experimentell untersuchen können.**
