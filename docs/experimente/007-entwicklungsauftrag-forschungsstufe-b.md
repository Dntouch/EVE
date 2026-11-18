# EVE – Forschungsstufe B
## Lokales neuronales Ereignisgedächtnis und Zuordnung von Handlungen

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Ausgangsbasis:** `codex/research-stage-a2-action-causality`  
**Status:** Forschungs- und Entwicklungsauftrag  
**Implementierung:** C++17

---

## 1. Ausgangssituation

Forschungsstufe A.2 hat erstmals einen kausal nachvollziehbaren neuronalen Handlungsabschluss nachgewiesen.

Die motorische Schaltung M3 verwendet einen Akkumulator, der vorausgegangene Datenmotorereignisse integriert und bei Erreichen seiner Erregungsschwelle den neuronalen Abschlusskontakt aktiviert.

In 32 primären Versuchsläufen ergaben sich:

- 22 Läufe mit motorischer Datenaktivität.
- 7 vollständige M3-Abschlüsse nach vorausgegangener Datenaktivität.
- 7 rekonstruierte neuronale Abschlusswege.
- Kein nachgewiesener M3-Abschlussweg bei funktional unterbrochener Schaltung K2.

Bei fortgeführtem neuronalen Zustand nahm die Motoraktivität nach der ersten Antwortphase stark ab.

Daher erfolgt die primäre Forschung zunächst mit kontrolliert wiederhergestellten neuronalen Ausgangszuständen.

**Forschungsstufe B untersucht, ob das neuronale Netzwerk selbst lokale Erinnerungen an vorausgegangene Aktivität bilden kann, die später zur Zuordnung von Lernrückmeldungen verwendet werden könnten.**

Eine tatsächliche rückmeldungsabhängige Veränderung der Synapsengewichte ist noch nicht Bestandteil dieser Forschungsstufe.

## 2. Zentrale Forschungsfrage

**Können Neuronen mit einem minimalen lokalen Ereignisgedächtnis die vorausgegangenen Eingangssignale erfassen, die an ihrer eigenen Aktivierung und an einer anschließenden motorischen Handlung beteiligt waren?**

Dabei muss zwischen zeitlicher Nähe, neuronaler Beteiligung und tatsächlicher kausaler Wirkung unterschieden werden.

Ein Eingang, der unmittelbar vor einem Feuerereignis eintrifft, muss nicht zwangsläufig dessen alleinige Ursache sein.

## 3. Architekturprinzipien

Die bestehende neuronale Architektur bleibt erhalten.

Insbesondere gelten:

- Ein logischer, ungewichteter Ausgang pro Neuron.
- Individuell gewichtete Eingangssynapsen.
- Ereignisgesteuerte Signalverarbeitung.
- Keine globale neuronale Uhr.
- Keine individuellen Zeitstempel pro Synapse.
- Keine vorgegebenen semantischen Rollen.
- Keine Veränderung der bisherigen Lernregeln.
- Keine neuen Rückmeldungs- oder Belohnungsmechanismen.
- Keine automatischen synaptischen Gewichtsänderungen durch das Ereignisgedächtnis.

Das Ereignisgedächtnis soll als lokaler Bestandteil neuronaler Verarbeitung untersucht werden.

Technische Forschungsprotokolle und neuronale Speicherzustände sind strikt voneinander zu trennen.

## 4. Forschungsphase I – Lokales Ereignisgedächtnis

### Ziel

Ergänze Neuronen um die experimentelle Fähigkeit, eine begrenzte Anzahl zuletzt empfangener synaptischer Eingangsereignisse festzuhalten.

Die Ereignisse müssen in ihrer Eingangsreihenfolge unterscheidbar bleiben.

### Versuchsvarianten

| Variante | Gespeicherte Eingangsereignisse pro Neuron |
|---|---:|
| G0 | 0 – Referenz |
| G1 | 2 |
| G2 | 4 |
| G3 | 8 |
| G4 | 16 |

Die Größen sind experimentelle Parameter.

### Speicherprinzip

Verwende möglichst eine kompakte Ringpufferstruktur.

Ein Eintrag enthält zunächst nur die ID der betreffenden Eingangssynapse.

Die Reihenfolge ergibt sich aus der Position im Puffer.

Es werden keine physikalischen Zeitstempel benötigt.

Wiederholte Ereignisse derselben Synapse müssen als eigenständige Eingänge erkennbar bleiben.

### Beobachtung

Untersuche:

- Welche Eingangssynapsen waren zuletzt aktiv?
- In welcher Reihenfolge trafen ihre Impulse ein?
- Wie viele unterschiedliche Synapsen waren beteiligt?
- Welche Informationen gehen bei kleinen Puffern verloren?
- Welcher Speicheraufwand entsteht pro Neuron?

Der Speicher muss auch Eingänge mit Gewichtung 0 erfassen können.

## 5. Forschungsphase II – Ereignisse und neuronales Feuern

### Ziel

Untersuche, welche Informationen das lokale Ereignisgedächtnis unmittelbar beim Feuern eines Neurons enthält.

### Vorgehen

Wenn ein eingehender Impuls das Feuern eines Neurons auslöst, soll dessen aktuelles Ereignisgedächtnis für die Versuchsauswertung als Momentaufnahme erfasst werden können.

Diese Momentaufnahme muss zum tatsächlichen Zeitpunkt der Feuerentscheidung gehören.

Wird das Feuerereignis erst später aus der Ereigniswarteschlange verarbeitet, darf die Momentaufnahme dadurch nicht nachträglich verändert werden.

### Fragestellungen

- Welche Eingänge gingen dem Feuerereignis voraus?
- Welcher Eingang führte zur Überschreitung der Erregungsschwelle?
- Welche vorausgegangenen Eingänge haben zum Membranpotenzial beigetragen?
- Welche Eingänge wurden lediglich registriert, ohne erregend zu wirken?
- Wie beeinflusst die Refraktärphase diese Zuordnung?
- Wie viele vorausgegangene Ereignisse müssen gespeichert werden, damit relevante Eingänge erhalten bleiben?

Besonders wichtig:

**Der zuletzt eingegangene Impuls ist nicht automatisch der einzige ursächlich beteiligte Impuls.**

Ein Neuron kann erst durch die Summe mehrerer Eingänge seine Erregungsschwelle erreichen.

## 6. Forschungsphase III – Zuordnung einer vollständigen Handlung

### Ziel

Verknüpfe die lokalen Ereignisgedächtnisse mit den bereits nachgewiesenen M3-Handlungsabläufen.

### Versuchsgrundlage

Verwende die bestehende M3-Schaltung mit:

- Angeborener Grundverschaltung V2.
- Reproduzierbaren Anfangszuständen.
- Eingeschränkter Exploration E4-neu.
- Bestehender Refraktärregel.
- Deaktiviertem Lernen und Wachstum.

Die verwendeten Parameter sind vollständig zu dokumentieren.

### Ablauf

1. Initialisiere einen reproduzierbaren Netzwerkzustand.
2. Präsentiere die festgelegte Eingabe.
3. Starte die Antwortphase.
4. Erfasse neuronale Eingangsereignisse in lokalen Ereignisgedächtnissen.
5. Erfasse die beim Feuern beteiligten Eingänge.
6. Beobachte motorische Datenereignisse.
7. Beobachte gegebenenfalls den Abschlusskontakt.
8. Vergleiche lokale Ereignisspuren mit der unabhängigen technischen Ereignisdiagnose.

### M3-Kausalitätskette

Untersuche insbesondere:

- Die vorausgegangenen Eingänge der Datenmotorneuronen.
- Die Eingangssignale des M3-Akkumulators.
- Die Akkumulation bis zur Erregungsschwelle.
- Die Weiterleitung an das Abschlussneuron.
- Die tatsächliche Feuerentscheidung des Abschlussneurons.

Es soll nachvollziehbar sein, welche lokalen neuronalen Informationen für die Rekonstruktion dieser Kette zur Verfügung standen.

Das Stammhirn darf dem neuronalen Ereignisgedächtnis keine fertige Kausalitätskette übergeben.

Die technische Diagnose darf diese Kette unabhängig rekonstruieren und mit den neuronalen Daten vergleichen.

## 7. Forschungsphase IV – Kontrollen und kausale Prüfung

### Kontrollgruppen

**K0 – Ohne Ereignisgedächtnis**

Referenz mit unveränderter neuronaler Verarbeitung.

**K1 – Ereignisgedächtnis aktiv**

Untersuche die verschiedenen Puffergrößen.

**K2 – Motorische M3-Schaltung unterbrochen**

Verwende die bereits etablierte funktional unterbrochene M3-Topologie.

**K3 – Nicht beteiligte Eingangssynapsen**

Untersuche, wie häufig Ereignisspuren auch Synapsen enthalten, die zwar aktiv waren, aber nicht zum untersuchten Feuerereignis beigetragen haben.

**K4 – Gezielte Unterbrechung einzelner beteiligter Verbindungen**

Wiederhole ausgewählte Versuche aus identischem Ausgangszustand und unterbrich gezielt eine zuvor als beteiligt identifizierte Synapse.

Prüfe, ob sich das betreffende Feuerereignis oder der motorische Abschluss verändert.

### Wichtige Einschränkung

Eine Unterbrechung kann die gesamte nachfolgende Netzwerkaktivität verändern.

Deshalb gilt:

- Der Vergleich erfolgt aus identischen Ausgangszuständen.
- Es wird jeweils nur die untersuchte Veränderung vorgenommen.
- Die vollständigen Auswirkungen werden protokolliert.
- Ein unveränderter Ausgang widerlegt nicht automatisch eine Beteiligung, da mehrere Eingänge gemeinsam oder ersatzweise wirken können.
- Ein veränderter Ausgang allein beweist noch keine eindeutige Kausalität der gesamten Handlung.

Die Auswertung soll lokale Ereigniszuordnung und kontrollierte Kausalitätsprüfung getrennt darstellen.

## 8. Forschungsphase V – Ereignisgedächtnis und Handlungsabschluss

### Ziel

Prüfe, wie lange lokale Informationen über neuronale Beteiligung verfügbar bleiben.

Dabei werden keine physikalischen Zeitintervalle untersucht.

Stattdessen wird die Zahl zwischenzeitlich verarbeiteter Ereignisse variiert.

### Versuchsvarianten

Untersuche die lokale Ereigniszuordnung:

- Unmittelbar nach einem Feuerereignis.
- Nach weiteren eingehenden Impulsen.
- Nach einem vollständigen motorischen Abschluss.
- Nach anschließender sensorischer Eigenrückmeldung.

### Fragestellungen

- Sind relevante Eingangssynapsen noch erkennbar?
- Welche Informationen werden durch neue Ereignisse überschrieben?
- Wie hängt der Informationsverlust von der Puffergröße ab?
- Reicht ein rein lokaler Ringpuffer zur späteren Rückmeldungszuordnung?
- Welche zusätzlichen Zustände wären gegebenenfalls erforderlich?

Die Entwicklung neuer Eligibility Traces oder synaptischer Belohnungsmarkierungen ist noch nicht Bestandteil dieser Forschungsstufe.

Die Ergebnisse sollen zunächst zeigen, ob und warum solche Mechanismen überhaupt notwendig werden.

## 9. Refraktärphase und wiederholte Handlungen

Die Wiederholungsinstabilität aus Forschungsstufe A.2 bleibt als eigenständige Forschungsfrage bestehen.

Für die primäre Untersuchung werden neuronale Ausgangszustände vor jeder unabhängigen Handlung wiederhergestellt.

Dadurch soll die Auswertung lokaler Ereignisgedächtnisse nicht von unkontrollierten Zustandsveränderungen zwischen Handlungen abhängen.

Zusätzlich sind getrennte Kontrollversuche mit fortgeführtem neuronalen Zustand durchzuführen.

Dabei darf:

- Die Refraktärzeit nicht stillschweigend zurückgesetzt werden.
- Der Membranzustand nicht künstlich normalisiert werden.
- Eine neue Erholungsregel nicht ohne gesonderte Forschungsentscheidung eingeführt werden.

Wenn fortgeführte Handlungen ausbleiben, ist dies als Einschränkung zu dokumentieren.

Die Untersuchung lokaler Ereigniszuordnung kann dennoch aus den unabhängigen, kausal nachgewiesenen M3-Handlungen erfolgen.

## 10. Stichprobe und Reproduzierbarkeit

Verwende mindestens die 32 Seeds aus Forschungsstufe A.2.

Ergänze weitere Seeds, damit ausreichend viele nachgewiesene M3-Abschlüsse untersucht werden können.

Dabei gelten:

- Alle getesteten Seeds sind zu dokumentieren.
- Nicht abgeschlossene Versuche bleiben Bestandteil der Auswertung.
- Keine ausschließliche Auswahl erfolgreicher Seeds.
- Gleiche Seeds, Netzwerke und Eingaben müssen reproduzierbare Ergebnisse liefern.
- Erfolgreiche und erfolglose Handlungsverläufe sind getrennt zu analysieren.

Die Ergebnisse dürfen nicht ausschließlich aus den bereits bekannten sieben erfolgreichen M3-Abschlüssen abgeleitet werden.

## 11. Messgrößen

Erfasse mindestens:

- Anzahl aller neuronalen Feuerereignisse.
- Anzahl erfasster Eingangssynapsen.
- Anzahl und Anteil überschreibender Ereignisse.
- Größe des lokalen Ereignisgedächtnisses.
- Beteiligte Eingangssynapsen pro Feuerereignis.
- Anzahl aus lokalen Spuren rekonstruierbarer Schaltungsabschnitte.
- Anzahl vollständiger rekonstruierbarer M3-Handlungswege.
- Übereinstimmung mit der technischen Ereignisdiagnose.
- Falsch zugeordnete oder nicht eindeutig zuordenbare Eingänge.
- Unterschiede zwischen den Speichergrößen.
- Auswirkungen gezielter Verbindungsunterbrechungen.
- Speicherverbrauch pro Neuron.
- Rechenzeit und Ereignisdurchsatz.

Die technische Ereignisdiagnose dient als unabhängige Vergleichsgrundlage.

Sie darf nicht heimlich Bestandteil des neuronalen Ereignisgedächtnisses werden.

## 12. Performance und Speicherarchitektur

Die lokale Ereignisspeicherung soll möglichst kompakt sein.

Zunächst genügt eine indexbasierte Lösung mit numerischen Synapsen-IDs.

Bei vier Byte pro Synapsen-ID ergeben sich beispielhaft:

| Ereignisse pro Neuron | Speicher bei 256 Neuronen, nur ID-Einträge |
|---|---:|
| 2 | 2 KiB |
| 4 | 4 KiB |
| 8 | 8 KiB |
| 16 | 16 KiB |

Zusätzliche Verwaltungsdaten sind separat auszuweisen.

Für größere Netzwerke ist der voraussichtliche Speicherbedarf zu extrapolieren.

Ermittle außerdem den zusätzlichen Aufwand beim Empfangen eines Impulses und beim Erzeugen eines Feuerereignisses.

Die neuen Ereignisgedächtnisse dürfen keine vollständigen Netzwerkdurchläufe erforderlich machen.

## 13. Automatisierte Tests

Ergänze mindestens Tests für:

1. Korrekte Speicherung einzelner Eingangsereignisse.
2. Erhalt der Eingangsreihenfolge.
3. Wiederholte Aktivierung derselben Synapse.
4. Ringpufferüberschreibung.
5. Synapsen mit Gewichtung 0.
6. Momentaufnahme beim tatsächlichen Feuerentscheid.
7. Korrekte Verarbeitung während einer Refraktärphase.
8. Rekonstruktion eines einfachen neuronalen Feuerereignisses.
9. Rekonstruktion einer M3-Akkumulatoraktivierung.
10. Rekonstruktion eines M3-Abschlusses.
11. Erkennung nicht zuordenbarer Aktivität.
12. Verhalten bei unterbrochenen Schaltungswegen.
13. Reproduzierbarkeit bei identischem Seed.
14. Persistenz beziehungsweise ausdrücklich definierte Lebensdauer der Ereignisgedächtnisse.
15. Unveränderte neuronale Verarbeitung bei deaktiviertem Ereignisgedächtnis.
16. Bestehende automatisierte Tests.

## 14. Nicht Bestandteil dieser Forschungsstufe

Nicht implementieren:

- Neue Belohnungs- oder Bestrafungssignale.
- Abgestufte Erfolgsbewertung.
- Vorhersagefehler.
- Erwartungsbildung.
- Rückmeldungsabhängige Verstärkung.
- Rückmeldungsabhängige Abschwächung.
- Neue hemmende Synapsen.
- STDP.
- Synaptische Zeitstempel.
- Neue automatische Regulationsmechanismen.
- Vorgegebene Handlungsmuster.
- Sprachverständnis.
- Community-Lernen.

Diese Mechanismen folgen erst, nachdem wir die lokale Zuordnung neuronaler Aktivität ausreichend verstanden haben.

## 15. Forschungsbericht und Lieferumfang

Erstelle:

- Vollständigen Forschungsbericht B als Markdown.
- Dokumentation des lokalen Ereignisgedächtnisses.
- Reproduzierbare Versuchsskripte.
- Maschinenlesbare Rohdaten.
- Vergleich der Gedächtnisgrößen G0 bis G4.
- Rekonstruierte Beispiele erfolgreicher und erfolgloser Handlungen.
- Ergebnisse gezielter Verbindungsunterbrechungen.
- Speicher- und Performancemessungen.
- Erweiterte automatisierte Tests.
- Entscheidungsvorlage für die anschließende rückmeldungsabhängige Lernforschung.

Bestehende Dokumente, Rohdaten und lokale Änderungen sind zu erhalten.

## 16. Abschlusskriterien

Forschungsstufe B soll folgende Fragen beantworten:

1. Können Neuronen vorausgegangene Eingangssignale lokal und in ihrer Reihenfolge speichern?
2. Welche dieser Informationen sind beim eigenen Feuern noch verfügbar?
3. Können damit tatsächlich beteiligte Eingänge erfasst werden?
4. Wie zuverlässig lässt sich eine M3-Handlung aus solchen lokalen Informationen rekonstruieren?
5. Welche Speichergröße bietet einen sinnvollen Kompromiss zwischen Informationsgehalt und Ressourcenbedarf?
6. Welche Informationen gehen zwischen Handlung und späterer Rückmeldung verloren?
7. Welche zusätzlichen Mechanismen könnten für eine spätere gezielte synaptische Anpassung erforderlich sein?

Ein unterschiedliches Ereignisprotokoll allein genügt nicht.

Die Untersuchung soll zeigen, **ob das lokale Gedächtnis tatsächlich relevante Informationen über neuronale Beteiligung enthält und wie verlässlich diese Informationen sind.**

---

## Zentraler Forschungsgrundsatz

**Ein Neuron muss nicht wissen, ob seine Aktivität richtig oder falsch war. Aber es könnte sich daran erinnern, welche Eingänge seiner eigenen Aktivität vorausgingen.**

Forschungsstufe B untersucht, ob sich daraus eine brauchbare Grundlage für spätere rückmeldungsabhängige synaptische Veränderungen entwickeln lässt.

Die Refraktärphase bleibt für diese Untersuchung unverändert. Ihre Auswirkungen auf fortlaufende Handlungen werden getrennt dokumentiert und anschließend eigenständig behandelt.
